
#ifndef __dsub_renderer_H__
#define __dsub_renderer_H__

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#include "dsub_decoder.h"

#ifdef __cplusplus
extern "C" {
#endif


/*
 * To create subtitle dsub_renderer you must implement following callback functions
 * and allocate space for 256 pseudo color entries.
 */


/*
 * Function that draws a horizontal line.
 * Parameters:
 *  priv: private data possibly meaningful to called function
 *  x, y: left ending point of a line
 *  length: line length
 *  thickness: line thickness (1 or 2), second line is drawn below the
 *             first line
 *  pseudo_color: references drawing color
 */
typedef void (*DrawHLine_CB)(void *priv, int x, int y, int length,
                                        int thickness, unsigned char pseudo_color);


/*
 * Draw a string.
 * Parameters:
 *  priv: private data
 *  x, y: top left corner?
 *  width, height: maximum string width/height in pixels
 *  str: decoding a string is left to this function
 *  length: string length
 *  fgcolor: foreground color of characters
 *  bgcolor: background color of characters whatever that means
 */
typedef void (*DrawString_CB)(void *priv, int x, int y, int width, int height, const unsigned short *str, int length, unsigned char fgcolor, unsigned char bgcolor);


/*
 * Maps pseudo color to actual color on target frame buffer.
 * Parameters:
 *  priv: private data possibly meaningful to called function
 *  pseudo_color: color to map
 *  Y, Cb, Cr: color components
 *  T: level of transparency (0=full transparency)
 */
typedef void (*ColorSet_CB)(void *priv, unsigned char pseudo_color, unsigned char Y, unsigned char Cb, unsigned char Cr, unsigned char T);


typedef struct {
    DrawHLine_CB   hline_callback;
    DrawString_CB string_callback;
    ColorSet_CB set_color_callback;
    void                      *private_data;
    const tDisplaySet *display_set;
    const tRegionCompositionSeg *region_composition;

    /* Used by dsub_renderer */
    int                       have_holes; /* non_modifying_colour_flag */
    unsigned char                   CLUT_2bit_to_8bit_map[4];
    unsigned char                   CLUT_4bit_to_8bit_map[16];
    unsigned int magic;
} tRenderingContext;


void RenderRegion(tRenderingContext *cntx);


/*
 * Create a rendering context.
 * Parameters:
 *  display_set: display set to render
 *  region_id: Region in page to render
 *  hline_callback: function to draw a horizontal line (may be NULL)
 *  string_callback: function to draw a string (may be NULL)
 *  set_color_callback: function to map pseudo color to display color (may be
 *                      NULL)
 *  priv: private data to callback functions
 */
tRenderingContext*
CreateRender(const tDisplaySet *display_set,
                                unsigned char region_id,
                                DrawHLine_CB hline_callback,
                                DrawString_CB string_callback,
                                ColorSet_CB set_color_callback,
                                void *priv);

void ReleaseRender(tRenderingContext *cntx);


/*
 * Returns background color for region. Value is either in range 0-255 for
 * pseudo color background, or something else if unspecified/transparent.
 */
int RegionBackGroundColorGet(const tRegionCompositionSeg *region);


#ifdef __cplusplus
}
#endif

#endif /* __dsub_renderer_H__ */
