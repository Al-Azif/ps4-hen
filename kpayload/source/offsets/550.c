#include "sections.h"

#include "offsets/550.h"

// clang-format off

const struct kpayload_offsets offsets_550 PAYLOAD_RDATA = {
  // data
  .XFAST_SYSCALL_addr              = 0x000001C0,
  .PRISON0_addr                    = 0x01134180,
  .ROOTVNODE_addr                  = 0x022EF570,
  .M_TEMP_addr                     = 0x01A8EFF0,
  .MINI_SYSCORE_SELF_BINARY_addr   = 0x01567618,
  .ALLPROC_addr                    = 0x0218D0E8,
  .SBL_DRIVER_MAPPED_PAGES_addr    = 0x0264F6C8,
  .SBL_PFS_SX_addr                 = 0x02664080,
  .SBL_KEYMGR_KEY_SLOTS_addr       = 0x02657700,
  .SBL_KEYMGR_KEY_RBTREE_addr      = 0x02657710,
  .SBL_KEYMGR_BUF_VA_addr          = 0x02658000,
  .SBL_KEYMGR_BUF_GVA_addr         = 0x02658808,
  .FPU_CTX_addr                    = 0x02668D40,
  .SYSENT_addr                     = 0x01115CC0,

  // common
  .memcmp_addr                     = 0x0005E270,
  ._sx_xlock_addr                  = 0x00482620,
  ._sx_xunlock_addr                = 0x004827E0,
  .malloc_addr                     = 0x00466AE0,
  .free_addr                       = 0x00466CE0,
  .strstr_addr                     = 0x000E4A50,
  .fpu_kern_enter_addr             = 0x0022C990,
  .fpu_kern_leave_addr             = 0x0022CA90,
  .memcpy_addr                     = 0x004059C0,
  .memset_addr                     = 0x00108810,
  .strlen_addr                     = 0x002A6CC0,
  .printf_addr                     = 0x0011AE10,
  .eventhandler_register_addr      = 0x0022D430,

  // Fself
  .sceSblACMgrGetPathId_addr       = 0x001B49C0,
  .sceSblServiceMailbox_addr       = 0x0064A260,
  .sceSblAuthMgrSmIsLoadable2_addr = 0x0065C0E0,
  ._sceSblAuthMgrGetSelfInfo_addr  = 0x0065C940,
  ._sceSblAuthMgrSmStart_addr      = 0x00656CE0,
  .sceSblAuthMgrVerifyHeader_addr  = 0x0065C140,

  // Fpkg
  .RsaesPkcs1v15Dec2048CRT_addr    = 0x002EA440,
  .Sha256Hmac_addr                 = 0x0031D200,
  .AesCbcCfb128Encrypt_addr        = 0x0045AA20,
  .AesCbcCfb128Decrypt_addr        = 0x0045AC50,
  .sceSblDriverSendMsg_0_addr      = 0x00635A60,
  .sceSblPfsSetKeys_addr           = 0x006413D0,
  .sceSblKeymgrSetKeyStorage_addr  = 0x0063C380,
  .sceSblKeymgrSetKeyForPfs_addr   = 0x0063E8C0,
  .sceSblKeymgrCleartKey_addr      = 0x0063EC40,
  .sceSblKeymgrSmCallfunc_addr     = 0x0063E490,

  // Patch
  .vmspace_acquire_ref_addr        = 0x00029C90,
  .vmspace_free_addr               = 0x00029AC0,
  .vm_map_lock_read_addr           = 0x00029E40,
  .vm_map_unlock_read_addr         = 0x00029E90,
  .vm_map_lookup_entry_addr        = 0x0002A470,
  .proc_rwmem_addr                 = 0x003931C0,

  // Fself hooks
  .sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId_hook        = 0x0065544D,
  .sceSblAuthMgrIsLoadable2_hook                             = 0x00655591,
  .sceSblAuthMgrVerifyHeader_hook1                           = 0x00655CEC,
  .sceSblAuthMgrVerifyHeader_hook2                           = 0x00656998,
  .sceSblAuthMgrSmLoadSelfSegment__sceSblServiceMailbox_hook = 0x006588D3,
  .sceSblAuthMgrSmLoadSelfBlock__sceSblServiceMailbox_hook   = 0x00659526,

  // Fpkg hooks
  .sceSblKeymgrSetKeyStorage__sceSblDriverSendMsg_hook       = 0x0063C425,
  .sceSblKeymgrInvalidateKey__sx_xlock_hook                  = 0x0063FAEA,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_isolated_rif_hook    = 0x00663CF0,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_rif_new_hook         = 0x00664AD3,
  .mountpfs__sceSblPfsSetKeys_hook1                          = 0x006B1628,
  .mountpfs__sceSblPfsSetKeys_hook2                          = 0x006B1857,

  // SceShellUI patches - debug patches - libkernel_sys.sprx
  .sceSblRcMgrIsAllowDebugMenuForSettings_patch              = 0x0001D4D0,
  .sceSblRcMgrIsStoreMode_patch                              = 0x0001D830,

  // SceShellUI patches - remote play patches
  .CreateUserForIDU_patch                                    = 0x001A3350, // system_ex\app\NPXS20001\eboot.bin
  .remote_play_menu_patch                                    = 0x00E84901, // system_ex\app\NPXS20001\psm\Application\app.exe.sprx

  // SceRemotePlay patches - remote play patches - system\vsh\app\NPXS21006
  .SceRemotePlay_patch1                                      = 0x0003C0B6,
  .SceRemotePlay_patch2                                      = 0x0003C0D1,

  // SceShellCore patches - call sceKernelIsGenuineCEX
  .sceKernelIsGenuineCEX_patch1    = 0x00177A4B,
  .sceKernelIsGenuineCEX_patch2    = 0x007B9CEB,
  .sceKernelIsGenuineCEX_patch3    = 0x00804783,
  .sceKernelIsGenuineCEX_patch4    = 0x009946EB,

  // SceShellCore patches - call nidf_libSceDipsw
  .nidf_libSceDipsw_patch1         = 0x00177A77,
  .nidf_libSceDipsw_patch2         = 0x0024A4ED,
  .nidf_libSceDipsw_patch3         = 0x007B9D17,
  .nidf_libSceDipsw_patch4         = 0x00994717,

  // SceShellCore patches - bypass firmware checks
  .check_disc_root_param_patch     = 0x00138F67,
  .app_installer_patch             = 0x00139061,
  .check_system_version            = 0x003CAD79,
  .check_title_system_update_patch = 0x003CDAD0,

  // SceShellCore patches - enable remote pkg installer
  .enable_data_mount_patch         = 0x0032779A,

  // SceShellCore patches - enable VR without spoof
  .enable_psvr_patch               = 0x00CE6C90,

  // SceShellCore patches - enable fpkg
  .enable_fpkg_patch               = 0x003DE492,

  // SceShellCore patches - use `free` prefix instead `fake`
  .fake_free_patch                 = 0x00F18910,

  // SceShellCore patches - enable official external HDD support
  .pkg_installer_patch             = 0x0097D071,
  .ext_hdd_patch                   = 0x0059C03D,

  // SceShellCore patches - enable debug trophies
  .debug_trophies_patch            = 0x006B8A09,

  // SceShellCore patches - disable screenshot block
  .disable_screenshot_patch        = 0x000D4766,

  // Process structure offsets
  .proc_p_comm_offset = 0x454,
  .proc_path_offset   = 0x474,
};

// clang-format on
