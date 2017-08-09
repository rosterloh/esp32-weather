#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int atoi(const char *s);

long atol(const char* s);

double atof(const char* s);

char* itoa (int val, char *s, int radix);

char* ltoa (long val, char *s, int radix);

char* utoa (unsigned int val, char *s, int radix);

char* ultoa (unsigned long val, char *s, int radix);

char* dtostrf (double val, signed char width, unsigned char prec, char *s);

#ifdef __cplusplus
} // extern "C"
#endif