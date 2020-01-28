/**
 * @author Pradeep Mahato
 * @email pradeepmahato007@gmail.com
 * @create date 2020-01-26 12:35:52
 * @modify date 2020-01-26 12:35:52
 */

#include "../include/commonHeader.h"
#include "../include/generateTrace.h"
#include "../include/minePattern.h"

int main(int argc, char *argv[]) {

  if (!strcmp("tracegen", argv[argc - 1])) {
    _main_generateTrace();
  } else {
    _main_mineTrace();
  }

  return 0;
}