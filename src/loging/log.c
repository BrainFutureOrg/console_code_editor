#include <stdio.h>
#include "log.h"
#include <stdarg.h>
#include <time.h>

const char *log_filename_global;
LogLevel logLevel_global;

void init_logger(LogLevel logLevel, const char *log_filename)
{
    log_filename_global = log_filename;
    logLevel_global = logLevel;
}

void print_time(FILE *fp)
{
    struct tm *tm_info;
    time_t now;
    time(&now);
    tm_info = gmtime(&now);
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    fputs(buffer, fp);
}

char *log_to_str(LogLevel logLevel)
{
    switch (logLevel)
    {
        case FATAL:
            return "FATAL";
        case ERROR:
            return "ERROR";
        case WARNING:
            return "WARNING";
        case INFO:
            return "INFO";
        case DEBUG:
            return "DEBUG";
    }
    return NULL;
}

/*
 * logLevel are: FATAL, ERROR, WARNING, INFO, DEBUG
 */
void write_log(LogLevel logLevel, const char *fmessage, ...)
{
    if (logLevel <= logLevel_global)
    {
        va_list args;
        va_start(args, fmessage);
        write_vlog(logLevel, fmessage, args);
        va_end(args);
    }
//    if (logLevel <= logLevel_global)
//    {
//        FILE *fp = fopen(log_filename_global, "a+");
//
//        print_time(fp);
//        fprintf(fp, " %s ", log_to_str(logLevel));
//
//        va_list args;
//        va_start(args, fmessage);
//        vfprintf(fp, fmessage, args);
//        va_end(args);
//
//        putc('\n', fp);
//
//        fclose(fp);
//    }
}

void write_vlog(LogLevel logLevel, const char *fmessage, va_list args)
{
    if (logLevel <= logLevel_global)
    {
        FILE *fp = fopen(log_filename_global, "a+");

        print_time(fp);
        fprintf(fp, " %s ", log_to_str(logLevel));

        vfprintf(fp, fmessage, args);

        putc('\n', fp);

        fclose(fp);
    }
}