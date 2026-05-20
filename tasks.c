#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tasks.h"

#define FILE_NAME "tasks.txt"

void add_task(const char *desc) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char fecha[20];
    strftime(fecha, sizeof(fecha), "%d/%m/%Y", tm);
    FILE *f = fopen(FILE_NAME, "a");
    fprintf(f, "[ ] %s |%s\n", desc, fecha);
    fclose(f);
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
    lines[id-1][5] = 'x';
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

void undone_task(int id) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return;
    char lines[100][256];
    int n = 0;
    while (fgets(lines[n], sizeof(lines[n]), f)) n++;
    fclose(f);
    if (id < 1 || id > n) return;
    lines[id-1][5] = ' ';
    f = fopen(FILE_NAME, "w");
    for (int i = 0; i < n; i++) fputs(lines[i], f);
    fclose(f);
}

void edit_task(int id, const char *desc) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return;
    char lines[100][256];
    int n = 0;
    while (fgets(lines[n], sizeof(lines[n]), f)) n++;
    fclose(f);
    if (id < 1 || id > n) return;
    char prio = lines[id-1][1];
    char estado = lines[id-1][4];
    snprintf(lines[id-1], 256, "[%c] [%c] %s\n", prio, estado, desc);
    f = fopen(FILE_NAME, "w");
    for (int i = 0; i < n; i++) fputs(lines[i], f);
    fclose(f);
}

void add_task_prio(const char *desc, char prio) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char fecha[20];
    strftime(fecha, sizeof(fecha), "%d/%m/%Y", tm);
    FILE *f = fopen(FILE_NAME, "a");
    fprintf(f, "[%c] [ ] %s |%s\n", prio, desc, fecha);
    fclose(f);
}
