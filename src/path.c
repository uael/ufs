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
  static FORCEINLINE, fs_path, char_t, 16, cap, len, buf, realloc, free, i8cmp
);

SEQ_IMPL_ensure_strict(
  static FORCEINLINE, fs_path, char_t, 16, cap, len, buf, realloc, free, i8cmp
);

SEQ_IMPL_grow_strict(
  static FORCEINLINE, fs_path, char_t, 16, cap, len, buf, realloc, free, i8cmp
);

SEQ_IMPL_append_nt(
  static FORCEINLINE, fs_path, char_t, 16, cap, len, buf, realloc, free, i8cmp
);

SEQ_IMPL_nappend(
  static FORCEINLINE, fs_path, char_t, 16, cap, len, buf, realloc, free, i8cmp
);

SEQ_IMPL_dtor(
  FORCEINLINE, fs_path, char_t, 16, cap, len, buf, realloc, free, i8cmp
);

SEQ_IMPL_cpy_nt(
  FORCEINLINE, fs_path, char_t, 16, cap, len, buf, realloc, free, i8cmp
);

void
fs_path_ctor(fs_path_t *__restrict__ self) {
  *self = (fs_path_t){0};
}

ret_t
fs_path(fs_path_t *self, char_t const *path) {
  ret_t ret;

  fs_path_ctor(self);
  if ((ret = fs_path_append(self, (char_t *) path)) > 0) {
    return ret;
  }
  return RET_SUCCESS;
}

ret_t
fs_pathn(fs_path_t *self, char_t const *path, u16_t n) {
  ret_t ret;

  fs_path_ctor(self);
  if ((ret = fs_path_nappend(self, (char_t *) path, (const u16_t) n)) > 0) {
    return ret;
  }
  return RET_SUCCESS;
}

ret_t
fs_path_cwd(fs_path_t *self) {
  char_t path[FS_PATH_MAX];
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
    && (self->buf[2] == '/' || self->buf[2] == '\\'));
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
  char_t path[FS_PATH_MAX], *ptr;

#ifdef OS_WIN
  if (_fullpath(self->buf, path, FS_PATH_MAX) == nil) {
    return RET_ERRNO;
  }
  ptr = path;
#else
  if ((ptr = realpath(self->buf, path)) == nil) {
    return RET_ERRNO;
  }
#endif
  if (out != nil) {
    return fs_path(out, ptr);
  }
  self->len = 0;
  return fs_path_append(self, ptr);
}

ret_t
fs_path_open(fs_path_t *self, fs_file_t *out, u32_t flags) {
  (void) self;
  (void) out;
  (void) flags;
  return RET_FAILURE;
}

ret_t
fs_path_join(fs_path_t *self, fs_path_t *other) {
  ret_t ret;

  if ((ret = fs_path_ensure(self, (u16_t) (other->len + 1))) > 0) {
    return ret;
  }
  self->buf[self->len++] = DS;
  return fs_path_nappend(self, other->buf, other->len);
}
