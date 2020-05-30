/* log.c */
int logfile_init(char *filename);
int logfile_close(void);
int logfile(char *format, ...);
int logfile_error(char *format, ...);
int logfile_flush(void);
