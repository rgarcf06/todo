#include <ncurses.h>
#include <string.h>
#include "tasks.h"
#include "ui.h"

#define MAX_TASKS 100
#define MAX_LEN 256
#define FILE_NAME "tasks.txt"

static int load_tasks(char tasks[][MAX_LEN]) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) return 0;
    int n = 0;
    while (n < MAX_TASKS && fgets(tasks[n], MAX_LEN, f)) n++;
    fclose(f);
    return n;
}

void run_ui() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);   // seleccionado
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // completado
    init_pair(3, COLOR_WHITE, COLOR_BLACK);  // normal

    char tasks[MAX_TASKS][MAX_LEN];
    int n, selected = 0;
    int ch;

    while (1) {
        n = load_tasks(tasks);

        char sorted[MAX_TASKS][MAX_LEN];
        int sorted_idx[MAX_TASKS];
        int ns = 0;
        for (int i = 0; i < n; i++)
            if (tasks[i][1] != 'x') { strcpy(sorted[ns], tasks[i]); sorted_idx[ns++] = i + 1; }
        for (int i = 0; i < n; i++)
            if (tasks[i][1] == 'x') { strcpy(sorted[ns], tasks[i]); sorted_idx[ns++] = i + 1; }
        memcpy(tasks, sorted, sizeof(sorted));

        clear();

        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        // Título
        int pendientes = 0, completadas = 0;
        for (int i = 0; i < n; i++) {
            if (tasks[i][1] == 'x') completadas++;
            else pendientes++;
        }
        char titulo[64];
        snprintf(titulo, sizeof(titulo), " TAREAS: %d pendientes  %d hechas ", pendientes, completadas);
        attron(A_BOLD);
        mvprintw(0, (cols - strlen(titulo)) / 2, "%s", titulo);
        attroff(A_BOLD);
        mvhline(1, 0, '-', cols);

        // Lista de tareas
        for (int i = 0; i < n; i++) {
            if (i == selected) attron(COLOR_PAIR(1) | A_BOLD);
            else if (tasks[i][1] == 'x') attron(COLOR_PAIR(2));
            else attron(COLOR_PAIR(3));

            char display[MAX_LEN];
            strncpy(display, tasks[i], MAX_LEN);
            char *sep = strrchr(display, '|');
            char fecha_str[20] = "";
            if (sep) { *sep = 0; strncpy(fecha_str, sep + 1, 19); fecha_str[strcspn(fecha_str, "\n")] = 0; }
            mvprintw(i + 2, 2, "%d. %s", i + 1, display);
            mvprintw(i + 2, cols - 12, "%s", fecha_str);

            attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3) | A_BOLD);
        }

        // Ayuda
        mvhline(rows - 2, 0, '-', cols);
        mvprintw(rows - 1, 0, " a:añadir  e:editar  d:borrar  /:buscar  enter:completar  q:salir");

        refresh();

        ch = getch();

        if (ch == 'q') break;

        else if ((ch == KEY_UP || ch == 'k') && selected > 0) selected--;
        else if ((ch == KEY_DOWN || ch == 'j') && selected < n - 1) selected++;

        else if (ch == '\n' && n > 0) {
            if (tasks[selected][1] == 'x')
                undone_task(sorted_idx[selected]);
            else
                done_task(sorted_idx[selected]);
        }

        else if (ch == 'd' && n > 0) {
            mvprintw(n + 3, 2, "¿Borrar tarea? (s/n): ");
            echo();
            curs_set(1);
            int confirm = getch();
            noecho();
            curs_set(0);
            if (confirm == 's') {
                delete_task(sorted_idx[selected]);
                if (selected >= n - 1 && selected > 0) selected--;
            }
        }

        else if (ch == 'a') {
            echo();
            curs_set(1);
            char desc[MAX_LEN];
            mvprintw(n + 3, 2, "Nueva tarea: ");
            getnstr(desc, MAX_LEN - 1);
            noecho();
            curs_set(0);
            if (strlen(desc) > 0) add_task(desc);
        }

        else if (ch == 'e' && n > 0) {
            char desc[MAX_LEN];
            strncpy(desc, tasks[selected] + 4, MAX_LEN);
            desc[strcspn(desc, "\n")] = 0;

            int len = strlen(desc);
            int pos = len;

            while (1) {
                mvprintw(n + 3, 2, "Editar: %-50s", desc);
                move(n + 3, 10 + pos);
                curs_set(1);
                refresh();

                int c = getch();

                if (c == '\n') break;
                else if (c == 27) { len = 0; break; }  // ESC cancela
                else if ((c == KEY_BACKSPACE || c == 127) && pos > 0) {
                    memmove(&desc[pos-1], &desc[pos], len - pos + 1);
                    pos--; len--;
                }
                else if (c == KEY_LEFT && pos > 0) pos--;
                else if (c == KEY_RIGHT && pos < len) pos++;
                else if (c >= 32 && c < 127 && len < MAX_LEN - 1) {
                    memmove(&desc[pos+1], &desc[pos], len - pos + 1);
                    desc[pos++] = c;
                    len++;
                }
            }

            curs_set(0);
            if (len > 0)
                edit_task(sorted_idx[selected], desc);

        }

        else if (ch == '/' ) {
            char query[MAX_LEN];
            int qlen = 0;
            query[0] = 0;

            while (1) {
                mvprintw(n + 3, 2, "Buscar: %-40s", query);
                move(n + 3, 10 + qlen);
                curs_set(1);
                refresh();

                int c = getch();

                if (c == '\n') break;
                else if (c == 27) { qlen = 0; break; }
                else if ((c == KEY_BACKSPACE || c == 127) && qlen > 0)
                    query[--qlen] = 0;
                else if (c >= 32 && c < 127 && qlen < MAX_LEN - 1) {
                    query[qlen++] = c;
                    query[qlen] = 0;
                }

                for (int i = 0; i < n; i++) {
                    if (strstr(tasks[i], query)) {
                        selected = i;
                        break;
                    }
                }
            }

            curs_set(0);
        }
    }

    endwin();
}
