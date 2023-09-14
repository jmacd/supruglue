#include "lib/cpx/CorePartition.h"
#include "lib/ui1203/ui1203.h"
#include <stddef.h>

void main() {
  UI1203_Run_Reader(NULL);
  Cpx_Join();
  Cpx_StaticStart(NULL, 10);
}
