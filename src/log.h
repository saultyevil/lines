/* xlog.c */
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
int Log_set_mpi_rank(int rank, int n_mpi);
int Log_parallel(char *format, ...);
int Debug(char *format, ...);
void Exit(int error_code);
