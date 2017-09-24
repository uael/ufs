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

#include <ctype.h>
#include <ufs.h>

#include "ufs/path.h"
#include "ufs/op.h"

SEQ_IMPL_realloc(
  static FORCEINLINE, fs_path, i8_t, 16, cap, len, buf, realloc, free, i8cmp
);

SEQ_DECL_ensure(static, fs_path, i8_t, 16) {
  if (self->cap < n) {
    return fs_path_realloc(self, n);
  }
  return RET_SUCCESS;
}

SEQ_DECL_grow(static, fs_path, i8_t, 16) {
  n += self->len;
  if (self->cap < n) {
    return fs_path_realloc(self, n);
  }
  return RET_SUCCESS;
}

SEQ_IMPL_append(
  static FORCEINLINE, fs_path, i8_t, 16, cap, len, buf, realloc, free, i8cmp
);

SEQ_IMPL_dtor(
  FORCEINLINE, fs_path, i8_t, 16, cap, len, buf, realloc, free, i8cmp
);

SEQ_IMPL_cpy(
  FORCEINLINE, fs_path, i8_t, 16, cap, len, buf, realloc, free, i8cmp
);

void
fs_path_ctor(fs_path_t *self) {
  *self = (fs_path_t){0};
}

ret_t
fs_path(fs_path_t *self, i8_t const *path) {
  ret_t ret;

  fs_path_ctor(self);
  if ((ret = fs_path_append(self, (i8_t *) path,
    (const u16_t) (strlen(path) + 1))) > 0) {
    return ret;
  }
  --self->len;
  return RET_SUCCESS;
}

ret_t
fs_pathn(fs_path_t *self, i8_t const *path, u16_t n) {
  ret_t ret;

  fs_path_ctor(self);
  if ((ret = fs_path_append(self, (i8_t *) path,
    (const u16_t) (n + 1))) > 0) {
    return ret;
  }
  --self->len;
  return RET_SUCCESS;
}

ret_t
fs_path_cwd(fs_path_t *self) {
  i8_t path[FS_PATH_MAX];
  u16_t n;

  if ((n = fs_cwd(path, FS_PATH_MAX)) > 0) {
    return fs_pathn(self, path, n);
  }
  return RET_FAILURE;
}

bool_t
fs_path_is_abs(fs_path_t const *self) {
#ifdef OS_WIN
  return *self->buf == '~' || (self->len > 2
    && isalpha(self->buf[0])
    && self->buf[1] == ':'
    && (self->buf[2] == '/' || self->buf[2] == '\\');
#else
  return *self->buf == '/' || *self->buf == '\\' ||*self->buf == '~';
#endif
}

bool_t
fs_path_is_rel(fs_path_t const *self) {
  return !fs_path_is_abs(self);
}

ret_t
fs_path_absolute(fs_path_t *self, fs_path_t *out) {
  i8_t path[FS_PATH_MAX], *ptr;

#ifndef CC_MSVC
  if ((ptr = realpath(self->buf, path)) == nil) {
    return RET_ERRNO;
  }
#else
  if (GetFullPathName(self->buf, FS_PATH_MAX, path, nil) == 0) {
      return RET_ERRNO;
    }
    ptr = path;
#endif
  if (out != nil) {
    fs_path(out, ptr);
  } else {
    self->len = 0;
    fs_path_append(self, ptr, (const u16_t) (strlen(ptr) + 1));
    --self->len;
  }
  return RET_SUCCESS;
}
