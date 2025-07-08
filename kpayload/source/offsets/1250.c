#include "sections.h"

#include "offsets/1250.h"

// clang-format off

const struct kpayload_offsets offsets_1250 PAYLOAD_RDATA = {
  // data
  .XFAST_SYSCALL_addr              = 0x000001C0,
  .PRISON0_addr                    = 0x0111FA18,
  .ROOTVNODE_addr                  = 0x02136E90,
  .M_TEMP_addr                     = 0x01520D00,
  .MINI_SYSCORE_SELF_BINARY_addr   = 0x0153D6C8,
  .ALLPROC_addr                    = 0x01B28538,
  .SBL_DRIVER_MAPPED_PAGES_addr    = 0x02647350,
  .SBL_PFS_SX_addr                 = 0x0265C080,
  .SBL_KEYMGR_KEY_SLOTS_addr       = 0x02668040,
  .SBL_KEYMGR_KEY_RBTREE_addr      = 0x02668050,
  .SBL_KEYMGR_BUF_VA_addr          = 0x0266C000,
  .SBL_KEYMGR_BUF_GVA_addr         = 0x0266C808,
  .FPU_CTX_addr                    = 0x026542C0,
  .SYSENT_addr                     = 0x01102B70,

  // common
  .memcmp_addr                     = 0x003942E0,
  ._sx_xlock_addr                  = 0x000A3840,
  ._sx_xunlock_addr                = 0x000A3A00,
  .malloc_addr                     = 0x00009520,
  .free_addr                       = 0x000096E0,
  .strstr_addr                     = 0x0021CC90,
  .fpu_kern_enter_addr             = 0x001E0020,
  .fpu_kern_leave_addr             = 0x001E00E0,
  .memcpy_addr                     = 0x002BD4C0,
  .memset_addr                     = 0x001FA180,
  .strlen_addr                     = 0x0036AB70,
  .printf_addr                     = 0x002E0420,
  .eventhandler_register_addr      = 0x00224150,

  // Fself
  .sceSblACMgrGetPathId_addr       = 0x003B2DC0,
  .sceSblServiceMailbox_addr       = 0x0062F9A0,
  .sceSblAuthMgrSmIsLoadable2_addr = 0x0063C810,
  ._sceSblAuthMgrGetSelfInfo_addr  = 0x0063D050,
  ._sceSblAuthMgrSmStart_addr      = 0x0063DBE0,
  .sceSblAuthMgrVerifyHeader_addr  = 0x0063C870,

  // Fpkg
  .RsaesPkcs1v15Dec2048CRT_addr    = 0x0021BC40,
  .Sha256Hmac_addr                 = 0x001F8D80,
  .AesCbcCfb128Encrypt_addr        = 0x00340E70,
  .AesCbcCfb128Decrypt_addr        = 0x003410A0,
  .sceSblDriverSendMsg_0_addr      = 0x0061BFE0,
  .sceSblPfsSetKeys_addr           = 0x00626720,
  .sceSblKeymgrSetKeyStorage_addr  = 0x00624780,
  .sceSblKeymgrSetKeyForPfs_addr   = 0x0062B070,
  .sceSblKeymgrCleartKey_addr      = 0x0062B3B0,
  .sceSblKeymgrSmCallfunc_addr     = 0x0062AC40,

  // Patch
  .vmspace_acquire_ref_addr        = 0x002F6F60,
  .vmspace_free_addr               = 0x002F6D90,
  .vm_map_lock_read_addr           = 0x002F70F0,
  .vm_map_unlock_read_addr         = 0x002F7140,
  .vm_map_lookup_entry_addr        = 0x002F7730,
  .proc_rwmem_addr                 = 0x00365FE0,

  // Fself hooks
  .sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId_hook        = 0x0064202C,
  .sceSblAuthMgrIsLoadable2_hook                             = 0x0064217E,
  .sceSblAuthMgrVerifyHeader_hook1                           = 0x00642916,
  .sceSblAuthMgrVerifyHeader_hook2                           = 0x006435F9,
  .sceSblAuthMgrSmLoadSelfSegment__sceSblServiceMailbox_hook = 0x0064002D,
  .sceSblAuthMgrSmLoadSelfBlock__sceSblServiceMailbox_hook   = 0x00640C68,

  // Fpkg hooks
  .sceSblKeymgrSetKeyStorage__sceSblDriverSendMsg_hook       = 0x00624825,
  .sceSblKeymgrInvalidateKey__sx_xlock_hook                  = 0x0062C22D,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_isolated_rif_hook    = 0x0064C580,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_rif_new_hook         = 0x0064D34E,
  .mountpfs__sceSblPfsSetKeys_hook1                          = 0x006A2E39,
  .mountpfs__sceSblPfsSetKeys_hook2                          = 0x006A306A,

  // SceShellUI patches - debug patches - libkernel_sys.sprx
  .sceSblRcMgrIsAllowDebugMenuForSettings_patch              = 0x0001D100,
  .sceSblRcMgrIsStoreMode_patch                              = 0x0001D460,

  // SceShellUI patches - remote play patches
  .CreateUserForIDU_patch                                    = 0x0018B3B0, // system_ex\app\NPXS20001\eboot.bin
  .remote_play_menu_patch                                    = 0x00EC88C2, // system_ex\app\NPXS20001\psm\Application\app.exe.sprx

  // SceRemotePlay patches - remote play patches - system\vsh\app\NPXS21006
  .SceRemotePlay_patch1                                      = 0x000ED1F5,
  .SceRemotePlay_patch2                                      = 0x000ED210,

  // SceShellCore patches - call sceKernelIsGenuineCEX
  .sceKernelIsGenuineCEX_patch1    = 0x0016F5A4,
  .sceKernelIsGenuineCEX_patch2    = 0x00874644,
  .sceKernelIsGenuineCEX_patch3    = 0x008C4962,
  .sceKernelIsGenuineCEX_patch4    = 0x00A28224,

  // SceShellCore patches - call nidf_libSceDipsw
  .nidf_libSceDipsw_patch1         = 0x0016F5D2,
  .nidf_libSceDipsw_patch2         = 0x0024E11C,
  .nidf_libSceDipsw_patch3         = 0x00874672,
  .nidf_libSceDipsw_patch4         = 0x00A28252,

  // SceShellCore patches - bypass firmware checks
  .check_disc_root_param_patch     = 0xDEADC0DE,
  .app_installer_patch             = 0x001389A0,
  .check_system_version            = 0x003CA3A7,
  .check_title_system_update_patch = 0x003CD5F0,

  // SceShellCore patches - enable remote pkg installer
  .enable_data_mount_patch         = 0x00323380,

  // SceShellCore patches - enable VR without spoof
  .enable_psvr_patch               = 0x00DAF5C0,

  // SceShellCore patches - enable fpkg
  .enable_fpkg_patch               = 0x003DE07F,

  // SceShellCore patches - use `free` prefix instead `fake`
  .fake_free_patch                 = 0x00FD0E19,

  // SceShellCore patches - enable official external HDD support
  .pkg_installer_patch             = 0x00A11771,
  .ext_hdd_patch                   = 0x0061465D,

  // SceShellCore patches - enable debug trophies
  .debug_trophies_patch            = 0x0074D099,

  // SceShellCore patches - disable screenshot block
  .disable_screenshot_patch        = 0x000D2216,

  // Process structure offsets
  .proc_p_comm_offset = 0x454,
  .proc_path_offset   = 0x474,
};

// clang-format on
