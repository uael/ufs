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

#include "ufs/op.h"

ret_t
fs_absolute(i8_t const *path, i8_t *out) {
#ifndef CC_MSVC
  i8_t *ptr;

  if ((ptr = realpath(path, out)) == nil) {
    return RET_ERRNO;
  }
  (void) ptr;
#else
  if (GetFullPathName((LPCSTR) path, FS_PATH_MAX, (LPSTR) out, nil) == 0) {
    return RET_ERRNO;
  }
#endif
  return RET_SUCCESS;
}

bool_t
fs_exists(i8_t *path) {
#ifdef CC_MSVC
  DWORD attributes;

  attributes = GetFileAttributes((LPCSTR) path);
  if (attributes == INVALID_FILE_ATTRIBUTES) {
    return (bool_t) (GetLastError() == ERROR_BAD_NETPATH);
  }
  return true;
#else
  if (access(path, F_OK) == 0) {
    return true;
  }
#endif
  return false;
}

u16_t
fs_cwd(i8_t *path, u16_t n) {
#ifndef CC_MSVC
  char *ret;

  if ((ret = getcwd(path, n)) == nil)
    return 0;
  return (i16_t) strlen(ret);
#else
  return (i16_t) GetCurrentDirectory((DWORD) n, (LPTSTR) path);
#endif
}

ret_t
fs_mkdir(i8_t const *path) {
#ifdef CC_MSVC
  if (!CreateDirectoryA((LPCSTR) path, nil)) {
#else
  if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
#endif
    ret_t ret;
    i8_t *t, full[FS_PATH_MAX], temp[FS_PATH_MAX] = {0};
    i8_t const* p, *e;

    if ((ret = fs_absolute(path, full)) > 0) {
      return ret;
    }
    p = full;
    t = temp;
    e = temp + FS_PATH_MAX - 1;
    for (; t < e && *p; t++)  {
      *t = *p;
      if (*p == '/') {
        if (!fs_exists(temp)) {
#ifdef CC_MSVC
          if (!CreateDirectoryA((LPCSTR) temp, nil)) {
#else
          if (mkdir(temp, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
#endif
            return RET_ERRNO;
          }
        }
        while (*p && *p == '/') p++;
      }
      else p++;
    }
#ifdef CC_MSVC
    if (!CreateDirectoryA((LPCSTR) path, nil)) {
#else
    if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
#endif
      return RET_ERRNO;
    }
  }
  return RET_SUCCESS;
}
