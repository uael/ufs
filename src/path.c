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

typedef fs_path_t __fs_path_t;

VEC_IMPL_ALLOC(__fs_path, i8_t, 16, i8cmp, malloc, realloc, free);

i16_t
fs_cwd(i8_t *path, u16_t n) {
  return 0;
}

fs_path_t *
fs_path_ctor(fs_path_t *self) {
  __fs_path_ctor(self);
  return self;
}

fs_path_t *
fs_path_cwd(fs_path_t *self) {
  i8_t path[U8_MAX];

  if (fs_cwd(path, U8_MAX)) {
    return fs_path(self, path);
  }
  return nil;
}

fs_path_t *
fs_path(fs_path_t *self, i8_t const *path) {
  if (__fs_path_append(fs_path_ctor(self), (i8_t *) path,
    (const u16_t) strlen(path)) != RET_SUCCESS) {
    return nil;
  }
  return self;
}

fs_path_t *
fs_pathn(fs_path_t *self, i8_t const *path, const u16_t n) {
  if (__fs_path_append(fs_path_ctor(self), (i8_t *) path, n) != RET_SUCCESS) {
    return nil;
  }
  return self;
}

bool_t
fs_path_is_abs(fs_path_t const *self) {
#ifdef OS_WIN
  return *self->buf == '~' || (self->len > 2
    && isalpha(self->buf[0])
    && self->buf[1] == ':'
    && self->buf[2] == DS);
#else
  return *self->buf == DS;
#endif
}

bool_t
fs_path_is_rel(fs_path_t const *self) {
  return !fs_path_is_abs(self);
}

fs_path_t *
fs_path_absto(fs_path_t *self, i8_t const *root, fs_path_t *out) {
  u64_t size;
  i8_t path[U8_MAX];

  if (fs_path_is_abs(self)) {
    __fs_path_cpy(out, self);
    return out;
  }
  size = 0;
  if (!root) {

    if (fs_cwd(path, U8_MAX) < 0) {
      return nil;
    }
    root = path;
  }
  __fs_path_append(self, (i8_t *) root, (const u16_t) strlen(root));
}
