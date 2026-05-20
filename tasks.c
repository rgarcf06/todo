#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tasks.h"

#define FILE_NAME "tasks.txt"

void add_task(const char *desc) {
    FILE *f = fopen(FILE_NAME, "a");
    fprintf(f, "[ ] %s\n", desc);
    fclose(f);
    printf("Tarea añadida.\n");
}

void list_tasks() {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) { printf("No hay tareas.\n"); return; }
    char line[256];
    int i = 1;
    while (fgets(line, sizeof(line), f))
        printf("%d. %s", i++, line);
    fclose(f);
}

void done_task(int id) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return;
    char lines[100][256];
    int n = 0;
    while (fgets(lines[n], sizeof(lines[n]), f)) n++;
    fclose(f);
    if (id < 1 || id > n) { printf("ID inválido.\n"); return; }
    lines[id-1][1] = 'x';
    f = fopen(FILE_NAME, "w");
    for (int i = 0; i < n; i++) fputs(lines[i], f);
    fclose(f);
}

void delete_task(int id) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return;
    char lines[100][256];
    int n = 0;
    while (fgets(lines[n], sizeof(lines[n]), f)) n++;
    fclose(f);
    if (id < 1 || id > n) { printf("ID inválido.\n"); return; }
    f = fopen(FILE_NAME, "w");
    for (int i = 0; i < n; i++)
        if (i != id-1) fputs(lines[i], f);
    fclose(f);
}
