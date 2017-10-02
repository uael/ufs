#include "ufs.h"

i32_t
main(void) {
  fs_file_t file;

  file = FS_FD_DFT;
  if (fs_file_open(&file, "LICENSE", FS_OPEN_RO) == RET_SUCCESS) {
    char_t buf[256];
    i64_t r;

    while (fs_file_read(&file, buf, 256, &r) == RET_SUCCESS) {
      buf[r] = '\0';
      printf(buf);
    }
  }
  fs_file_close(&file);
  return 0;
}
