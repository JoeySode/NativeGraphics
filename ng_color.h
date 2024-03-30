
#ifndef NG_COLOR_H_

#define NG_COLOR_H_


#include <stdint.h>


typedef union
{
  struct
  {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
  };

  uint32_t rgba;
} NG_Color;


#define NG_BLACK  (NG_Color){ .r = 0x00, .g = 0x00, .b = 0x00, .a = 0x00 }
#define NG_WHITE  (NG_Color){ .r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF }

#define NG_RED    (NG_Color){ .r = 0xFF, .g = 0x00, .b = 0x00, .a = 0xFF }
#define NG_GREEN  (NG_Color){ .r = 0x00, .g = 0xFF, .b = 0x00, .a = 0xFF }
#define NG_BLUE   (NG_Color){ .r = 0x00, .g = 0x00, .b = 0xFF, .a = 0xFF }


#endif // NG_COLOR_H_
