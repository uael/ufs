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
#include "mod.h"

enum fs_kind {
  FS_KIND_DIR,
  FS_KIND_FILE,
  FS_FILE_DOT,
  FS8FILE_DOT2
};

typedef enum fs_kind fs_kind_t;

struct fs_file {
#ifdef CC_MSVC
  HANDLE *fd;
#else
  i32_t fd;
#endif
  fs_kind_t kind;
  i32_t flags;
  i8_t const *filename;
  i8_t const *path;
  i8_t const *ext;
  vecof(i8_t, 16) cache;
};

typedef struct fs_file fs_file_t;

__extern_c__ ret_t
fs_file_open(fs_file_t *self, i8_t const *filename, u32_t flags);

__extern_c__ ret_t
fs_file_close(fs_file_t *self);

__extern_c__ ret_t
fs_file_read(fs_file_t *self, i8_t *buf, u64_t len, u64_t *out);

__extern_c__ ret_t
fs_file_write(fs_file_t *self, i8_t const *buf, u64_t len, u64_t *out);

__extern_c__ ret_t
fs_file_seek(fs_file_t *self, i64_t off, fs_seek_mod_t mod, u64_t *out);

__extern_c__ bool_t
fs_file_sync(fs_file_t *self);

__extern_c__ i64_t
fs_file_offset(fs_file_t *self);

#endif /* !__UFS_FILE_H */
