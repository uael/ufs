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

#define FS_FD_DFT (-1)

enum fs_kind {
  FS_KIND_DIR,
  FS_KIND_FILE,
  FS_FILE_DOT,
  FS8FILE_DOT2
};

typedef enum fs_kind fs_kind_t;
typedef i32_t fs_file_t;

__api__ bool_t
fs_file_exists(fs_file_t *__restrict self);

__api__ bool_t
fs_file_opened(fs_file_t __const *__restrict self);

__api__ ret_t
fs_file_open(fs_file_t *__restrict self, char_t __const *filename, u32_t flags);

__api__ ret_t
fs_file_close(fs_file_t *__restrict self);

__api__ ret_t
fs_file_read(fs_file_t *__restrict self, char_t *buf, usize_t len,
  isize_t *out);

__api__ ret_t
fs_file_write(fs_file_t *__restrict self, char_t __const *buf, usize_t len,
  isize_t *out);

__api__ ret_t
fs_file_seek(fs_file_t *__restrict self, isize_t off, fs_seek_mod_t whence,
  isize_t *out);

__api__ isize_t
fs_file_offset(fs_file_t *__restrict self);

#endif /* !__UFS_FILE_H */
