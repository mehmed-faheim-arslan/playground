#include <stdio.h>

#include "cli_args.h"

void print_cli_args(int argc, char **argv) {
  for(int i=0; i<argc; i++) {
    printf("%s\n", argv[i]);
  }
}
