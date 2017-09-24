#include <assert.h>
#include <ufs.h>
#include "ufs.h"

i32_t
main(void) {
  fs_path_t path = {0};
  fs_file_t file = {0};

  fs_path_cwd(&path);
  printf("%s\n", path.buf);
  fs_file_open(
    &file,
    "C:\\Users\\easypara\\work\\jay\\cmake-build-release\\vendor\\ufs\\build\\oops",
    FS_OPEN_CREAT | FS_OPEN_RO
  );
  fs_file_close(&file);
  fs_path_dtor(&path);
  return 0;
}
