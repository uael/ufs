#include <assert.h>
#include <ufs.h>
#include "ufs.h"

i32_t
main(void) {
  fs_path_t path;

  fs_path_cwd(&path);
  printf("%s\n", path.buf);
  fs_path_dtor(&path);
  return 0;
}
