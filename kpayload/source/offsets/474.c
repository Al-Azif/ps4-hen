#include "sections.h"

#include "offsets/474.h"

// clang-format off

const struct kpayload_offsets offsets_474 PAYLOAD_RDATA = {
  // data
  .XFAST_SYSCALL_addr              = 0x0030B7D0,
  .PRISON0_addr                    = 0x01042AB0,
  .ROOTVNODE_addr                  = 0x021B89E0,
  .M_TEMP_addr                     = 0x0199BB80,
  .MINI_SYSCORE_SELF_BINARY_addr   = 0x01479558,
  .ALLPROC_addr                    = 0x01ADF718,
  .SBL_DRIVER_MAPPED_PAGES_addr    = 0x02525DD0,
  .SBL_PFS_SX_addr                 = 0x02529310,
  .SBL_KEYMGR_KEY_SLOTS_addr       = 0x02544DD0,
  .SBL_KEYMGR_KEY_RBTREE_addr      = 0x02544DE0,
  .SBL_KEYMGR_BUF_VA_addr          = 0x02548000,
  .SBL_KEYMGR_BUF_GVA_addr         = 0x02548800,
  .FPU_CTX_addr                    = 0x02549700,
  .SYSENT_addr                     = 0x01034790,

  // common
  .memcmp_addr                     = 0x00244EE0,
  ._sx_xlock_addr                  = 0x003907A0,
  ._sx_xunlock_addr                = 0x003909E0,
  .malloc_addr                     = 0x003F85C0,
  .free_addr                       = 0x003F87A0,
  .strstr_addr                     = 0x00263B90,
  .fpu_kern_enter_addr             = 0x00058B60,
  .fpu_kern_leave_addr             = 0x00058C60,
  .memcpy_addr                     = 0x00149D40,
  .memset_addr                     = 0x00304DD0,
  .strlen_addr                     = 0x00353720,
  .printf_addr                     = 0x00017F30,
  .eventhandler_register_addr      = 0x003CA6A0,

  // Fself
  .sceSblACMgrGetPathId_addr       = 0x00169840,
  .sceSblServiceMailbox_addr       = 0x00617AB0,
  .sceSblAuthMgrSmIsLoadable2_addr = 0x00629040,
  ._sceSblAuthMgrGetSelfInfo_addr  = 0x00629880,
  ._sceSblAuthMgrSmStart_addr      = 0x00625410,
  .sceSblAuthMgrVerifyHeader_addr  = 0x006290A0,

  // Fpkg
  .RsaesPkcs1v15Dec2048CRT_addr    = 0x003F0070,
  .Sha256Hmac_addr                 = 0x002D7E00,
  .AesCbcCfb128Encrypt_addr        = 0x00179720,
  .AesCbcCfb128Decrypt_addr        = 0x00179950,
  .sceSblDriverSendMsg_0_addr      = 0x00603CA0,
  .sceSblPfsSetKeys_addr           = 0x006095E0,
  .sceSblKeymgrSetKeyStorage_addr  = 0x006093D0,
  .sceSblKeymgrSetKeyForPfs_addr   = 0x006109E0,
  .sceSblKeymgrCleartKey_addr      = 0x00610D80,
  .sceSblKeymgrSmCallfunc_addr     = 0x00611530,

  // Patch
  .vmspace_acquire_ref_addr        = 0x00392D00,
  .vmspace_free_addr               = 0x00392B30,
  .vm_map_lock_read_addr           = 0x00392ED0,
  .vm_map_unlock_read_addr         = 0x00392F20,
  .vm_map_lookup_entry_addr        = 0x00393A90,
  .proc_rwmem_addr                 = 0x0017BDD0,

  // Fself hooks
  .sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId_hook        = 0x006224EC,
  .sceSblAuthMgrIsLoadable2_hook                             = 0x0062263F,
  .sceSblAuthMgrVerifyHeader_hook1                           = 0x00622D66,
  .sceSblAuthMgrVerifyHeader_hook2                           = 0x00623989,
  .sceSblAuthMgrSmLoadSelfSegment__sceSblServiceMailbox_hook = 0x00626CAA,
  .sceSblAuthMgrSmLoadSelfBlock__sceSblServiceMailbox_hook   = 0x006278D1,

  // Fpkg hooks
  .sceSblKeymgrSetKeyStorage__sceSblDriverSendMsg_hook       = 0x00609475,
  .sceSblKeymgrInvalidateKey__sx_xlock_hook                  = 0x00611C0D,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_isolated_rif_hook    = 0x006312F0,
  .sceSblKeymgrSmCallfunc_npdrm_decrypt_rif_new_hook         = 0x006320CE,
  .mountpfs__sceSblPfsSetKeys_hook1                          = 0x0069AFE4,
  .mountpfs__sceSblPfsSetKeys_hook2                          = 0x0069B214,

  // SceShellUI patches - debug patches - libkernel_sys.sprx
  .sceSblRcMgrIsAllowDebugMenuForSettings_patch              = 0x0001A130,
  .sceSblRcMgrIsStoreMode_patch                              = 0x0001A430,

  // SceShellUI patches - remote play patches
  .CreateUserForIDU_patch                                    = 0x00197030, // system_ex\app\NPXS20001\eboot.bin
  .remote_play_menu_patch                                    = 0x01299AF7, // system_ex\app\NPXS20001\libSceVsh_aot.sprx

  // SceRemotePlay patches - remote play patches - system\vsh\app\NPXS21006
  .SceRemotePlay_patch1                                      = 0x0003C882,
  .SceRemotePlay_patch2                                      = 0x0003C89D,

  // SceShellCore patches - call sceKernelIsGenuineCEX
  .sceKernelIsGenuineCEX_patch1    = 0x0014BC6B,
  .sceKernelIsGenuineCEX_patch2    = 0x006F3C5B,
  .sceKernelIsGenuineCEX_patch3    = 0x007278D3,
  .sceKernelIsGenuineCEX_patch4    = 0x0086168B,

  // SceShellCore patches - call nidf_libSceDipsw
  .nidf_libSceDipsw_patch1         = 0x0014BC97,
  .nidf_libSceDipsw_patch2         = 0x001FEAA8,
  .nidf_libSceDipsw_patch3         = 0x006F3C87,
  .nidf_libSceDipsw_patch4         = 0x008616B7,

  // SceShellCore patches - bypass firmware checks
  .check_disc_root_param_patch     = 0xDEADC0DE,
  .app_installer_patch             = 0x001213A1,
  .check_system_version            = 0x00375BF9,
  .check_title_system_update_patch = 0x00377E00,

  // SceShellCore patches - enable remote pkg installer
  .enable_data_mount_patch         = 0x002D055C,

  // SceShellCore patches - enable VR without spoof
  .enable_psvr_patch               = 0x00B3CDC0,

  // SceShellCore patches - enable fpkg
  .enable_fpkg_patch               = 0x00385032,

  // SceShellCore patches - use `free` prefix instead `fake`
  .fake_free_patch                 = 0x00D50208,

  // SceShellCore patches - enable official external HDD support
  .pkg_installer_patch             = 0x00853241,
  .ext_hdd_patch                   = 0x0050951D,

  // SceShellCore patches - enable debug trophies
  .debug_trophies_patch            = 0x0062C679,

  // SceShellCore patches - disable screenshot block
  .disable_screenshot_patch        = 0x000B7F4B,

  // Process structure offsets
  .proc_p_comm_offset = 0x444,
  .proc_path_offset   = 0x464,
};

// clang-format on
