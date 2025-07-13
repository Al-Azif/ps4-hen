#include "sections.h"

#include "offsets/651.h"

// clang-format off

const struct kpayload_offsets offsets_651 PAYLOAD_RDATA = {
  // data
  .XFAST_SYSCALL_addr              = 0x0,
  .PRISON0_addr                    = 0x0,
  .ROOTVNODE_addr                  = 0x0,
  .M_TEMP_addr                     = 0x0,
  .MINI_SYSCORE_SELF_BINARY_addr   = 0x0,
  .ALLPROC_addr                    = 0x0,
  .SBL_DRIVER_MAPPED_PAGES_addr    = 0x0,
  .SBL_PFS_SX_addr                 = 0x0,
  .SBL_KEYMGR_KEY_SLOTS_addr       = 0x0,
  .SBL_KEYMGR_KEY_RBTREE_addr      = 0x0,
  .SBL_KEYMGR_BUF_VA_addr          = 0x0,
  .SBL_KEYMGR_BUF_GVA_addr         = 0x0,
  .FPU_CTX_addr                    = 0x0,
  .SYSENT_addr                     = 0x0,

  // common
  .memcmp_addr                     = 0x0,
  ._sx_xlock_addr                  = 0x0,
  ._sx_xunlock_addr                = 0x0,
  .malloc_addr                     = 0x0,
  .free_addr                       = 0x0,
  .strstr_addr                     = 0x0,
  .fpu_kern_enter_addr             = 0x0,
  .fpu_kern_leave_addr             = 0x0,
  .memcpy_addr                     = 0x0,
  .memset_addr                     = 0x0,
  .strlen_addr                     = 0x0,
  .printf_addr                     = 0x0,
  .eventhandler_register_addr      = 0x0,

  // Fself
  .sceSblACMgrGetPathId_addr       = 0x0,
  .sceSblServiceMailbox_addr       = 0x0,
  .sceSblAuthMgrSmIsLoadable2_addr = 0x0,
  ._sceSblAuthMgrGetSelfInfo_addr  = 0x0,
  ._sceSblAuthMgrSmStart_addr      = 0x0,
  .sceSblAuthMgrVerifyHeader_addr  = 0x0,

  // Fpkg
  .RsaesPkcs1v15Dec2048CRT_addr    = 0x0,
  .Sha256Hmac_addr                 = 0x0,
  .AesCbcCfb128Encrypt_addr        = 0x0,
  .AesCbcCfb128Decrypt_addr        = 0x0,
  .sceSblDriverSendMsg_0_addr      = 0x0,
  .sceSblPfsSetKeys_addr           = 0x0,
  .sceSblKeymgrSetKeyStorage_addr  = 0x0,
  .sceSblKeymgrSetKeyForPfs_addr   = 0x0,
  .sceSblKeymgrCleartKey_addr      = 0x0,
  .sceSblKeymgrSmCallfunc_addr     = 0x0,

  // Patch
  .vmspace_acquire_ref_addr        = 0x0,
  .vmspace_free_addr               = 0x0,
  .vm_map_lock_read_addr           = 0x0,
  .vm_map_unlock_read_addr         = 0x0,
  .vm_map_lookup_entry_addr        = 0x0,
  .proc_rwmem_addr                 = 0x0,

  // Fself hooks
  .sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId_hook        = 0x0,
  .sceSblAuthMgrIsLoadable2_hook                             = 0x0,
  .sceSblAuthMgrVerifyHeader_hook1                           = 0x0,
  .sceSblAuthMgrVerifyHeader_hook2                           = 0x0,
  .sceSblAuthMgrSmLoadSelfSegment__sceSblServiceMailbox_hook = 0x0,
  .sceSblAuthMgrSmLoadSelfBlock__sceSblServiceMailbox_hook   = 0x0,

  // Fpkg hooks
  .sceSblKeymgrSetKeyStorage__sceSblDriverSendMsg_hook       = 0x0,
  .sceSblKeymgrInvalidateKey__sx_xlock_hook                  = 0x0,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_isolated_rif_hook    = 0x0,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_rif_new_hook         = 0x0,
  .mountpfs__sceSblPfsSetKeys_hook1                          = 0x0,
  .mountpfs__sceSblPfsSetKeys_hook2                          = 0x0,

  // SceShellUI patches - debug patches - libkernel_sys.sprx
  .sceSblRcMgrIsAllowDebugMenuForSettings_patch              = 0x0,
  .sceSblRcMgrIsStoreMode_patch                              = 0x0,

  // SceShellUI patches - remote play patches
  .CreateUserForIDU_patch                                    = 0x0, // system_ex\app\NPXS20001\eboot.bin
  .remote_play_menu_patch                                    = 0x0, // system_ex\app\NPXS20001\psm\Application\app.exe.sprx

  // SceRemotePlay patches - remote play patches - system\vsh\app\NPXS21006
  .SceRemotePlay_patch1                                      = 0x0,
  .SceRemotePlay_patch2                                      = 0x0,

  // SceShellCore patches - call sceKernelIsGenuineCEX
  .sceKernelIsGenuineCEX_patch1    = 0x0,
  .sceKernelIsGenuineCEX_patch2    = 0x0,
  .sceKernelIsGenuineCEX_patch3    = 0x0,
  .sceKernelIsGenuineCEX_patch4    = 0x0,

  // SceShellCore patches - call nidf_libSceDipsw
  .nidf_libSceDipsw_patch1         = 0x0,
  .nidf_libSceDipsw_patch2         = 0x0,
  .nidf_libSceDipsw_patch3         = 0x0,
  .nidf_libSceDipsw_patch4         = 0x0,

  // SceShellCore patches - bypass firmware checks
  .check_disc_root_param_patch     = 0x0,
  .app_installer_patch             = 0x0,
  .check_system_version            = 0x0,
  .check_title_system_update_patch = 0x0,

  // SceShellCore patches - enable remote pkg installer
  .enable_data_mount_patch         = 0x0,

  // SceShellCore patches - enable VR without spoof
  .enable_psvr_patch               = 0x0,

  // SceShellCore patches - enable fpkg
  .enable_fpkg_patch               = 0x0,

  // SceShellCore patches - use `free` prefix instead `fake`
  .fake_free_patch                 = 0x0,

  // SceShellCore patches - enable official external HDD support
  .pkg_installer_patch             = 0x0,
  .ext_hdd_patch                   = 0x0,

  // SceShellCore patches - enable debug trophies
  .debug_trophies_patch            = 0x0,

  // SceShellCore patches - disable screenshot block
  .disable_screenshot_patch        = 0x0,

  // Process structure offsets
  .proc_p_comm_offset = 0x44C,
  .proc_path_offset   = 0x46C,
};

// clang-format on
