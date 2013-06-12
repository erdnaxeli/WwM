#ifndef LOGGER_H
#define LOGGER_H

enum level {
    INFO,
    DEBUG,
    WARN,
    ERROR,
    CRITICAL
};

void set_logger_level(enum level lvl);

void logger(enum level lvl, char* format, ...);

#endif // LOGGER_H
