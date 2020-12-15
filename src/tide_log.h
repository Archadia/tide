#ifndef TIDE_LOG_H
#define TIDE_LOG_H

#define TIDE_ERROR(msg, ...) printf("[TIDE ERROR] "); printf(msg, ##__VA_ARGS__); printf("\n");
#define TIDE_LOG(msg, ...) printf("[TIDE] "); printf(msg, ##__VA_ARGS__); printf("\n");

#endif // TIDE_LOG_H