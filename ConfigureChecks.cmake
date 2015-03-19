include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckTypeSize)
include(CheckCXXSourceCompiles)

set(PACKAGE ${APPLICATION_NAME})
set(VERSION ${APPLICATION_VERSION})
set(DATADIR ${DATA_INSTALL_DIR})
set(LIBDIR ${LIB_INSTALL_DIR})

if ( WITH_LOCAL_PLUGINDIR)
  set(PLUGINDIR ".")
else ( WITH_LOCAL_PLUGINDIR  )
  set(PLUGINDIR "${PLUGIN_INSTALL_DIR}-${LIBRARY_SOVERSION}")
endif ( WITH_LOCAL_PLUGINDIR )

set(SYSCONFDIR ${SYSCONF_INSTALL_DIR})

set(BINARYDIR ${CMAKE_BINARY_DIR})
set(SOURCEDIR ${CMAKE_SOURCE_DIR})

# HEADER FILES
check_include_file(argp.h HAVE_ARGP_H)

# FUNCTIONS
if (NOT LINUX)
    # librt
    check_library_exists(rt nanosleep "" HAVE_LIBRT)

    set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} )
endif (NOT LINUX)

check_library_exists(rt clock_gettime "" HAVE_CLOCK_GETTIME)
if (HAVE_LIBRT OR HAVE_CLOCK_GETTIME)
    set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} rt)
endif (HAVE_LIBRT OR HAVE_CLOCK_GETTIME)

check_library_exists(dl dlopen "" HAVE_LIBDL)
if (HAVE_LIBDL)
    find_library(DLFCN_LIBRARY dl)
    set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${DLFCN_LIBRARY})
endif (HAVE_LIBDL)

check_function_exists(asprintf HAVE_ASPRINTF)
if(NOT HAVE_ASPRINTF)
    if(MINGW)
        add_definitions( -D__USE_MINGW_ANSI_STDIO=1 )
    endif()
endif()

check_function_exists(fnmatch HAVE_FNMATCH)
if(NOT HAVE_FNMATCH AND WIN32)
  find_library(SHLWAPI_LIBRARY shlwapi)
endif()

if(WIN32)
  set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} psapi kernel32)
endif()

check_function_exists(strerror_r HAVE_STRERROR_R)
check_function_exists(utimes HAVE_UTIMES)
check_function_exists(lstat HAVE_LSTAT)
check_function_exists(asprintf HAVE_ASPRINTF)
if (UNIX AND HAVE_ASPRINTF)
  add_definitions(-D_GNU_SOURCE)
endif (UNIX AND HAVE_ASPRINTF)
if (WIN32)
  check_function_exists(__mingw_asprintf HAVE___MINGW_ASPRINTF)
endif(WIN32)

if (UNIT_TESTING)
    set(WITH_UNIT_TESTING ON)
endif (UNIT_TESTING)

set(CSYNC_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} CACHE INTERNAL "csync required system libraries")
