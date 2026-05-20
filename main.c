#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tasks.h"

int main(int argc, char *argv[]) {
    if (argc < 2) { printf("Uso: ./todo [add|list|done|delete]\n"); return 1; }
    if (strcmp(argv[1], "add") == 0 && argc == 3)    add_task(argv[2]);
    else if (strcmp(argv[1], "list") == 0)            list_tasks();
    else if (strcmp(argv[1], "done") == 0 && argc==3) done_task(atoi(argv[2]));
    else if (strcmp(argv[1], "delete") == 0 && argc==3) delete_task(atoi(argv[2]));
    else printf("Comando no reconocido.\n");
    return 0;
}
