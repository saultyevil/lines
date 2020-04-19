/* log.c */
int log_init(char *filename);
int log_close(void);
int logfile(char *format, ...);
int add_error_to_log(char *format, ...);
int log_flush(void);
