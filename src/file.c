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

#include "ufs/file.h"

#if !defined O_RDONLY
# define O_RDONLY _O_RDONLY
# define O_WRONLY _O_WRONLY
# define O_RDWR _O_RDWR
# define O_APPEND _O_APPEND
# define O_CREAT _O_CREAT
# define O_TRUNC _O_TRUNC
# define O_EXCL _O_EXCL
# define O_TEXT _O_TEXT
# define O_BINARY _O_BINARY
# define O_RAW _O_BINARY
# define O_TEMPORARY _O_TEMPORARY
# define O_NOINHERIT _O_NOINHERIT
# define O_SEQUENTIAL _O_SEQUENTIAL
# define O_RANDOM _O_RANDOM
#endif

#if !defined S_IFMT
# define S_IFMT _S_IFMT
# define S_IFDIR _S_IFDIR
# define S_IFCHR _S_IFCHR
# define S_IFREG _S_IFREG
# define S_IREAD _S_IREAD
# define S_IWRITE _S_IWRITE
# define S_IEXEC _S_IEXEC
#endif

FORCEINLINE bool_t
fs_file_exists(fs_file_t *__restrict self) {
  return fs_file_opened(self);
}

FORCEINLINE bool_t
fs_file_opened(__const fs_file_t *__restrict self) {
  return (bool_t) (self && *self != FS_FD_DFT);
}

ret_t
fs_file_open(fs_file_t *__restrict self, char_t __const *filename,
  u32_t flags) {
  if (fs_file_opened(self)) {
    return RET_SUCCESS;
  }
  u32_t modes, uflags;

  uflags = flags;
  flags = 0;
  if (uflags & FS_OPEN_RO) flags |= O_RDONLY;
  else if (uflags & FS_OPEN_WO) flags |= O_WRONLY;
  else if (uflags & FS_OPEN_RW) flags |= O_RDWR;
  if (uflags & FS_OPEN_CREAT) flags |= O_CREAT;
  if (uflags & FS_OPEN_APPEND) flags |= O_APPEND;
  if (uflags & FS_OPEN_TRUNC) flags |= O_TRUNC;
  modes = 0;
#ifdef OS_UNIX
  if (uflags & FS_OPEN_DIRECT) flags |= O_DIRECT;
  flags |= O_NONBLOCK;
  if (uflags & FS_OPEN_CREAT) {
    modes = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  }
#elif defined OS_WIN
  if (uflags & FS_OPEN_CREAT) {
    modes = S_IREAD | S_IWRITE;
  }
#endif
#if defined OS_WIN
  *self = _open(filename, flags, modes);
#else
  *self = open(filename, flags, modes);
#endif
  if (*self == FS_FD_DFT) {
    return RET_ERRNO;
  }
  return RET_SUCCESS;
}

ret_t
fs_file_close(fs_file_t *__restrict self) {
  if (!fs_file_opened(self)) {
    return RET_FAILURE;
  }
#if defined OS_WIN
  return _close(*self) == 0 ? RET_SUCCESS : RET_ERRNO;
#else
  return close(*self) == 0 ? RET_SUCCESS : RET_ERRNO;
#endif
}

FORCEINLINE ret_t
fs_file_read(fs_file_t *__restrict self, char_t *buf, usize_t len, isize_t *out) {
  isize_t r;

  if (!fs_file_opened(self) ||
#if defined OS_WIN
    (r = _read(*self, buf, (unsigned) (len - 1))) == 0) {
#else
    (r = read(*self, buf, len)) == 0) {
#endif
    return RET_FAILURE;
  }
  if (r < 0) {
    return RET_ERRNO;
  }
  *out = r;
  return RET_SUCCESS;
}

FORCEINLINE ret_t
fs_file_write(fs_file_t *__restrict self, char_t __const *buf, usize_t len, isize_t *out) {
  isize_t r;

  if (!fs_file_opened(self) ||
#if defined OS_WIN
    (r = _write(*self, buf, (unsigned) len)) == 0) {
#else
    (r = write(*self, buf, len)) == 0) {
#endif
    return RET_FAILURE;
  }
  if (r < 0) {
    return RET_ERRNO;
  }
  *out = (u64_t) r;
  return RET_SUCCESS;
}

FORCEINLINE ret_t
fs_file_seek(fs_file_t *__restrict self, isize_t off, fs_seek_mod_t whence,
  isize_t *out) {
  isize_t r;
#if defined OS_WIN
  if ((r = _lseek(*self, (long) off, whence)) < 0) {
#else
  if ((r = lseek(*self, (long) off, whence)) < 0) {
#endif
    return errno != 0 ? RET_ERRNO : RET_FAILURE;
  }
  if (out != nil) {
    *out = r;
  }
  return RET_SUCCESS;
}

isize_t
fs_file_offset(fs_file_t *__restrict self) {
  i64_t off;

  switch (fs_file_seek(self, 0, FS_SEEK_CUR, &off)) {
    case RET_SUCCESS:
      return off;
    case RET_FAILURE:
      return 0;
    default:
    case RET_ERRNO:
      return -1;
  }
}
