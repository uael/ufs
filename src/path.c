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

#include <uty.h>
#include <ctype.h>
#include <ufs.h>

#include "ufs/path.h"

typedef path_t path_vec_t;

VEC_IMPL_ALLOC(path_vec, i8_t, 16, i8cmp, malloc, realloc, free);

path_t *
fs_path_ctor(path_t *self) {
  path_vec_ctor(self);
  return self;
}

path_t *
fs_path(path_t *self, i8_t const *path) {
  if (path_vec_append(fs_path_ctor(self), path, strlen(path)) != SUCCESS) {
    return nil;
  }
  return self;
}

path_t *
fs_pathn(path_t *self, i8_t const *path, const u16_t n) {
  if (path_vec_append(fs_path_ctor(self), (i8_t *) path, n) != SUCCESS) {
    return nil;
  }
  return self;
}

bool_t
fs_path_is_abs(path_t const *self) {
#ifdef OS_WIN
  return self->len > 2
    && isalpha(self->buf[0])
    && self->buf[1] == ':'
    && self->buf[2] == DS;
#else
  return *self->buf == DS;
#endif
}

/*
bool_t
fs_path_is_rel(path_t const *self);

bool_t
fs_path_is_root(path_t const *self);

bool_t
fs_path_is_home(path_t const *self);

path_t *
fs_path_abs(path_t *self, path_t *out);

path_t *
fs_path_rel(path_t *self, path_t *out);

path_t *
fs_path_base(path_t *self, path_t *out);

path_t *
fs_path_ext(path_t *self, path_t *out);

path_t *
fs_path_dir(path_t *self, path_t *out);

path_t *
fs_path_join(path_t *self, i8_t const *b, path_t *out);*/
