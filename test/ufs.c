#include <assert.h>
#include "ufs.h"

i32_t
main(void) {
  fs_file_t *file = new (fs_file_t, nil);

  fs_file_ctor(file, "LICENSE");
  if (fs_file_open(file, FS_OPEN_RO) == RET_SUCCESS) {
    i8_t buf[256];
    i64_t r;

    while (fs_file_read(file, buf, 256, &r) == RET_SUCCESS) {
      buf[r] = '\0';
      printf(buf);
    }
  }
  fs_file_dtor(file);
  return 0;
}
