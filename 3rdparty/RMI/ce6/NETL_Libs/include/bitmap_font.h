#ifndef __bitmap_font__
#define __bitmap_font__

#include <ctypes.h>

// Bitmap tiled fonts
typedef struct
{
    const char *name;
    const char **pixels;

    uint32 size;
    uint16 num_letters;
    uint16 letter_width;
    uint16 letter_height;
    unsigned bpp;
    unsigned max_char;

    // Private
    uint16 letter_size;
} BitmapFont;

BitmapFont *font_alloc(void);
void font_load(BitmapFont *font);
void font_printchar(BitmapFont *font, uint8 *buffer, unsigned buffer_bpp, unsigned buffer_width, uint8 c);
unsigned font_printstr(BitmapFont *font, uint8 *buffer, unsigned buffer_bpp, unsigned buffer_width, const char *str,
                       unsigned xpos, unsigned ypos);

#endif
