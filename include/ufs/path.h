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

/*!@file ufs/path.h
 * @author uael
 */
#ifndef __UFS_PATH_H
# define __UFS_PATH_H

#include "conf.h"
#include "file.h"

typedef vecof(char_t, 16) fs_path_t;

SEQ_DECL_ctor(__api__, fs_path, char_t, 16);
SEQ_DECL_dtor(__api__, fs_path, char_t, 16);
SEQ_DECL_cpy(__api__, fs_path, char_t, 16);

__api__ ret_t
fs_path_cwd(fs_path_t *self);

__api__ ret_t
fs_path(fs_path_t *self, char_t const *path);

__api__ ret_t
fs_pathn(fs_path_t *self, char_t const *path, u16_t n);

__api__ bool_t
fs_path_is_abs(fs_path_t const *self);

__api__ bool_t
fs_path_is_rel(fs_path_t const *self);

__api__ ret_t
fs_path_absolute(fs_path_t *self, fs_path_t *out);

__api__ ret_t
fs_path_open(fs_path_t *self, fs_file_t *out, u32_t flags);

__api__ ret_t
fs_path_join(fs_path_t *self, fs_path_t *other);

#endif /* !__UFS_PATH_H */
