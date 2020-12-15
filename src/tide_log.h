#ifndef TIDE_LOG_H
#define TIDE_LOG_H

#define TIDE_ERROR_PREFIX "[TIDE ERROR]"
#define TIDE_LOG_PREFIX "[TIDE]"

#define printlnf(msg, ...) printf(msg, ##__VA_ARGS__); printf("\n")

#endif // TIDE_LOG_H