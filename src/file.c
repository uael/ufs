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
#include "ufs/path.h"

#ifdef OS_WIN
# define FS_FD_DEFAULT INVALID_HANDLE_VALUE
#else
# define FS_FD_DEFAULT (-1)
#endif

ret_t
fs_file_ctor(fs_file_t *self, i8_t const *filename) {
  ret_t ret;
  fs_path_t path = {0}, cwd = {0};

  if ((ret = fs_path(&path, filename)) > 0) return ret;
  if ((ret = fs_path_absolute(&path, nil)) == RET_ERRNO) {
    if (errno != ENOENT) return ret;
    if ((ret = fs_path_cwd(&cwd)) > 0) return ret;
    if ((ret = fs_path_join(&cwd, &path)) > 0) return ret;
    path = cwd;
  }
  *self = init (fs_file_t,
    .fd = FS_FD_DEFAULT,
    .path = path.buf,
    .filename = filename
  );
  return RET_SUCCESS;
}

ret_t
fs_file_dtor(fs_file_t *self) {
  ret_t ret;

  if (fs_file_opened(self)) {
    if ((ret = fs_file_close(self)) > 0) {
      return ret;
    }
    self->fd = FS_FD_DEFAULT;
  }
  if (self->path) {
    free((i8_t *) self->path);
    self->path = nil;
  }
  return RET_SUCCESS;
}

FORCEINLINE bool_t
fs_file_exists(fs_file_t *self) {
  return fs_file_opened(self) || fs_exists(self->path);
}

FORCEINLINE bool_t
fs_file_opened(fs_file_t *self) {
  return (bool_t) (self->fd != FS_FD_DEFAULT);
}

ret_t
fs_file_open(fs_file_t *self, u32_t flags) {
  if (fs_file_opened(self)) {
    return RET_SUCCESS;
  }
  {
#ifdef OS_WIN
    ret_t ret;
    DWORD access, share, cflag, attr;

    self->flags = flags;
    access = GENERIC_READ;
    if (self->flags & FS_OPEN_RO) access = GENERIC_READ;
    else if (self->flags & FS_OPEN_WO) access = GENERIC_WRITE;
    else if (self->flags & FS_OPEN_RW) access = GENERIC_READ | GENERIC_WRITE;
    share = FILE_SHARE_READ;
    if (self->flags & FS_OPEN_RO) share = FILE_SHARE_READ;
    else if (self->flags & FS_OPEN_WO) share = FILE_SHARE_WRITE;
    else if (self->flags & FS_OPEN_RW) share = FILE_SHARE_READ | FILE_SHARE_WRITE;
    cflag = 0;
    if (self->flags & (FS_OPEN_CREAT | FS_OPEN_TRUNC)) cflag |= CREATE_ALWAYS;
    else if (self->flags & FS_OPEN_CREAT) cflag |= CREATE_NEW;
    else if (self->flags & FS_OPEN_TRUNC) cflag |= TRUNCATE_EXISTING;
    if (!cflag) cflag |= OPEN_EXISTING;
    attr = FILE_ATTRIBUTE_NORMAL;
    if (self->flags & FS_OPEN_ASIO) attr |= FILE_FLAG_OVERLAPPED;
    if (self->flags & FS_OPEN_DIRECT) attr |= FILE_FLAG_NO_BUFFERING;
    self->fd = CreateFile(
      (LPCSTR) self->path, access, share, nil, cflag, attr, nil
    );
    if (self->fd == INVALID_HANDLE_VALUE) {
      if (self->flags & FS_OPEN_CREAT) {
        if ((ret = fs_mkdir(self->path)) > 0) {
          return ret;
        }
        self->fd = CreateFile(
          (LPCSTR) self->path, access, share, nil, cflag, attr, nil
        );
        if (self->fd == INVALID_HANDLE_VALUE) {
          return RET_ERRNO;
        }
      }
      return RET_ERRNO;
    }
#else
    u32_t modes;

    self->flags = flags;
    flags = 0;
    if (self->flags & FS_OPEN_RO) flags |= O_RDONLY;
    else if (self->flags & FS_OPEN_WO) flags |= O_WRONLY;
    else if (self->flags & FS_OPEN_RW) flags |= O_RDWR;
    if (self->flags & FS_OPEN_CREAT) flags |= O_CREAT;
    if (self->flags & FS_OPEN_APPEND) flags |= O_APPEND;
    if (self->flags & FS_OPEN_TRUNC) flags |= O_TRUNC;
# ifdef OS_UNIX
    if (self->flags & FS_OPEN_DIRECT) flags |= O_DIRECT;
# endif
    flags |= O_NONBLOCK;
    modes = 0;
    if (self->flags & FS_OPEN_CREAT) {
      modes = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }
    self->fd = open(self->path, flags, modes);
    if (self->fd < 0) {
      return RET_ERRNO;
    }
#endif
  }
  return RET_SUCCESS;
}

ret_t
fs_file_close(fs_file_t *self) {
  ret_t ret;

  if (!fs_file_opened(self)) {
    return RET_FAILURE;
  }
#ifdef OS_WIN
  ret = CloseHandle(self->fd) ? RET_SUCCESS : RET_ERRNO;
#else
  ret = close(self->fd) == 0 ? RET_SUCCESS : RET_ERRNO;
#endif
  self->fd = FS_FD_DEFAULT;
  return ret;
}

bool_t
fs_file_rm(fs_file_t *self) {
  if (fs_file_opened(self)) {
    fs_file_close(self);
  }
  return fs_rm(self->path);
}

bool_t
fs_file_touch(fs_file_t *self) {
  if (fs_file_opened(self)) {
    return true;
  }
  return fs_touch(self->path);
}

FORCEINLINE ret_t
fs_file_read(fs_file_t *self, i8_t *buf, u64_t len, i64_t *out) {
  i64_t r;

  if (!fs_file_opened(self)) {
    return RET_FAILURE;
  }
#ifdef OS_WIN
  if (!ReadFile(self->fd, buf, (DWORD) len, &r, nil)) {
    return RET_ERRNO;
  }
#else
  if ((r = read(self->fd, buf, (size_t) len)) < 0) {
    return RET_ERRNO;
  }
#endif
  if (r == 0) {
    return RET_FAILURE;
  }
  *out = r;
  return RET_SUCCESS;
}
