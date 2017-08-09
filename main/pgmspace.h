#pragma once

typedef void prog_void;
typedef char prog_char;
typedef unsigned char prog_uchar;
typedef char prog_int8_t;
typedef unsigned char prog_uint8_t;
typedef short prog_int16_t;
typedef unsigned short prog_uint16_t;
typedef long prog_int32_t;
typedef unsigned long prog_uint32_t;

#define SIZE_IRRELEVANT 0x7fffffff

#define PROGMEM
#define PGM_P         const char *
#define PGM_VOID_P    const void *
#define FPSTR(p)      ((const char *)(p))
#define PSTR(s)       (s)
#define _SFR_BYTE(n)  (n)

#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))
#define pgm_read_word(addr)   (*(const unsigned short *)(addr))
#define pgm_read_dword(addr)  (*(const unsigned long *)(addr))
#define pgm_read_float(addr)  (*(const float *)(addr))

#define pgm_read_byte_near(addr)  pgm_read_byte(addr)
#define pgm_read_word_near(addr)  pgm_read_word(addr)
#define pgm_read_dword_near(addr) pgm_read_dword(addr)
#define pgm_read_float_near(addr) pgm_read_float(addr)
#define pgm_read_byte_far(addr)   pgm_read_byte(addr)
#define pgm_read_word_far(addr)   pgm_read_word(addr)
#define pgm_read_dword_far(addr)  pgm_read_dword(addr)
#define pgm_read_float_far(addr)  pgm_read_float(addr)

#define memcmp_P      memcmp
#define memccpy_P     memccpy
#define memmem_P      memmem
#define memcpy_P      memcpy
#define strncpy_P     strncpy
#define strncat_P     strncat
#define strncmp_P     strncmp
#define strncasecmp_P strncasecmp
#define strnlen_P     strnlen
#define strstr_P      strstr
#define printf_P      printf
#define sprintf_P     sprintf
#define snprintf_P    snprintf
#define vsnprintf_P   vsnprintf

#define strlen_P(strP)  strnlen_P((strP), SIZE_IRRELEVANT)
#define strcasecmp_P(str1, str2P) strncasecmp_P((str1), (str2P), SIZE_IRRELEVANT)
#define strcmp_P(str1, str2P) strncmp_P((str1), (str2P), SIZE_IRRELEVANT)
#define strcat_P(dest, src) strncat_P((dest), (src), SIZE_IRRELEVANT)
#define strcpy_P(dest, src) strncpy_P((dest), (src), SIZE_IRRELEVANT)