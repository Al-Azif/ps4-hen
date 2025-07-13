#include "path.h"
#include "ps4.h"
#include <stdbool.h>

// https://github.com/idc/ps4-experiments-405/blob/361738a2ee8a0fd32090c80bd2b49dae94ff08a5/hostapp_launch_patcher/source/patch.c#L57
static int get_code_info(const int pid, const void *addrstart, uint64_t *paddress, uint64_t *psize, const uint32_t page_idx) {
  int mib[4] = {1, 14, 32, pid};
  size_t size = 0, count = 0;
  char *data;
  char *entry;

  if (sysctl(mib, 4, NULL, &size, NULL, 0) < 0) {
    return -1;
  }

  if (size == 0) {
    return -2;
  }

  data = (char *)malloc(size);
  if (data == NULL) {
    return -3;
  }

  if (sysctl(mib, 4, data, &size, NULL, 0) < 0) {
    free(data);
    return -4;
  }

  int struct_size = *(int *)data;
  count = size / struct_size;
  entry = data;

  int found = 0;
  int valid = 0;
  uint32_t idx = 0;
  uint64_t first_addr = 0;
  while (count != 0) {
    int type = *(int *)(&entry[0x4]);
    uint64_t start_addr = *(uint64_t *)(&entry[0x8]);
    uint64_t end_addr = *(uint64_t *)(&entry[0x10]);
    uint64_t code_size = end_addr - start_addr;
    uint32_t prot = *(uint32_t *)(&entry[0x38]);

    if (addrstart && start_addr == (uint64_t)addrstart) {
      valid = 1;
      idx = 0;
      first_addr = start_addr;
    } else if (!first_addr && addrstart) {
      goto next;
    }

    printf_debug("idx %d page_idx %d\n", idx, page_idx);
    printf_debug("%d %lx %lx (%lu) %x\n", type, start_addr, end_addr, code_size, prot);

    if ((valid && idx == page_idx) || (page_idx == 0 && type == 9 && (prot == 4 || prot == 5))) {
      *paddress = start_addr;
      *psize = code_size;
      found = 1;
      break;
    }

  next:
    entry += struct_size;
    count--;
    idx++;
  }

  free(data);
  return !found ? -5 : 0;
}

static int sys_proc_rw(const int pid, const uintptr_t addr, const void *data, const uint64_t datasz, const uint64_t write_) {
  return syscall(108, pid, addr, data, datasz, write_);
}

static uint32_t pattern_to_byte(const char *pattern, uint8_t *bytes) {
  uint32_t count = 0;
  const char *start = pattern;
  const char *end = pattern + strlen(pattern);

  for (const char *current = start; current < end; ++current) {
    if (*current == '?') {
      ++current;
      if (*current == '?') {
        ++current;
      }
      bytes[count++] = -1;
    } else {
      bytes[count++] = strtol(current, (char **)&current, 16);
    }
  }
  return count;
}

/*
 * @brief Scan for a given byte pattern on a module
 *
 * @param module_base Base of the module to search
 * @param module_size Size of the module to search
 * @param signature   IDA-style byte array pattern
 * @credit
 * https://github.com/OneshotGH/CSGOSimple-master/blob/59c1f2ec655b2fcd20a45881f66bbbc9cd0e562e/CSGOSimple/helpers/utils.cpp#L182
 * @returns           Address of the first occurrence
 */
static uintptr_t PatternScan(const void *module_base, const uint64_t module_size, const char *signature, const uint64_t offset) {
  if (!module_base || !module_size) {
    return 0;
  }
// constexpr uint32_t MAX_PATTERN_LENGTH = 256;
#define MAX_PATTERN_LENGTH 512
  uint8_t patternBytes[MAX_PATTERN_LENGTH] = {0};
  int32_t patternLength = pattern_to_byte(signature, patternBytes);
  if (!patternLength || patternLength >= MAX_PATTERN_LENGTH) {
    return 0;
  }
  uint8_t *scanBytes = (uint8_t *)module_base;

  for (uint64_t i = 0; i < module_size; ++i) {
    bool found = true;
    for (int32_t j = 0; j < patternLength; ++j) {
      if (scanBytes[i + j] != patternBytes[j] && patternBytes[j] != 0xff) {
        found = false;
        break;
      }
    }
    if (found) {
      return (((uintptr_t)&scanBytes[i] + offset));
    }
  }
  return 0;
}

static uintptr_t pid_chunk_scan(const int pid, const uintptr_t mem_start, const uintptr_t mem_sz, const char *pattern, const size_t pattern_offset) {
#define chunk_size (8ul * 1024)
  uintptr_t found = 0;
  uint8_t mem[chunk_size];
  for (size_t i = 0; i < (mem_sz - chunk_size); i += chunk_size) {
    const uintptr_t chunk_start = mem_start + i;
    sys_proc_rw(pid, chunk_start, (const void *)mem, chunk_size, 0);
    const uintptr_t mem_start = (uintptr_t)mem;
    const uintptr_t found_pattern = PatternScan(mem, chunk_size, pattern, pattern_offset);
    if (found_pattern) {
      const uintptr_t chunk_offset = found_pattern - mem_start;
      const uintptr_t chunk_found = chunk_start + chunk_offset;
      found = chunk_found;
      printf_debug("found data at 0x%lx 0x%lx chunk loc 0x%lx offset 0x%lx\n", found_pattern, chunk_offset, chunk_start, chunk_found);
      break;
    }
    sceKernelUsleep(1);
  }
  printf_debug("free mem\n");
  if (!found) {
    printf_debug("couldn't find pattern\n"
                 "%s\n"
                 "pid %d\n", pattern, pid);
  }
  return found;
#undef chunk_size
}

