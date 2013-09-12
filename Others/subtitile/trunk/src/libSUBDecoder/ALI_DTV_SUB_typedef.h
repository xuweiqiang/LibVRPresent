//=============================================================================
//   THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO Cidana (Shanghai).
//   ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//   ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//   COMPANY.
//
//   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//   KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//   IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//   PURPOSE.
//
//   Copyright (c) 2007 - 2008  Cidana (Shanghai) Co., Ltd.  All Rights Reserved.
//
//-----------------------------------------------------------------------------


#ifndef _ALI_DTV_SUB_TYPEDEF_H_
#define _ALI_DTV_SUB_TYPEDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


/*
 * Function that receives  ready display set and it's PTS.
 * Parameters:
 *  priv: private data possibly meaningful to called function
 *  display_set: the display set received
 *  pts: the pts of display_set
 *
 */
typedef void (*DisplaySetRecv_CB)(void *priv, void *display_set,UINT64 pts);

typedef struct{
    unsigned int region_id;
    /* Horizontal region composition position in page 0..719 */
    unsigned int region_horizontal_address;
    /* Vertical region composition position in page 0..575 */
    unsigned int region_vertical_address;
    unsigned int region_width;
    unsigned int region_height;
    int bkcolor; //background color for region. Value is either in range 0-255 for pseudo color background
} region_info;

/*
 * Draws a horizontal line.
 * Parameters:
 *  priv: private data 
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
 *  bgcolor: background color of characters 
 */
typedef void (*DrawString_CB)(void *priv, int x, int y, int width, int height, const unsigned short *str, int length, unsigned char fgcolor, unsigned char bgcolor);


/*
 * Maps pseudo color to actual color 
 * Parameters:
 *  priv: private data 
 *  pseudo_color: color to map
 *  Y, Cb, Cr: color components
 *  T: level of transparency (0=full transparency)
 */
typedef void (*ColorSet_CB)(void *priv, unsigned char pseudo_color, unsigned char Y, unsigned char Cb, unsigned char Cr, unsigned char T);


#endif

