#ifndef UTILITY_H
#define UTILITY_H

char* itoa(int32_t value, char* result, int base);

void checksum32bit();

void assert_failed(uint8_t* file, uint32_t line);

#endif
