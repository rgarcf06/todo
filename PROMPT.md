Estoy desarrollando un gestor de tareas en C llamado "todo" en WSL Ubuntu.
Ruta del proyecto: ~/Programming/todo

Estructura:
- main.c, tasks.c, tasks.h, ui.c, ui.h, Makefile
- Binario instalado en /usr/local/bin/todo

Stack: C + ncurses + gcc + make + git

Formato de tasks.txt:
[PRIO] [ESTADO] descripción |dd/mm/yyyy
Donde PRIO = A/M/B, ESTADO = ' ' o 'x', posición del estado = índice 5

Funcionalidades implementadas:
- TUI con ncurses
- Añadir tarea con prioridad (a)lta/(m)edia/(b)aja
- Listar tareas ordenadas: pendientes primero, completadas después
- Completar/descompletar con Enter
- Borrar con 'd' + confirmación s/n
- Editar con 'e' (edición inline con cursor)
- Buscar con '/' (búsqueda en tiempo real)
- Fecha de creación visible a la derecha
- Contador de pendientes/completadas en el título
- Navegación con j/k y flechas
- Colores: rojo=alta, amarillo=media, cyan=baja, verde=completada, azul=seleccionada
- Acceso directo en escritorio Windows que lanza el programa

Repo GitHub: https://github.com/rgarcf06/todo

Continúa ayudándome a desarrollar el proyecto. Responde de forma breve y precisa,
sin saludos ni resúmenes. Usa código cuando sea más claro que texto.
