#ifndef VERSIONINFO_H
#define VERSIONINFO_H

#include "version.h"

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#define VER_FILE_VERSION VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION
#define VER_FILE_VERSION_STR                                                                                           \
    STRINGIZE(VERSION_MAJOR)                                                                                           \
    "." STRINGIZE(VERSION_MINOR) "." STRINGIZE(VERSION_BUILD) "." STRINGIZE(VERSION_REVISION)

#define VER_FILE_VERSION_STR_INFO                                                                                      \
    STRINGIZE(VERSION_MAJOR)                                                                                           \
    "." STRINGIZE(VERSION_MINOR) "." STRINGIZE(VERSION_BUILD)

#define VER_PRODUCT_VERSION VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR VER_FILE_VERSION_STR
#define VER_ORIGINAL_FILENAME_STR VER_PRODUCTNAME_STR ".exe"
#define VER_INTERNAL_NAME_STR VER_ORIGINAL_FILENAME_STR

#define VER_PRODUCTNAME_STR "BIN_V_024_M_Remote"
#define VER_COPYRIGHT_STR "Copyright © 2022 PAO \"PNPPK\""
#define VER_FILE_DESCRIPTION "BIN_V_024_M_Remote"
#define VER_COMPANY_NAME "PNPPK JSC"
#define VER_PROGRAM_TITLE "Пульт для BIN-V-024-M"
#ifdef _DEBUG
#define VER_VER_DEBUG VS_FF_DEBUG
#else
#define VER_VER_DEBUG 0
#endif

#define VER_FILEOS VOS_NT_WINDOWS32
#define VER_FILEFLAGS VER_VER_DEBUG
#define VER_FILETYPE VFT_APP

#endif // VERSIONINFO_H
