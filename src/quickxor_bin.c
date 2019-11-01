#include <stdio.h>
#include "quickxor.h"

char* process_file(char** filename, char* hash);

int main(int argc, char** argv) {
  QX* pqx    = NULL;
  int status = 0;

  if (argc < 2) {
    printf("Usage: quickxorhash FILE[S]\n");
    status = 1;
  } else {
    pqx = QX_new();
    for (int i = 1; i < argc; ++i) {
      if (QX_readFile(pqx, argv[i])) {
        status = 1;
        printf("quickxorhash: %s: file not found!\n", argv[i]);
      } else {
        printf("%s  %s\n", QX_b64digest(pqx), argv[i]);
      }
    }
    QX_free(pqx);
  }

  return status;
}
