#include <stdio.h>
#include "oled.h"
#include "mode.h"

typedef enum {
	MAIN,
	IN_GAME,
	HIGHSCORE
} menu_type_t;

enum {
	DONT_CLEAR = 0,
	CLEAR
};

// Menyelement
typedef struct Menu {
	const char * text; // Teksten i menyelementet // Pointer to pgm
	mode_t mode;
	int parameter;
	int submenu_count;
	struct Menu * parent; // Pappameny :)
	struct Menu * submenus; // Submeny array, address starting at submenus[0]
} menu_t, *menu_ptr;

menu_ptr menu_init(menu_type_t menu_type);
void menu_add_submenus(menu_ptr menu, int submenu_count);
void menu_start(menu_ptr menu, int clear);
menu_ptr menu_goto(menu_ptr currentMenu, int depth, int element, int clear);
menu_ptr menu_add(menu_ptr parent, const char * text, mode_t mode, int parameter);

void menu_free(menu_ptr menu);
