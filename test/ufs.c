#include <assert.h>
#include "ufs.h"

i32_t
main(void) {
  path_t path;

  assert(fs_path_is_abs(fs_path(&path, "/")));
  return 0;
}
