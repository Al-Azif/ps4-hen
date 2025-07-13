#include "sections.h"

#include "offsets/500.h"

// clang-format off

const struct kpayload_offsets offsets_500 PAYLOAD_RDATA = {
  // data
  .XFAST_SYSCALL_addr              = 0x000001C0,
  .PRISON0_addr                    = 0x010986A0,
  .ROOTVNODE_addr                  = 0x022C19F0,
  .M_TEMP_addr                     = 0x014B4110,
  .MINI_SYSCORE_SELF_BINARY_addr   = 0x014C9D48,
  .ALLPROC_addr                    = 0x02382FF8,
  .SBL_DRIVER_MAPPED_PAGES_addr    = 0x0271E208,
  .SBL_PFS_SX_addr                 = 0x0271E5D8,
  .SBL_KEYMGR_KEY_SLOTS_addr       = 0x02744548,
  .SBL_KEYMGR_KEY_RBTREE_addr      = 0x02744558,
  .SBL_KEYMGR_BUF_VA_addr          = 0x02748000,
  .SBL_KEYMGR_BUF_GVA_addr         = 0x02748800,
  .FPU_CTX_addr                    = 0x0274C040,
  .SYSENT_addr                     = 0x0107C610,

  // common
  .memcmp_addr                     = 0x00050AC0,
  ._sx_xlock_addr                  = 0x000F5B20,
  ._sx_xunlock_addr                = 0x000F5EC0,
  .malloc_addr                     = 0x0010E140,
  .free_addr                       = 0x0010E350,
  .strstr_addr                     = 0x0017DEA0,
  .fpu_kern_enter_addr             = 0x001BFE80,
  .fpu_kern_leave_addr             = 0x001BFF80,
  .memcpy_addr                     = 0x001EA420,
  .memset_addr                     = 0x003201F0,
  .strlen_addr                     = 0x003B6DD0,
  .printf_addr                     = 0x00435C70,
  .eventhandler_register_addr      = 0x001EC2F0,

  // Fself
  .sceSblACMgrGetPathId_addr       = 0x000117E0,
  .sceSblServiceMailbox_addr       = 0x00632160,
  .sceSblAuthMgrSmIsLoadable2_addr = 0x0063C110,
  ._sceSblAuthMgrGetSelfInfo_addr  = 0x0063C960,
  ._sceSblAuthMgrSmStart_addr      = 0x00641500,
  .sceSblAuthMgrVerifyHeader_addr  = 0x0063C170,

  // Fpkg
  .RsaesPkcs1v15Dec2048CRT_addr    = 0x001FD6C0,
  .Sha256Hmac_addr                 = 0x002D52E0,
  .AesCbcCfb128Encrypt_addr        = 0x003A2800,
  .AesCbcCfb128Decrypt_addr        = 0x003A2A30,
  .sceSblDriverSendMsg_0_addr      = 0x0061D410,
  .sceSblPfsSetKeys_addr           = 0x0061EBC0,
  .sceSblKeymgrSetKeyStorage_addr  = 0x00623BE0,
  .sceSblKeymgrSetKeyForPfs_addr   = 0x0062D3A0,
  .sceSblKeymgrCleartKey_addr      = 0x0062D730,
  .sceSblKeymgrSmCallfunc_addr     = 0x0062DEC0,

  // Patch
  .vmspace_acquire_ref_addr        = 0x0019EE80,
  .vmspace_free_addr               = 0x0019ECB0,
  .vm_map_lock_read_addr           = 0x0019F030,
  .vm_map_unlock_read_addr         = 0x0019F080,
  .vm_map_lookup_entry_addr        = 0x0019F650,
  .proc_rwmem_addr                 = 0x0030CDC0,

  // Fself hooks
  .sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId_hook        = 0x0063DE7D,
  .sceSblAuthMgrIsLoadable2_hook                             = 0x0063DFC1,
  .sceSblAuthMgrVerifyHeader_hook1                           = 0x0063E71C,
  .sceSblAuthMgrVerifyHeader_hook2                           = 0x0063F338,
  .sceSblAuthMgrSmLoadSelfSegment__sceSblServiceMailbox_hook = 0x00642DAB,
  .sceSblAuthMgrSmLoadSelfBlock__sceSblServiceMailbox_hook   = 0x006439C2,

  // Fpkg hooks
  .sceSblKeymgrSetKeyStorage__sceSblDriverSendMsg_hook       = 0x00623C85,
  .sceSblKeymgrInvalidateKey__sx_xlock_hook                  = 0x0062E58D,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_isolated_rif_hook    = 0x0064C340,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_rif_new_hook         = 0x0064D11F,
  .mountpfs__sceSblPfsSetKeys_hook1                          = 0x006AA6F5,
  .mountpfs__sceSblPfsSetKeys_hook2                          = 0x006AA924,

  // SceShellUI patches - debug patches - libkernel_sys.sprx
  .sceSblRcMgrIsAllowDebugMenuForSettings_patch              = 0x0001BD90,
  .sceSblRcMgrIsStoreMode_patch                              = 0x0001C090,

  // SceShellUI patches - remote play patches
  .CreateUserForIDU_patch                                    = 0x001A8C50, // system_ex\app\NPXS20001\eboot.bin
  .remote_play_menu_patch                                    = 0x00EE606E, // system_ex\app\NPXS20001\psm\Application\app.exe.sprx

  // SceRemotePlay patches - remote play patches - system\vsh\app\NPXS21006
  .SceRemotePlay_patch1                                      = 0x0003C33F,
  .SceRemotePlay_patch2                                      = 0x0003C35A,

  // SceShellCore patches - call sceKernelIsGenuineCEX
  .sceKernelIsGenuineCEX_patch1    = 0x0016D05B,
  .sceKernelIsGenuineCEX_patch2    = 0x007992FB,
  .sceKernelIsGenuineCEX_patch3    = 0x007E5503,
  .sceKernelIsGenuineCEX_patch4    = 0x00946C3B,

  // SceShellCore patches - call nidf_libSceDipsw
  .nidf_libSceDipsw_patch1         = 0x0016D087,
  .nidf_libSceDipsw_patch2         = 0x0023747B,
  .nidf_libSceDipsw_patch3         = 0x00799327,
  .nidf_libSceDipsw_patch4         = 0x00946C67,

  // SceShellCore patches - bypass firmware checks
  .check_disc_root_param_patch     = 0x0013097F,
  .app_installer_patch             = 0x00130A71,
  .check_system_version            = 0x003CCB79,
  .check_title_system_update_patch = 0x003CF8D0,

  // SceShellCore patches - enable remote pkg installer
  .enable_data_mount_patch         = 0x00319A53,

  // SceShellCore patches - enable VR without spoof
  .enable_psvr_patch               = 0x00C788D0,

  // SceShellCore patches - enable fpkg
  .enable_fpkg_patch               = 0x003E0602,

  // SceShellCore patches - use `free` prefix instead `fake`
  .fake_free_patch                 = 0x00EA7A47,

  // SceShellCore patches - enable official external HDD support
  .pkg_installer_patch             = 0x00930D81,
  .ext_hdd_patch                   = 0x005937DD,

  // SceShellCore patches - enable debug trophies
  .debug_trophies_patch            = 0x006AB929,

  // SceShellCore patches - disable screenshot block
  .disable_screenshot_patch        = 0x000CB8C6,

  // Process structure offsets
  .proc_p_comm_offset = 0x44C,
  .proc_path_offset   = 0x46C,
};

// clang-format on
