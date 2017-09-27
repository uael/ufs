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

/*!@file ufs/file.h
 * @author uael
 */
#ifndef __UFS_FILE_H
# define __UFS_FILE_H

#include "conf.h"
#include "op.h"
#include "mod.h"

enum fs_kind {
  FS_KIND_DIR,
  FS_KIND_FILE,
  FS_FILE_DOT,
  FS8FILE_DOT2
};

typedef enum fs_kind fs_kind_t;

#ifdef OS_WIN
typedef HANDLE *fs_file_fd_t;
#else
typedef i32_t fs_file_fd_t;
#endif

struct fs_file {
  i8_t const *filename;
  i8_t const *path;
  fs_file_fd_t fd;
  i32_t flags;
  fs_kind_t kind;
};

typedef struct fs_file fs_file_t;

__export__ ret_t
fs_file_ctor(fs_file_t *self, i8_t const *filename);

__export__ ret_t
fs_file_dtor(fs_file_t *self);

__export__ bool_t
fs_file_exists(fs_file_t *self);

__export__ bool_t
fs_file_opened(fs_file_t *self);

__export__ ret_t
fs_file_open(fs_file_t *self, u32_t flags);

__export__ ret_t
fs_file_close(fs_file_t *self);

__export__ bool_t
fs_file_rm(fs_file_t *self);

__export__ bool_t
fs_file_touch(fs_file_t *self);

__export__ ret_t
fs_file_read(fs_file_t *self, i8_t *buf, u64_t len, i64_t *out);

__export__ ret_t
fs_file_write(fs_file_t *self, i8_t const *buf, u64_t len, u64_t *out);

__export__ ret_t
fs_file_seek(fs_file_t *self, i64_t off, fs_seek_mod_t whence, u64_t *out);

__export__ bool_t
fs_file_sync(fs_file_t *self);

__export__ i64_t
fs_file_offset(fs_file_t *self);

#endif /* !__UFS_FILE_H */
