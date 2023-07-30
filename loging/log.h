#ifndef CONSOLE_CODE_EDITOR__LOG_H
#define CONSOLE_CODE_EDITOR__LOG_H

#ifndef LOGFILE_NAME
#define LOGFILE_NAME "log.txt"
#endif

typedef enum
{
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG
} LogLevel;

void init_logger(LogLevel logLevel, const char *log_filename);

void write_log(LogLevel logLevel, const char *fmessage, ...);

void write_vlog(LogLevel logLevel, const char *fmessage, va_list args);

#endif //CONSOLE_CODE_EDITOR__LOG_H
