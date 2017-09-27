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

FORCEINLINE ret_t
fs_absolute(i8_t const *path, i8_t *out) {
#ifdef OS_WIN
  if (GetFullPathName((LPCSTR) path, FS_PATH_MAX, (LPSTR) out, nil) == 0) {
    return RET_ERRNO;
  }
#else
  i8_t *ptr;

  if ((ptr = realpath(path, out)) == nil) {
    return RET_ERRNO;
  }
  (void) ptr;
#endif
  return RET_SUCCESS;
}

FORCEINLINE bool_t
fs_exists(i8_t const *path) {
#ifdef OS_WIN
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

FORCEINLINE u16_t
fs_cwd(i8_t *path, u16_t n) {
#ifdef OS_WIN
  return (i16_t) GetCurrentDirectory((DWORD) n, (LPTSTR) path);
#else
  char *ret;

  if ((ret = getcwd(path, n)) == nil)
    return 0;
  return (i16_t) strlen(ret);
#endif
}

FORCEINLINE ret_t
fs_mkdir(i8_t const *path) {
#ifdef OS_WIN
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
#ifdef OS_WIN
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
#ifdef OS_WIN
    if (!CreateDirectoryA((LPCSTR) path, nil)) {
#else
    if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
#endif
      return RET_ERRNO;
    }
  }
  return RET_SUCCESS;
}

FORCEINLINE bool_t
fs_rm(i8_t const *path) {
#ifdef OS_WIN
  if (UNLIKELY(DeleteFile((LPCSTR) path) == 0)) {
#else
  if (UNLIKELY(unlink(path) != 0)) {
#endif
    return false;
  }
  return true;
}

FORCEINLINE bool_t
fs_touch(i8_t const *path) {
#ifdef OS_WIN
  HANDLE *fd;

  fd = CreateFile(
    (LPCSTR) path, GENERIC_READ, FILE_SHARE_READ, nil,
    CREATE_ALWAYS | CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nil
  );
  if (fd == INVALID_HANDLE_VALUE) {
    if (fs_mkdir(path) > 0) {
      return false;
    }
    fd = CreateFile(
      (LPCSTR) path, GENERIC_READ, FILE_SHARE_READ, nil,
      CREATE_ALWAYS | CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nil
    );
    if (fd == INVALID_HANDLE_VALUE) {
      return false;
    }
    return false;
  }
  CloseHandle(fd);
#else
  i32_t fd;

  fd = open(path, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) {
    if (fs_mkdir(path) > 0) {
      return false;
    }
    fd = open(path, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0) {
      return false;
    }
    return false;
  }
  close(fd);
#endif
  return true;
}
