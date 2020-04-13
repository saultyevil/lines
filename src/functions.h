/* lines.c */
void bound_bound_home_menu(void);
void print_bound_bound_lines(ScreenBuffer_t *sb, double wmin, double wmax);
/* log.c */
int Log_init(char *filename);
int Log_close(void);
int Log(char *format, ...);
int Log_silent(char *format, ...);
int Error(char *format, ...);
int Error_silent(char *format, ...);
int error_count(char *format);
int error_summary(char *message);
int error_summary_parallel(char *msg);
int Log_flush(void);
int Log_parallel(char *format, ...);
int Debug(char *format, ...);
void Exit(int error_code);
/* main.c */
int main_menu(int current_index);
void process_main_menu_choices(int choice);
int main(int argc, char *argv[]);
/* menu.c */
void clean_up_menu(MENU *menu, ITEM **items, int nitems, WINDOW *win_menu);
int control_menu(MENU *menu, int c);
int goto_menu(char *menu_message, char **menu_items, int nitems, int current_index);
/* read_atomic_data.c */
int get_atomic_data(char masterfile[]);
int index_lines(void);
int index_phot_top(void);
int index_inner_cross(void);
void indexx(int n, float arrin[], int indx[]);
int limit_lines(double freqmin, double freqmax);
int check_xsections(void);
double q21(struct lines *line_ptr, double t);
double q12(struct lines *line_ptr, double t);
double a21(struct lines *line_ptr);
double upsilon(int n_coll, double u0);
int fraction(double value, double array[], int npts, int *ival, double *f, int mode);
int linterp(double x, double xarray[], double yarray[], int xdim, double *y, int mode);
/* tools.c */
void parse_input(int argc, char *argv[], char *fname, double *wmin, double *wmax);
void get_element_name(int z, char *element);
void display_text_buffer(ScreenBuffer_t *sb, WINDOW *win, int y, int x);
void append_to_buffer(ScreenBuffer_t *sb, char *s, size_t len);
void append_separator(ScreenBuffer_t *sb);
/* ui.c */
void init_ncurses_screen(void);
void clean_ncurses_screen(void);
void write_banner_stdscr(void);
void create_sub_window(WINDOW **win);
void query_wavelength_range(double *wmin, double *wmax);
/* xsections.c */
void photoionization_home_menu(void);
void query_photoionization_cross_sections(ScreenBuffer_t *sb, double wmin, double wmax);
