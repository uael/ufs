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

/*!@file ufs/mod.h
 * @author uael
 */
#ifndef __UFS_MOD_H
# define __UFS_MOD_H

enum fs_open_mod {
  FS_OPEN_RO = 1 << 0,
  FS_OPEN_WO = 1 << 1,
  FS_OPEN_RW = 1 << 2,
  FS_OPEN_CREAT = 1 << 3,
  FS_OPEN_APPEND = 1 << 4,
  FS_OPEN_TRUNCATE = 1 << 5,
  FS_OPEN_DIRECT = 1 << 6,
  FS_OPEN_ASIO = 1 << 7
};

enum fs_seek_mod {
  FS_SEEK_BEG = 0,
  FS_SEEK_CUR,
  FS_SEEK_END
};

typedef enum fs_open_mod fs_open_mod_t;
typedef enum fs_seek_mod fs_seek_mod_t;

#endif /* !__UFS_MOD_H */
