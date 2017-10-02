/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!@file ufs/conf.h
 * @author uael
 */
#ifndef __UFS_CONF_H
# define __UFS_CONF_H

#include <unt.h>
#include <uds.h>

#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef OS_WIN
# include <io.h>

# define EOL "\r\n"
#else
# include <unistd.h>
# include <dirent.h>

# define EOL "\n"
#endif

#if defined OS_WIN || defined OS_OS2
# define DS '\\'
#else
# define DS '/'
#endif

#if defined PATH_MAX
# define FS_PATH_MAX PATH_MAX
#elif defined MAX_PATH
# define FS_PATH_MAX MAX_PATH
#elif defined _MAX_PATH
# define FS_PATH_MAX _MAX_PATH
#elif defined MAXPATHLEN
# define FS_PATH_MAX MAXPATHLEN
#else
# define FS_PATH_MAX (4096)
#endif

#if defined FILENAME_MAX && (FILENAME_MAX <= U8_MAX)
# define FS_FILENAME_MAX PATH_MAX
#elif defined NAME_MAX && (NAME_MAX <= U8_MAX)
# define FS_FILENAME_MAX (NAME_MAX)
#elif defined MAXNAMLEN && (MAXNAMLEN <= U8_MAX)
# define FS_FILENAME_MAX (MAXNAMLEN)
#else
# define FS_FILENAME_MAX (U8_MAX)
#endif

#endif /* !__UFS_CONF_H */
