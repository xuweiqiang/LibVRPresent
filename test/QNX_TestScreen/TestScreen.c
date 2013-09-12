#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/keycodes.h>
#include <time.h>
#include <screen.h>

#include "hourglass.h"

const int barwidth = 32;

typedef short int16_t;


static int frame = 0;

static void draw_rgb565(int16_t *buffer,
						int width,
						int height,
						int stride,
						int red,
						int green,
						int blue,
						int *rects)
{
	int16_t bg;
	int16_t bar;
	int16_t grey;
	int tmp;
	int16_t *p;
	int i, j;

	stride >>= 1;
	bg = (0x1f << red);
	bar = (0x3f << green);
	grey = (0x14 << red) | (0x28 << green) | (0x14 << blue);

	p= buffer;




	if (frame <= 0) {
		for (i = 0, p = buffer; i < 160; i++, p += stride - 400) {
			printf("the p is %x\n",p);
			printf("the stride  is %x the height is %x the width is %x\n",stride,height,width);
			p += barwidth;

			for (j = 0; j < 400; j++, p++) {
				*p = bg;
			}
		}
	}

	tmp = frame % (width - barwidth);

	if (tmp != 0) {
		for (i = 0; i < height; i++) {
			buffer[i * stride + tmp - 1] = bg;
			buffer[i * stride + tmp + barwidth - 1] = bar;
		}
	} else {
		if (frame != 0) {
			for (i = 0; i < height; i++) {
				p = buffer + i * stride + width - barwidth - 1;
				for (j = 0; j < barwidth; j++, p++) {
					*p = bg;
				}
			}
		}

		for (i = 0; i < height; i++) {
			p = buffer + i * stride;
			for (j = 0; j < barwidth; j++, p++) {
				*p = bar;
			}
		}
	}

	if (tmp + barwidth - 1 >= 10 && tmp < 110) {
		for (i = 0; i < hourglass_height; i++) {
			p = buffer + (i + 10) * stride + 10;
			for (j = 0; j < hourglass_width; j++, p++) {
				if (hourglass_data[i * hourglass_width + j] != ' ') {
					*p = grey;
				}
			}
		}
	}

	if (tmp != 0) {
		rects[0] = tmp - 1;
		rects[2] = 1;
		rects[4] = tmp + barwidth - 1;
		rects[6] = 1;
	} else {
		rects[0] = 0;
		rects[2] = barwidth;
		if (frame != 0) {
			rects[4] = width - barwidth - 1;
			rects[6] = barwidth;
		} else {
			rects[4] = barwidth;
			rects[6] = width - barwidth;
		}
	}

	frame++;
}



int main(int argc, char **argv)
{
	const int exit_area_size = 20;

	struct {
		int src_x[2];
		int src_y[2];
		int src_width[2];
		int src_height[2];
		int dst_x[2];
		int dst_y[2];
		int dst_width[2];
		int dst_height[2];
		int none;
	} screen_blit_attr = {
		.src_x = { SCREEN_BLIT_SOURCE_X, 0 },
		.src_y = { SCREEN_BLIT_SOURCE_Y, 0 },
		.src_width = { SCREEN_BLIT_SOURCE_WIDTH, 400 },
		.src_height = { SCREEN_BLIT_SOURCE_HEIGHT, 160 },
		.dst_x = { SCREEN_BLIT_DESTINATION_X, 0 },
		.dst_y = { SCREEN_BLIT_DESTINATION_Y, 0 },
		.dst_width = { SCREEN_BLIT_DESTINATION_WIDTH, 400 },
		.dst_height = { SCREEN_BLIT_DESTINATION_HEIGHT, 160 },
		.none = SCREEN_BLIT_END
	};

	screen_context_t screen_ctx;
	screen_display_t screen_disp;
	screen_window_t screen_win;
	screen_pixmap_t screen_pix;
	screen_buffer_t screen_pbuf;
	screen_buffer_t screen_wbuf;
	screen_event_t screen_ev;
	//int size[2] = { 400, 160 };
	int pos[2] = { 0, 0 };
	int buf_size[2]={400, 160};
	int format = 0;
	int rotation = 0;
	int reorient = 0;
	int scale = 0;
	int val;
	const char *tok;
	int rval = EXIT_FAILURE;
	int rc;
	int i;
	int stride;
	void *pointer;
	int pause = 0;
	int rects[8]={0,0,400,160,0,0,400,160};
	int rect[8];

	rc = screen_create_context(&screen_ctx, SCREEN_WINDOW_MANAGER_CONTEXT);
	rc = screen_create_window(&screen_win, screen_ctx);
	val = SCREEN_USAGE_NATIVE | SCREEN_USAGE_ROTATION;
	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &val);

	if (format) {
		rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_FORMAT, &format);
	}
	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, buf_size);
	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SOURCE_SIZE, buf_size);
	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SIZE, buf_size);
	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_POSITION, pos);
	rc = screen_create_window_buffers(screen_win, 3);

	rc = screen_create_pixmap(&screen_pix, screen_ctx);
	val = SCREEN_USAGE_WRITE | SCREEN_USAGE_VIDEO;
	rc = screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_USAGE, &val);

	rc = screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_FORMAT, &format);
	rc = screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_FORMAT, &format);


	rc = screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_BUFFER_SIZE, buf_size);
	rc = screen_create_pixmap_buffer(screen_pix);
	rc = screen_get_pixmap_property_pv(screen_pix, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_pbuf);
	rc = screen_get_buffer_property_pv(screen_pbuf, SCREEN_PROPERTY_POINTER, &pointer);
	rc = screen_get_buffer_property_iv(screen_pbuf, SCREEN_PROPERTY_STRIDE, &stride);
	printf("the stride is %d\n",stride);

	while (1) {
			//draw_rgb565(pointer, 400, 160, stride, 11, 5, 0, rect);
			rc = screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_wbuf);
			rc = screen_blit(screen_ctx, screen_wbuf, screen_pbuf, (const int *)&screen_blit_attr);
			rc = screen_post_window(screen_win, screen_wbuf, 2, rects, 0);
		}


end:
	rval = EXIT_SUCCESS;

	screen_destroy_event(screen_ev);
fail4:
	screen_destroy_pixmap(screen_pix);
fail3:
	screen_destroy_window(screen_win);
fail2:
	screen_destroy_context(screen_ctx);
fail1:
	return rval;
}
