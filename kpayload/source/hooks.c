// Most of this code has been taken from ps4debug
// https://github.com/kruniak/ps4debug
//

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "amd_helper.h"
#include "elf_helper.h"
#include "freebsd_helper.h"
#include "offsets.h"
#include "sbl_helper.h"
#include "sections.h"
#include "self_helper.h"
#include "sparse.h"

#include "hooks.h"

#include "../../installer/include/path.h"

extern char *(*strstr)(const char *haystack, const char *needle)PAYLOAD_BSS;
extern void *(*memcpy)(void *dst, const void *src, size_t len)PAYLOAD_BSS;
extern void *(*memset)(void *s, int c, size_t n)PAYLOAD_BSS;
extern int (*printf)(const char *fmt, ...) PAYLOAD_BSS;

extern struct vmspace *(*vmspace_acquire_ref)(struct proc *p)PAYLOAD_BSS;
extern void (*vmspace_free)(struct vmspace *vm) PAYLOAD_BSS;
extern void (*vm_map_lock_read)(struct vm_map *map) PAYLOAD_BSS;
extern void (*vm_map_unlock_read)(struct vm_map *map) PAYLOAD_BSS;
extern int (*vm_map_lookup_entry)(struct vm_map *map, uint64_t address, struct vm_map_entry **entries) PAYLOAD_BSS;
extern int (*proc_rwmem)(struct proc *p, struct uio *uio) PAYLOAD_BSS;

extern struct proc **ALLPROC PAYLOAD_BSS;
extern struct sysent *SYSENT PAYLOAD_BSS;

extern int (*sys_dynlib_load_prx)(void *param_1, void *param_2) PAYLOAD_BSS;
extern int (*sys_dynlib_dlsym)(void *param_1, void *param_2) PAYLOAD_BSS;

extern int proc_get_vm_map(struct proc *p, struct proc_vm_map_entry **entries, size_t *num_entries) PAYLOAD_CODE;

PAYLOAD_CODE int sys_proc_list(struct thread *td, struct sys_proc_list_args *uap) {
  struct proc *p;
  int num;
  int r;

  r = 0;

  if (!uap->num) {
    r = 1;
    goto finish;
  }

  if (!uap->procs) {
    // count
    num = 0;
    p = *ALLPROC;
    do {
      num++;
    } while ((p = p->p_forw));

    *uap->num = num;
  } else {
    // fill structure
    num = *uap->num;
    p = *ALLPROC;
    for (int i = 0; i < num; i++) {
      char *p_comm = proc_get_p_comm(p);
      memcpy(uap->procs[i].p_comm, p_comm, sizeof(uap->procs[i].p_comm));
      uap->procs[i].pid = p->pid;

      if (!(p = p->p_forw)) {
        break;
      }
    }
  }

finish:
  td->td_retval[0] = r;
  return r;
}

PAYLOAD_CODE struct proc *proc_find_by_pid(int pid) {
  struct proc *p;

  p = *ALLPROC;
  do {
    if (p->pid == pid) {
      return p;
    }
  } while ((p = p->p_forw));

  return NULL;
}

PAYLOAD_CODE int proc_rw_mem(struct proc *p, void *ptr, uint64_t size, void *data, uint64_t *n, int write) {
  struct thread *td = curthread();
  struct iovec iov;
  struct uio uio;
  int r = 0;

  if (!p) {
    return 1;
  }

  if (size == 0) {
    if (n) {
      *n = 0;
    }

    return 0;
  }

  memset(&iov, '\0', sizeof(iov));
  iov.iov_base = (uint64_t)data;
  iov.iov_len = size;

  memset(&uio, '\0', sizeof(uio));
  uio.uio_iov = (uint64_t)&iov;
  uio.uio_iovcnt = 1;
  uio.uio_offset = (uint64_t)ptr;
  uio.uio_resid = (uint64_t)size;
  uio.uio_segflg = UIO_SYSSPACE;
  uio.uio_rw = write ? UIO_WRITE : UIO_READ;
  uio.uio_td = td;

  r = proc_rwmem(p, &uio);

  if (n) {
    *n = (uint64_t)((uint64_t)size - uio.uio_resid);
  }

  return r;
}

PAYLOAD_CODE int sys_proc_rw(struct thread *td, struct sys_proc_rw_args *uap) {
  struct proc *p;
  int r;

  r = 1;

  p = proc_find_by_pid(uap->pid);
  if (p) {
    r = proc_rw_mem(p, (void *)uap->address, uap->length, uap->data, 0, uap->write);
  }

  td->td_retval[0] = r;
  return r;
}

PAYLOAD_CODE void install_syscall(uint32_t n, void *func) {
  struct sysent *p = &SYSENT[n];
  memset(p, '\0', sizeof(struct sysent));
  p->sy_narg = 8;
  p->sy_call = func;
  p->sy_thrcnt = 1;
}

