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

#include <ufs.h>
#include "ufs/file.h"
#include "ufs/path.h"

ret_t
fs_file_open(fs_file_t *self, i8_t const *filename, u32_t flags) {
  ret_t ret;
  fs_path_t path;

  if ((ret = fs_path_toabs(fs_path(&path, filename))) > 0) {
    return ret;
  }
  *self = init (fs_file_t,
    .flags = 0,
    .path = path.buf,
    .filename = filename
  );
#ifndef CC_MSVC
  {
    u32_t modes;
    
    if (flags & FS_OPEN_RO) self->flags |= O_RDONLY;
    else if (flags & FS_OPEN_WO) self->flags |= O_WRONLY;
    else if (flags & FS_OPEN_RW) self->flags |= O_RDWR;
    if (flags & FS_OPEN_CREAT) self->flags |= O_CREAT;
    if (flags & FS_OPEN_APPEND) self->flags |= O_APPEND;
    if (flags & FS_OPEN_TRUNC) self->flags |= O_TRUNC;
# ifdef OS_UNIX
    if (flags & FS_OPEN_DIRECT) self->flags |= O_DIRECT;
# endif
    self->flags |= O_NONBLOCK;
    modes = 0;
    if (flags & FS_OPEN_CREAT) {
      modes = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }
    self->fd = open(self->path, self->flags, modes);
  }
#else
  {
    DWORD access, share, cflag, attr;
    
    access = GENERIC_READ;
    if (flags & FS_OPEN_RO) access = GENERIC_READ;
    else if (flags & FS_OPEN_WO) access = GENERIC_WRITE;
    else if (flags & FS_OPEN_RW) access = GENERIC_READ | GENERIC_WRITE;
    DWORD share = FILE_SHARE_READ;
    if (flags & FS_OPEN_RO) share = FILE_SHARE_READ;
    else if (flags & FS_OPEN_WO) share = FILE_SHARE_WRITE;
    else if (flags & FS_OPEN_RW) share = FILE_SHARE_READ | FILE_SHARE_WRITE;
    cflag = 0;
    if (flags & (FS_OPEN_CREAT | FS_OPEN_TRUNC)) cflag |= CREATE_ALWAYS;
    else if (flags & FS_OPEN_CREAT) cflag |= CREATE_NEW;
    else if (flags & FS_OPEN_TRUNC) cflag |= TRUNCATE_EXISTING;
    if (!cflag) cflag |= OPEN_EXISTING;
    attr = FILE_ATTRIBUTE_NORMAL;
    if (flags & FS_OPEN_ASIO) attr |= FILE_FLAG_OVERLAPPED;
    if (flags & FS_OPEN_DIRECT) attr |= FILE_FLAG_NO_BUFFERING;
    self->fd = CreateFile(self->path, access, share, nil, cflag, attr, nil);
  }
#endif
  return RET_SUCCESS;
}
