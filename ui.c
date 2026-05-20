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
        clear();

        int rows, cols;
        getmaxyx(stdscr, rows, cols);

        // Título
        attron(A_BOLD);
        mvprintw(0, (cols - 14) / 2, " GESTOR TAREAS ");
        attroff(A_BOLD);
        mvhline(1, 0, '-', cols);

        // Lista de tareas
        for (int i = 0; i < n; i++) {
            if (i == selected) attron(COLOR_PAIR(1) | A_BOLD);
            else if (tasks[i][1] == 'x') attron(COLOR_PAIR(2));
            else attron(COLOR_PAIR(3));

            mvprintw(i + 2, 2, "%d. %s", i + 1, tasks[i]);

            attroff(COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3) | A_BOLD);
        }

        // Ayuda
        mvhline(rows - 2, 0, '-', cols);
        mvprintw(rows - 1, 0, " a:añadir  d:borrar  enter:completar  q:salir");

        refresh();

        ch = getch();

        if (ch == 'q') break;

        else if ((ch == KEY_UP || ch == 'k') && selected > 0) selected--;
        else if ((ch == KEY_DOWN || ch == 'j') && selected < n - 1) selected++;

        else if (ch == '\n' && n > 0) {
            if (tasks[selected][1] == 'x')
                undone_task(selected + 1);
            else
                done_task(selected + 1);
        }

        else if (ch == 'd' && n > 0) {
            delete_task(selected + 1);
            if (selected >= n - 1 && selected > 0) selected--;
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
    }

    endwin();
}
