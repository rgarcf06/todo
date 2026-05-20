#ifndef TASKS_H
#define TASKS_H

void add_task(const char *desc);
void list_tasks();
void done_task(int id);
void undone_task(int id);
void delete_task(int id);
void edit_task(int id, const char *desc);


#endif
