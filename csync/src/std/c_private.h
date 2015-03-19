/*
 * cynapses libc functions
 *
 * Copyright (c) 2008-2013 by Andreas Schneider <asn@cryptomilk.org>
 * Copyright (c) 2012-2013 by Dominik Schmidt <dev@dominik-schmidt.de>
 * Copyright (c) 2012-2013 by Klaas Freitag <freitag@owncloud.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _C_PRIVATE_H
#define _C_PRIVATE_H

#include "config.h"

/* cross platform defines */
#include "config.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windef.h>
#include <winbase.h>
#endif

#ifdef _WIN32
#define EDQUOT 0
#define ENODATA 0
#define S_IRGRP 0
#define S_IROTH 0
#define S_IXGRP 0
#define S_IXOTH 0

#define S_IFSOCK 10000 /* dummy val on Win32 */
#define S_IFLNK 10001  /* dummy val on Win32 */

#define O_NOFOLLOW 0
#define O_NOCTTY 0

#define uid_t int
#define gid_t int
#define nlink_t int
#define getuid() 0
#define geteuid() 0
#endif

#ifdef _WIN32
typedef struct _stat csync_stat_t;
#else
typedef struct stat csync_stat_t;
#endif

#ifndef O_NOATIME
#define O_NOATIME 0
#endif

#ifndef ENODATA
#define ENODATA EBADF
#endif

#if !defined(HAVE_ASPRINTF) && defined(HAVE___MINGW_ASPRINTF)
#define asprintf __mingw_asprintf
#endif

#ifndef HAVE_STRERROR_R
#define strerror_r(errnum, buf, buflen) snprintf(buf, buflen, "%s", strerror(errnum))
#endif

#ifndef HAVE_LSTAT
#define lstat _stat
#endif
#ifdef _WIN32
#define fstat  _fstat
#endif


/* tchar definitions for clean win32 filenames */
#define _UNICODE

#if defined _WIN32 && defined _UNICODE
typedef  wchar_t         mbchar_t;
#define _topen           _wopen
#define _tdirent         _wdirent
#define _TDIR            _WDIR
#define _topendir        _wopendir
#define _tclosedir       _wclosedir
#define _treaddir        _wreaddir
#define _trewinddir      _wrewinddir
#define _ttelldir        _wtelldir
#define _tseekdir        _wseekdir
#define _tcreat          _wcreat
#define _tstat           _wstat
#define _tunlink         _wunlink
#define _tmkdir(X,Y)     _wmkdir(X)
#define _trmdir	         _rmdir
#define _tchmod          _wchmod
#define _trewinddir      _wrewinddir
#define _tchown(X, Y, Z) /* no chown on Win32 */
#else
typedef char           mbchar_t;
#define _tdirent       dirent
#define _topen         open
#define _TDIR          DIR
#define _topendir      opendir
#define _tclosedir     closedir
#define _treaddir      readdir
#define _trewinddir    rewinddir
#define _ttelldir      telldir
#define _tseekdir      seekdir
#define _tcreat        creat
#define _tstat         stat
#define _tunlink       unlink
#define _tmkdir(X,Y)   mkdir(X,Y)
#define _trmdir	       rmdir
#define _tchmod        chmod
#define _trewinddir    rewinddir
#define _tchown(X,Y,Z) chown(X,Y,Z)
#endif

#ifdef WITH_ICONV
/** @internal */
int c_setup_iconv(const char* to);
/** @internal */
int c_close_iconv(void);
#endif

#if defined(__GNUC__)
# define CSYNC_THREAD __thread
#elif defined(_MSC_VER)
# define CSYNC_THREAD __declspec(thread)
#else
# define CSYNC_THREAD
#endif

#endif //_C_PRIVATE_H

/* vim: set ft=c.doxygen ts=8 sw=2 et cindent: */