PAYLOAD_CODE void *get_syscall(uint64_t n) {
  struct sysent *p = &SYSENT[n];
  return p->sy_call;
}

int sys_proc_info_handle(struct proc *p, struct sys_proc_info_args *args) {
  args->pid = p->pid;
  char *p_comm = proc_get_p_comm(p);
  memcpy(args->name, p_comm, sizeof(args->name));
  char *path = proc_get_path(p);
  memcpy(args->path, path, sizeof(args->path));
  memcpy(args->titleid, p->titleid, sizeof(args->titleid));
  memcpy(args->contentid, p->contentid, sizeof(args->contentid));
  return 0;
}

int sys_proc_thrinfo_handle(struct proc *p, struct sys_proc_thrinfo_args *args) {
  struct thread *thr;

  TAILQ_FOREACH(thr, &p->p_threads, td_plist) {
    if (thr->tid == args->lwpid) {
      args->priority = thr->td_priority;
      memcpy(args->name, thr->td_name, sizeof(args->name));
      break;
    }
  }

  if (thr && thr->tid == args->lwpid) {
    return 0;
  }

  return 1;
}

PAYLOAD_CODE int sys_proc_vm_map_handle(struct proc *p, struct sys_proc_vm_map_args *args) {
  struct vm_map_entry *entry = NULL;

  struct vmspace *vm = vmspace_acquire_ref(p);
  if (!vm) {
    return -1;
  }

  struct vm_map *map = &vm->vm_map;

  int num = map->nentries;
  if (!num) {
    vmspace_free(vm);
    return 0;
  }

  vm_map_lock_read(map);

  if (!args->maps) {
    args->num = num;
  } else {
    if (vm_map_lookup_entry(map, 0, &entry)) {
      vm_map_unlock_read(map);
      vmspace_free(vm);
      return -1;
    }

    for (uint64_t i = 0; i < args->num; i++) {
      args->maps[i].start = entry->start;
      args->maps[i].end = entry->end;
      args->maps[i].offset = entry->offset;
      args->maps[i].prot = entry->prot & (entry->prot >> 8);
      memcpy(args->maps[i].name, entry->name, sizeof(args->maps[i].name));

      if (!(entry = entry->next)) {
        break;
      }
    }
  }

  vm_map_unlock_read(map);
  vmspace_free(vm);

  return 0;
}

PAYLOAD_CODE int sys_proc_cmd(struct thread *td, struct sys_proc_cmd_args *uap) {
  struct proc *p;
  int r;

  p = proc_find_by_pid(uap->pid);
  if (!p) {
    r = 1;
    goto finish;
  }

  switch (uap->cmd) {
  case SYS_PROC_VM_MAP:
    r = sys_proc_vm_map_handle(p, (struct sys_proc_vm_map_args *)uap->data);
    break;
  default:
    r = 1;
    break;
  }

finish:
  td->td_retval[0] = r;
  return r;
}

PAYLOAD_CODE static int dlsym_wrap(struct thread *td, int module, const char *sym, uintptr_t *out) {
  struct dynlib_dlsym_args dlsym_args = {};
  dlsym_args.module = module;
  dlsym_args.symbol = sym;
  dlsym_args.symbol_ptr = out;
  return sys_dynlib_dlsym(td, &dlsym_args);
}

PAYLOAD_CODE int sys_dynlib_load_prx_hook(struct thread *td, struct dynlib_load_prx_args *args) {
  const int r = sys_dynlib_load_prx(td, args);
  // https://github.com/OpenOrbis/mira-project/blob/d8cc5790f08f93267354c2370eb3879edba0aa98/kernel/src/Plugins/Substitute/Substitute.cpp#L1003
  const char *titleid = td->td_proc->titleid;
  const char *p = args->prx_path ? args->prx_path : "";
  // printf("%s td_name %s titleid %s prx %s\n", __FUNCTION__, td->td_name, titleid, p);
  const uint8_t jmp[] = {0xff, 0x25, 0x00, 0x00, 0x00, 0x00};
  if (strstr(p, "/app0/sce_module/libc.prx")) {
    const int handle_out = args->handle_out ? *args->handle_out : 0;
    struct dynlib_load_prx_args my_args = {};
    int handle = 0;
    my_args.prx_path = PRX_BOOTLOADER_PATH;
    my_args.handle_out = &handle;
    sys_dynlib_load_prx(td, &my_args);
    uintptr_t init_env_ptr = 0;
    dlsym_wrap(td, handle_out, "_init_env", &init_env_ptr);
    uintptr_t plugin_load_ptr = 0;
    dlsym_wrap(td, handle, "plugin_load", &plugin_load_ptr);
    if (init_env_ptr && plugin_load_ptr) {
      proc_rw_mem(td->td_proc, (void *)init_env_ptr, sizeof(jmp), (void *)jmp, 0, 1);
      proc_rw_mem(td->td_proc, (void *)(init_env_ptr + sizeof(jmp)), sizeof(plugin_load_ptr), &plugin_load_ptr, 0, 1);
    }
  }
  const bool isPartyDaemon = strstr(td->td_name, "ScePartyDaemonMain") != NULL;
  const bool isShellUI = strstr(td->td_name, "SceShellUIMain") != NULL;
  // printf("%d %d\n", isPartyDaemon, isShellUI);
  if (strstr(p, "/common/lib/libSceSysmodule.sprx") && (isPartyDaemon || isShellUI)) {
    // dummy process to load server prx into
    struct dynlib_load_prx_args my_args = {};
    int handle = 0;
    if (isPartyDaemon) {
      my_args.prx_path = PRX_SERVER_PATH;
    } else if (isShellUI) {
      my_args.prx_path = PRX_MONO_PATH;
    }
    my_args.handle_out = &handle;
    sys_dynlib_load_prx(td, &my_args);
    uintptr_t init_env_ptr = 0;
    dlsym_wrap(td, 0x2, "_init_env", &init_env_ptr);
    uintptr_t plugin_load_ptr = 0;
    dlsym_wrap(td, handle, "plugin_load", &plugin_load_ptr);
    if (init_env_ptr && plugin_load_ptr) {
      proc_rw_mem(td->td_proc, (void *)init_env_ptr, sizeof(jmp), (void *)jmp, 0, 1);
      proc_rw_mem(td->td_proc, (void *)(init_env_ptr + sizeof(jmp)), sizeof(plugin_load_ptr), &plugin_load_ptr, 0, 1);
    }
    printf("%s init env 0x%lx plugin load 0x%lx\n", titleid, init_env_ptr, plugin_load_ptr);
  }
  return r;
}

