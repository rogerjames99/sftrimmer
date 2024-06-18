 
/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2003-2024 Christian Schoenebeck                         *
 *                           <cuse@users.sourceforge.net>                  *
 *                                                                         *
 *   This library is part of libgig.                                       *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this library; if not, write to the Free Software           *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,                 *
 *   MA  02111-1307  USA                                                   *
 ***************************************************************************/

#ifndef LIBGIG_SYSDEF_H
#define LIBGIG_SYSDEF_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef WIN32
# define POSIX 0
#endif

#ifndef POSIX
# define POSIX 1
#endif

#ifndef DEBUG
# define DEBUG 0
#endif

#if POSIX
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
#endif // POSIX

#if defined _MSC_VER && _MSC_VER < 1600
// Visual C++ 2008 doesn't have stdint.h
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

#ifdef WIN32
# if (_WIN32 && !_WIN64) || (__GNUC__ && !(__x86_64__ || __ppc64__)) /* if 32 bit windows compilation */
#  if _WIN32_WINNT < 0x0501
#   undef _WIN32_WINNT
#   define _WIN32_WINNT 0x0501 /* Win XP (no service pack): required for 32 bit compilation for GetFileSizeEx() to be declared by windows.h */
#  endif
# endif
# include <windows.h>
  typedef unsigned int   uint;
#endif // WIN32

#ifdef _MSC_VER
#include <BaseTsd.h>
using ssize_t = SSIZE_T;
#endif

#ifndef OVERRIDE
# if defined(__cplusplus) && __cplusplus >= 201103L
#  define OVERRIDE override
# else
#  define OVERRIDE
# endif
#endif

#ifdef __GNUC__
# define LIBGIG_DEPRECATED_API(msg) __attribute__ ((deprecated(msg)))
#else
# define LIBGIG_DEPRECATED_API(msg)
#endif

#endif // LIBGIG_SYSDEF_H