static int sys_proc_memset(const int pid, const uintptr_t src, const uint32_t c, const uint64_t len) {
#define max_len 8ul * 1024
  if (len > max_len) {
    printf_debug("Attempting to memset pid %d with length %lu is larger than maximum length %lu\n", pid, len, max_len);
    return -1;
  }
  uint8_t temp[max_len];
  memset(temp, c, len);
  return sys_proc_rw(pid, src, temp, len, 1);
#undef max_len
}

static void WriteJump32_pid(const int pid, const uintptr_t src, const uintptr_t dst, const uint64_t len, const bool call) {
  const int jmpcallbytes = 5;
  if (!src || !dst || len < jmpcallbytes) {
    return;
  }
  if (len != jmpcallbytes) {
    sys_proc_memset(pid, src, 0x90, len);
  }
  const int32_t relativeAddress = ((uintptr_t)dst - (uintptr_t)src) - jmpcallbytes;
  const uint8_t op = call ? 0xe8 : 0xe9;
  sys_proc_rw(pid, src, &op, sizeof(op), 1);
  sys_proc_rw(pid, src + 1, &relativeAddress, sizeof(relativeAddress), 1);
}

void InstallShellCoreCodeForAppinfo(void) {
  if (!file_exists(IS_SHELLCORE_APPINFO_INSTALLED_PATH)) {
    const int shellcore_pid = findProcess("SceShellCore");
    if (shellcore_pid > 0) {
      uintptr_t sc_start = 0;
      uint64_t sc_size = 0;
      int codeerr = get_code_info(shellcore_pid, 0, &sc_start, &sc_size, 0);
      if (codeerr == 0) {
        const uintptr_t ehframe_header = pid_chunk_scan(shellcore_pid, sc_start, sc_size, "14 00 00 00 00 00 00 00 01 7a 52 00 01 78 10 01 1b 0c 07 08 90 01 00 00", 0);
        const uintptr_t sc_update = pid_chunk_scan(shellcore_pid, sc_start, sc_size, "e8 ? ? ? ? 48 89 df e8 ? ? ? ? 48 89 df e8 ? ? ? ? e8 ? ? ? ? 31 c0", 21);
        printf_debug("ehframe_header %lx\n"
                     "sc_update %lx\n", ehframe_header, sc_update);
        // clang-format off
        // small shellcode to start `/data/hen/plugin_shellcore.prx`
        static unsigned char code[] = {
            0xeb, 0x2e, 0x48, 0x31, 0xc0, 0x49, 0x89, 0xca, 
            0x0f, 0x05, 0xc3, 0x48, 0x89, 0xd1, 0x48, 0x89, 
            0xf2, 0x89, 0xfe, 0xbf, 0x4f, 0x02, 0x00, 0x00, 
            0x31, 0xc0, 0xeb, 0xe6, 0x48, 0x89, 0xf1, 0x48, 
            0x89, 0xfe, 0xbf, 0x52, 0x02, 0x00, 0x00, 0x31, 
            0xd2, 0x45, 0x31, 0xc0, 0x31, 0xc0, 0xeb, 0xd2, 
            0x53, 0x48, 0x83, 0xec, 0x10, 0x48, 0x8d, 0x5c, 
            0x24, 0x04, 0xc7, 0x03, 0x00, 0x00, 0x00, 0x00, 
            0x48, 0x8d, 0x3d, 0x48, 0x00, 0x00, 0x00, 0x48,
            0x89, 0xde, 0xe8, 0xcd, 0xff, 0xff, 0xff, 0x85,
            0xc0, 0x0f, 0x94, 0xc0, 0x8b, 0x3b, 0x85, 0xff,
            0x0f, 0x9f, 0xc1, 0x20, 0xc1, 0x80, 0xf9, 0x01,
            0x75, 0x25, 0x48, 0x8d, 0x5c, 0x24, 0x08, 0x48,
            0xc7, 0x03, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8d,
            0x35, 0x39, 0x00, 0x00, 0x00, 0x48, 0x89, 0xda,
            0xe8, 0x8e, 0xff, 0xff, 0xff, 0x48, 0x8b, 0x03,
            0x48, 0x85, 0xc0, 0x74, 0x02, 0xff, 0xd0, 0x31,
            0xc0, 0x48, 0x83, 0xc4, 0x10, 0x5b, 0xc3, 0x2f,
            0x64, 0x61, 0x74, 0x61, 0x2f, 0x68, 0x65, 0x6e,
            0x2f, 0x70, 0x6c, 0x75, 0x67, 0x69, 0x6e, 0x5f,
            0x73, 0x68, 0x65, 0x6c, 0x6c, 0x63, 0x6f, 0x72,
            0x65, 0x2e, 0x70, 0x72, 0x78, 0x00, 0x70, 0x6c,
            0x75, 0x67, 0x69, 0x6e, 0x5f, 0x6c, 0x6f, 0x61,
            0x64, 0x00, };
        // clang-format on
        if (ehframe_header && sc_update) {
          sys_proc_rw(shellcore_pid, ehframe_header, code, sizeof(code), 1);
          WriteJump32_pid(shellcore_pid, sc_update, ehframe_header, 5, true);
          touch_file(IS_SHELLCORE_APPINFO_INSTALLED_PATH);
        }
      }
    }
  }
}
