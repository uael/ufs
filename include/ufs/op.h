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

/*!@file ufs/op.h
 * @author uael
 */
#ifndef __UFS_OP_H
# define __UFS_OP_H

#include "conf.h"

__export__ ret_t
fs_absolute(char_t const *path, char_t *out);

__export__ bool_t
fs_cp(char_t const *path, char_t const *dest);

__export__ u16_t
fs_cwd(char_t *path, u16_t n);

__export__ bool_t
fs_exists(char_t const *path);

__export__ bool_t
fs_ln(char_t const *path, char_t const *dest);

__export__ ret_t
fs_mkdir(char_t const *path);

__export__ bool_t
fs_mv(char_t const *path, char_t const *dest);

__export__ bool_t
fs_rm(char_t const *path);

__export__ bool_t
fs_touch(char_t const *path);

#endif /* !__UFS_OP_H */
