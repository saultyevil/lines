/* bound.c */
void bound_bound_main_menu(void);
void get_bound_bound_lines(double wmin, double wmax);
/* buffer.c */
void clean_up_display(void);
void add_to_display(char *fmt, ...);
void add_separator_to_display(const int len);
void scroll_display(Window_t win);
void display(Window_t win, int scroll);
/* main.c */
int main(int argc, char *argv[]);
/* menu.c */
void clean_up_menu(MENU *menu, ITEM **items, int nitems);
int control_menu(MENU *menu, int c);
int main_menu(char *menu_message, const MenuItem_t *menu_items, int nitems, int current_index, int control_this_menu);
int create_menu(Window_t win, char *menu_message, const MenuItem_t *menu_items, int nitems, int current_index, int control_this_menu);
/* tools.c */
char *get_random_subtitle(void);
void get_element_name(int z, char *element);
int check_command_line(int argc, char **argv);
void error_atomix(char *fmt, ...);
void exit_atomix(int errno, char *fmt, ...);
void menu_exit_atomix(void);
char *trim_whitespaces(char *str);
int create_string(char *str, char *fmt, ...);
/* ui.c */
void initialise_ncurses_stdscr(void);
void cleanup_ncurses_stdscr(void);
void initialise_main_windows(void);
void draw_window_boundaries(void);
void bold_message(WINDOW *win, int y, int x, char *fmt, ...);
void update_status_bar(char *fmt, ...);
/* photoionization.c */
void bound_free_main_menu(void);
void get_photoionization_cross_sections(double wmin, double wmax);
/* read_atomic_data.c */
int fraction(double value, double array[], int npts, int *ival, double *f, int mode);
int linterp(double x, double xarray[], double yarray[], int xdim, double *y, int mode);
int index_phot_top(void);
int index_inner_cross(void);
void indexx(int n, float arrin[], int indx[]);
int limit_lines(double freqmin, double freqmax);
int check_xsections(void);
double a21(struct lines *line_ptr);
double upsilon(int n_coll, double u0);
int index_lines(void);
int get_atomic_data(char *masterfile);
/* query.c */
void clean_up_form(FORM *form, FIELD **fields, int nfields);
int control_form(FORM *form, int ch, int exit_index);
int query_user(Window_t w, Query_t *q, int nfields, char *title_message);
void init_single_question_form(Query_t *q, char *label, char *answer);
void init_two_question_form(Query_t *q, char *label1, char *label2, char *answer1, char *answer2);
int query_wavelength_range(double *wmin, double *wmax);
int query_atomic_number(int *z);
int query_ion_input(int nion_or_z, int *z, int *istate, int *nion);
void query_atomic_data(void);
/* elements.c */
void elements_main_menu(void);
void get_single_element(void);
void get_elements(void);
/* ions.c */
void ions_main_menu(void);
void get_ion_z(void);
void get_ion_nion(void);
void get_all_ions(void);
void get_ion_element(void);
/* levels.c */
void levels_main_menu(void);