struct sys_jailbreak_ucred {
  uint32_t useless1;
  uint32_t cr_uid;
  uint32_t cr_ruid;
  uint32_t useless2;
  uint32_t useless3;
  uint32_t cr_rgid;
  uint32_t useless4;
  void *useless5;
  void *useless6;
  void *cr_prison;
  void *useless7;
  uint32_t useless8;
  void *useless9[2];
  void *useless10;
  char auditinfo_addr[184];
  uint32_t *cr_groups;
  uint32_t useless12;
};

struct sys_jailbreak_filedesc {
  void *useless1[3];
  void *fd_rdir;
  void *fd_jdir;
};

PAYLOAD_CODE void *sys_jailbreak(struct thread *td) {
  struct sys_jailbreak_ucred *cred = (struct sys_jailbreak_ucred *)td->td_proc->p_ucred;
  struct sys_jailbreak_filedesc *fd = (struct sys_jailbreak_filedesc *)td->td_proc->p_fd;

  void *td_ucred = *(void **)(((char *)td) + 304); // p_ucred == td_ucred

  const uint64_t kernel_ptr = getkernbase(fw_offsets->XFAST_SYSCALL_addr);
  void **got_prison0 = (void **)((uint8_t *)kernel_ptr + fw_offsets->PRISON0_addr);
  void **got_rootvnode = (void **)((uint8_t *)kernel_ptr + fw_offsets->ROOTVNODE_addr);

  cred->cr_uid = 0;
  cred->cr_ruid = 0;
  cred->cr_rgid = 0;
  cred->cr_groups[0] = 0;
  cred->cr_prison = *got_prison0;
  fd->fd_rdir = fd->fd_jdir = *got_rootvnode;

  // sceSblACMgrIsSystemUcred
  uint64_t *sonyCred = (uint64_t *)(((char *)td_ucred) + 96);
  *sonyCred = 0xFFFFFFFFFFFFFFFFULL;

  // sceSblACMgrGetDeviceAccessType
  uint64_t *sceProcType = (uint64_t *)(((char *)td_ucred) + 88);
  *sceProcType = 0x3801000000000013; // Max access

  // sceSblACMgrHasSceProcessCapability
  uint64_t *sceProcCap = (uint64_t *)(((char *)td_ucred) + 104);
  *sceProcCap = 0xFFFFFFFFFFFFFFFFULL; // Sce Process

  return 0;
}

PAYLOAD_CODE void install_nobd_syscall_hooks(void) {
  uint64_t flags, cr0;

  cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);
  flags = intr_disable();

  install_syscall(9, sys_jailbreak);

  intr_restore(flags);
  writeCr0(cr0);
}

PAYLOAD_CODE void install_syscall_hooks(void) {
  uint64_t flags, cr0;

  cr0 = readCr0();
  writeCr0(cr0 & ~X86_CR0_WP);
  flags = intr_disable();

  // proc
  install_syscall(107, sys_proc_list);
  install_syscall(108, sys_proc_rw);
  install_syscall(109, sys_proc_cmd);
  if (sys_dynlib_load_prx && sys_dynlib_dlsym) {
    install_syscall(594, sys_dynlib_load_prx_hook);
  }

  intr_restore(flags);
  writeCr0(cr0);
}
