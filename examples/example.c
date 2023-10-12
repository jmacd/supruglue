#include "external/ti-pru-support/include/pru_rpmsg.h"
#include "lib/cpx/cpx.h"
#include "lib/ui1203/ui1203.h"
#include <stddef.h>

void main() {
  UI1203_Run_Reader(NULL);
  Cpx_Join();
  Cpx_StaticStart(NULL, 10);
  pru_rpmsg_init(NULL, NULL, NULL, 0, 0);
}
