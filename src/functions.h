/* ../src/lines.c */
void bound_bound_home_menu(void);
void get_bound_bound_lines(ScreenBuffer_t *sb, double wmin, double wmax);
/* ../src/main.c */
int main_menu(int current_index);
void process_main_menu_choices(int choice);
int main(int argc, char *argv[]);
/* ../src/menu.c */
void clean_up_menu(MENU *menu, ITEM **items, int nitems, WINDOW *win_menu);
int control_menu(MENU *menu, int c);
int create_menu(char *menu_message, char **menu_items, int nitems, int current_index);
/* ../src/tools.c */
void get_element_name(int z, char *element);
/* ../src/ui.c */
void init_ncurses_screen(void);
void clean_ncurses_screen(void);
void write_banner_stdscr(void);
void bold_message(WINDOW *win, int y, int x, char *msg, ...);
void create_sub_window(WINDOW **win);
void query_atomic_data(void);
double get_wavelength(WINDOW *win, char *msg, int y, int x, int len);
void query_wavelength_range(double *wmin, double *wmax);
void display_text_buffer(ScreenBuffer_t *sb, WINDOW *win, int y, int x);
void append_to_buffer(ScreenBuffer_t *sb, char *s, size_t len);
void append_separator(ScreenBuffer_t *sb);
/* ../src/xsections.c */
void photoionization_home_menu(void);
void query_photoionization_cross_sections(ScreenBuffer_t *sb, double wmin, double wmax);
