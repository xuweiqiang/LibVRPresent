/*
 * $QNXLicenseC:
 * Copyright 2010, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable
 * license fees to QNX Software Systems before you may reproduce,
 * modify or distribute this software, or any work that includes
 * all or part of this software.   Free development licenses are
 * available for evaluation and non-commercial purposes.  For more
 * information visit http://licensing.qnx.com or email
 * licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review
 * this entire file for other proprietary rights or license notices,
 * as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#ifndef _SCREEN_SCREEN_H_
#define _SCREEN_SCREEN_H_

#include <process.h>
#include <stdio.h>
#include <stdint.h>

__BEGIN_DECLS

struct _screen_context;
typedef struct _screen_context *screen_context_t;

struct _screen_display;
typedef struct _screen_display *screen_display_t;

struct _screen_window;
typedef struct _screen_window *screen_window_t;

struct _screen_pixmap;
typedef struct _screen_pixmap *screen_pixmap_t;

struct _screen_buffer;
typedef struct _screen_buffer *screen_buffer_t;

struct _screen_event;
typedef struct _screen_event *screen_event_t;

enum {
	SCREEN_APPLICATION_CONTEXT             = 0,
	SCREEN_WINDOW_MANAGER_CONTEXT          = 1,
	SCREEN_INPUT_PROVIDER_CONTEXT          = 2,
};

enum {
	SCREEN_APPLICATION_WINDOW              = 0,
	SCREEN_CHILD_WINDOW                    = 1,
	SCREEN_EMBEDDED_WINDOW                 = 2,
	SCREEN_ROOT_WINDOW                     = 3,
};

enum {
	/**
	 ** buffer properties
	 **/

	SCREEN_PROPERTY_BUFFER_SIZE            = 5,
	SCREEN_PROPERTY_EGL_HANDLE             = 12,
	SCREEN_PROPERTY_FORMAT                 = 14,
	SCREEN_PROPERTY_INTERLACED             = 22,
	SCREEN_PROPERTY_PHYSICALLY_CONTIGUOUS  = 32,
	SCREEN_PROPERTY_PLANAR_OFFSETS         = 33,
	SCREEN_PROPERTY_POINTER                = 34,
	SCREEN_PROPERTY_PROTECTED              = 36,
	SCREEN_PROPERTY_STRIDE                 = 44,
	SCREEN_PROPERTY_PHYSICAL_ADDRESS       = 55,

	/**
	 ** context properties
	 **/

	SCREEN_PROPERTY_DISPLAY_COUNT          = 59,
	SCREEN_PROPERTY_DISPLAYS               = 60,
	SCREEN_PROPERTY_IDLE_STATE             = 81,
	SCREEN_PROPERTY_IDLE_TIMEOUT           = 83,
	SCREEN_PROPERTY_KEYBOARD_FOCUS         = 84,
	SCREEN_PROPERTY_MTOUCH_FOCUS           = 85,
	SCREEN_PROPERTY_POINTER_FOCUS          = 86,

	/**
	 ** display properties
	 **/

	SCREEN_PROPERTY_GAMMA                  = 2,
	SCREEN_PROPERTY_ID_STRING              = 20,
	SCREEN_PROPERTY_ROTATION               = 38,
	SCREEN_PROPERTY_SIZE                   = 40,
	SCREEN_PROPERTY_TRANSPARENCY           = 46,
	SCREEN_PROPERTY_TYPE                   = 47,
	SCREEN_PROPERTY_MIRROR_MODE            = 58,
	SCREEN_PROPERTY_ATTACHED               = 64,
	SCREEN_PROPERTY_DETACHABLE             = 65,
	SCREEN_PROPERTY_NATIVE_RESOLUTION      = 66,
	SCREEN_PROPERTY_PROTECTION_ENABLE      = 67,
	SCREEN_PROPERTY_PHYSICAL_SIZE          = 69,
	SCREEN_PROPERTY_FORMAT_COUNT           = 70,
	SCREEN_PROPERTY_FORMATS                = 71,
	SCREEN_PROPERTY_FILL_PORT_AREA         = 72,
	SCREEN_PROPERTY_KEEP_AWAKES            = 82,
	SCREEN_PROPERTY_ID                     = 87,
	SCREEN_PROPERTY_POWER_MODE             = 88,
	SCREEN_PROPERTY_MODE_COUNT             = 89,
	SCREEN_PROPERTY_MODE                   = 90,
	SCREEN_PROPERTY_DEINTERLACE            = 94,

	/**
	 ** event properties
	 **/

	SCREEN_PROPERTY_BUTTONS                = 6,
	SCREEN_PROPERTY_DEVICE_INDEX           = 10,
	SCREEN_PROPERTY_DISPLAY                = 11,
	SCREEN_PROPERTY_GROUP                  = 18,
	SCREEN_PROPERTY_INPUT_VALUE            = 21,
	SCREEN_PROPERTY_JOG_COUNT              = 23,
	SCREEN_PROPERTY_KEY_CAP                = 24,
	SCREEN_PROPERTY_KEY_FLAGS              = 25,
	SCREEN_PROPERTY_KEY_MODIFIERS          = 26,
	SCREEN_PROPERTY_KEY_SCAN               = 27,
	SCREEN_PROPERTY_KEY_SYM                = 28,
	SCREEN_PROPERTY_NAME                   = 30,
	SCREEN_PROPERTY_POSITION               = 35,
	/* SCREEN_PROPERTY_SIZE                = 40, */
	SCREEN_PROPERTY_SOURCE_POSITION        = 41,
	SCREEN_PROPERTY_SOURCE_SIZE            = 42,
	/* SCREEN_PROPERTY_TYPE                = 47, */
	SCREEN_PROPERTY_USER_DATA              = 49,
	SCREEN_PROPERTY_WINDOW                 = 52,
	/* SCREEN_PROPERTY_MIRROR_MODE         = 58, */
	SCREEN_PROPERTY_EFFECT                 = 62,
	/* SCREEN_PROPERTY_ATTACHED            = 64, */
	/* SCREEN_PROPERTY_PROTECTION_ENABLE   = 67, */
	SCREEN_PROPERTY_TOUCH_ID               = 73,
	SCREEN_PROPERTY_TOUCH_ORIENTATION      = 76,
	SCREEN_PROPERTY_TOUCH_PRESSURE         = 77,
	SCREEN_PROPERTY_TIMESTAMP              = 78,
	SCREEN_PROPERTY_SEQUENCE_ID            = 79,
	/* SCREEN_PROPERTY_IDLE_STATE          = 81, */
	/* SCREEN_PROPERTY_MODE                = 90, */

	/**
	 ** pixmap properties
	 **/

	SCREEN_PROPERTY_ALPHA_MODE             = 1,
	/* SCREEN_PROPERTY_BUFFER_SIZE         = 5, */
	SCREEN_PROPERTY_COLOR_SPACE            = 8,
	/* SCREEN_PROPERTY_FORMAT              = 14, */
	/* SCREEN_PROPERTY_ID_STRING           = 20, */
	SCREEN_PROPERTY_RENDER_BUFFERS         = 37,
	SCREEN_PROPERTY_USAGE                  = 48,

	/**
	 ** window properties
	 **/

	/* SCREEN_PROPERTY_ALPHA_MODE          = 1, */
	SCREEN_PROPERTY_BRIGHTNESS             = 3,
	SCREEN_PROPERTY_BUFFER_COUNT           = 4,
	/* SCREEN_PROPERTY_BUFFER_SIZE         = 5, */
	SCREEN_PROPERTY_CLASS                  = 7,
	/* SCREEN_PROPERTY_COLOR_SPACE         = 8, */
	SCREEN_PROPERTY_CONTRAST               = 9,
	/* SCREEN_PROPERTY_DISPLAY             = 11, */
	SCREEN_PROPERTY_FLIP                   = 13,
	/* SCREEN_PROPERTY_FORMAT              = 14, */
	SCREEN_PROPERTY_FRONT_BUFFER           = 15,
	SCREEN_PROPERTY_GLOBAL_ALPHA           = 16,
	SCREEN_PROPERTY_PIPELINE               = 17,
	/* SCREEN_PROPERTY_GROUP               = 18, */
	SCREEN_PROPERTY_HUE                    = 19,
	/* SCREEN_PROPERTY_ID_STRING           = 20, */
	SCREEN_PROPERTY_MIRROR                 = 29,
	SCREEN_PROPERTY_OWNER_PID              = 31,
	/* SCREEN_PROPERTY_POSITION            = 35, */
	/* SCREEN_PROPERTY_RENDER_BUFFERS      = 37, */
	/* SCREEN_PROPERTY_ROTATION            = 38, */
	SCREEN_PROPERTY_SATURATION             = 39,
	/* SCREEN_PROPERTY_SIZE                = 40, */
	/* SCREEN_PROPERTY_SOURCE_POSITION     = 41, */
	/* SCREEN_PROPERTY_SOURCE_SIZE         = 42, */
	SCREEN_PROPERTY_STATIC                 = 43,
	SCREEN_PROPERTY_SWAP_INTERVAL          = 45,
	/* SCREEN_PROPERTY_TRANSPARENCY        = 46, */
	/* SCREEN_PROPERTY_TYPE                = 47, */
	/* SCREEN_PROPERTY_USAGE               = 48, */
	SCREEN_PROPERTY_USER_HANDLE            = 50,
	SCREEN_PROPERTY_VISIBLE                = 51,
	SCREEN_PROPERTY_WRAP_POSITION          = 53,
	SCREEN_PROPERTY_ZORDER                 = 54,
	SCREEN_PROPERTY_SCALE_QUALITY          = 56,
	SCREEN_PROPERTY_SENSITIVITY            = 57,
	SCREEN_PROPERTY_CBABC_MODE             = 61,
	SCREEN_PROPERTY_FLOATING               = 63,
	SCREEN_PROPERTY_VIEWPORT_POSITION      = 74,
	SCREEN_PROPERTY_VIEWPORT_SIZE          = 75,
	SCREEN_PROPERTY_IDLE_MODE              = 80,
	SCREEN_PROPERTY_CLIP_POSITION          = 91,
	SCREEN_PROPERTY_CLIP_SIZE              = 92,
	SCREEN_PROPERTY_COLOR                  = 93,
};

enum {
	SCREEN_MODE_PREFERRED                  = 0x1
};

#define SCREEN_MODE_PREFERRED_INDEX (-1)

typedef struct _screen_mode {
	_Uint32t width;
	_Uint32t height;
	_Uint32t refresh;
	_Uint32t interlaced;
	_Uint32t aspect_ratio[2];
	_Uint32t flags;
	_Uint32t index;
	_Uint32t reserved[6];
} screen_display_mode_t;

enum {
	 SCREEN_POWER_MODE_OFF                 = 0x7680,
	 SCREEN_POWER_MODE_SUSPEND             = 0x7681,
	 SCREEN_POWER_MODE_LIMITED_USE         = 0x7682,
	 SCREEN_POWER_MODE_ON                  = 0x7683,
};

enum {
	SCREEN_IDLE_MODE_NORMAL                = 0,
	SCREEN_IDLE_MODE_KEEP_AWAKE            = 1,
};

enum {
	SCREEN_DISPLAY_TYPE_INTERNAL           = 0x7660,
	SCREEN_DISPLAY_TYPE_COMPOSITE          = 0x7661,
	SCREEN_DISPLAY_TYPE_SVIDEO             = 0x7662,
	SCREEN_DISPLAY_TYPE_COMPONENT_YPbPr    = 0x7663,
	SCREEN_DISPLAY_TYPE_COMPONENT_RGB      = 0x7664,
	SCREEN_DISPLAY_TYPE_COMPONENT_RGBHV    = 0x7665,
	SCREEN_DISPLAY_TYPE_DVI                = 0x7666,
	SCREEN_DISPLAY_TYPE_HDMI               = 0x7667,
	SCREEN_DISPLAY_TYPE_DISPLAYPORT        = 0x7668,
	SCREEN_DISPLAY_TYPE_OTHER              = 0x7669,
};

enum {
	SCREEN_MIRROR_DISABLED                 = 0,
	SCREEN_MIRROR_NORMAL                   = 1,
	SCREEN_MIRROR_STRETCH                  = 2,
	SCREEN_MIRROR_ZOOM                     = 3,
	SCREEN_MIRROR_FILL                     = 4,
};

/*
 * Available alpha modes. The default value is non pre-multiplied. In this
 * case, src blending is done using the equation
 *
 *   c(r,g,b) = s(r,g,b) * s(a) + d(r,g,b) * (1 - s(a)).
 *
 * Pre-multiplied alpha content is src blended using the equation
 *
 *   c(r,g,b) = s(r,g,b) + d(r,g,b) * (1 - s(a)).
 */
enum {
	SCREEN_NON_PRE_MULTIPLIED_ALPHA        = 0,
	SCREEN_PRE_MULTIPLIED_ALPHA            = 1,
};

/*
 * Pixel formats supported. Formats with an alpha channel will have source
 * alpha enabled automatically. Applications that want composition manager to
 * disregard the alpha channel can choose a pixel format with an X.
 */
enum {
	SCREEN_FORMAT_BYTE                     = 1,
	SCREEN_FORMAT_RGBA4444                 = 2,
	SCREEN_FORMAT_RGBX4444                 = 3,
	SCREEN_FORMAT_RGBA5551                 = 4,
	SCREEN_FORMAT_RGBX5551                 = 5,
	SCREEN_FORMAT_RGB565                   = 6,
	SCREEN_FORMAT_RGB888                   = 7,
	SCREEN_FORMAT_RGBA8888                 = 8,
	SCREEN_FORMAT_RGBX8888                 = 9,
	SCREEN_FORMAT_YVU9                     = 10,
	SCREEN_FORMAT_YUV420                   = 11,
	SCREEN_FORMAT_NV12                     = 12,
	SCREEN_FORMAT_YV12                     = 13,
	SCREEN_FORMAT_UYVY                     = 14,
	SCREEN_FORMAT_YUY2                     = 15,
	SCREEN_FORMAT_YVYU                     = 16,
	SCREEN_FORMAT_V422                     = 17,
	SCREEN_FORMAT_AYUV                     = 18,
	SCREEN_FORMAT_NFORMATS,
};

/*
 * Usage flags are used when allocating buffers. Depending on the usage,
 * different constraints like width, height, stride granularity or special
 * alignment must be observed. The usage is also a valuable hint in determining
 * the amount of caching that can be set on a particular buffer. The display
 * usage flag is reserved and cannot be used by applications.
 */
enum {
	SCREEN_USAGE_DISPLAY                   = (1 << 0),
	SCREEN_USAGE_READ                      = (1 << 1),
	SCREEN_USAGE_WRITE                     = (1 << 2),
	SCREEN_USAGE_NATIVE                    = (1 << 3),
	SCREEN_USAGE_OPENGL_ES1                = (1 << 4),
	SCREEN_USAGE_OPENGL_ES2                = (1 << 5),
	SCREEN_USAGE_OPENVG                    = (1 << 6),
	SCREEN_USAGE_VIDEO                     = (1 << 7),
	SCREEN_USAGE_CAPTURE                   = (1 << 8),
	SCREEN_USAGE_ROTATION                  = (1 << 9),
	SCREEN_USAGE_OVERLAY                   = (1 << 10),
	SCREEN_USAGE_DEINTERLACE               = (1 << 11),
};

enum {
	SCREEN_TRANSPARENCY_SOURCE             = 0,
	SCREEN_TRANSPARENCY_TEST               = 1,
	SCREEN_TRANSPARENCY_SOURCE_COLOR       = 2,
	SCREEN_TRANSPARENCY_SOURCE_OVER        = 3,
	SCREEN_TRANSPARENCY_NONE               = 4,
	SCREEN_TRANSPARENCY_DISCARD            = 5,
};

enum {
	SCREEN_SENSITIVITY_TEST                = 0,
	SCREEN_SENSITIVITY_ALWAYS              = 1,
	SCREEN_SENSITIVITY_NEVER               = 2,
	SCREEN_SENSITIVITY_NO_FOCUS            = 3,
	SCREEN_SENSITIVITY_FULLSCREEN          = 4,
};

enum {
	SCREEN_QUALITY_NORMAL                  = 0,
	SCREEN_QUALITY_FASTEST                 = 1,
	SCREEN_QUALITY_NICEST                  = 2,
};

enum {
	SCREEN_CBABC_MODE_NONE                 = 0x7671,
	SCREEN_CBABC_MODE_VIDEO                = 0x7672,
	SCREEN_CBABC_MODE_UI                   = 0x7673,
	SCREEN_CBABC_MODE_PHOTO                = 0x7674,
};

enum {
	SCREEN_EVENT_NONE                      = 0,
	SCREEN_EVENT_CREATE                    = 1,
	SCREEN_EVENT_PROPERTY                  = 2,
	SCREEN_EVENT_CLOSE                     = 3,
	SCREEN_EVENT_INPUT                     = 4,
	SCREEN_EVENT_JOG                       = 5,
	SCREEN_EVENT_POINTER                   = 6,
	SCREEN_EVENT_KEYBOARD                  = 7,
	SCREEN_EVENT_USER                      = 8,
	SCREEN_EVENT_POST                      = 9,
	SCREEN_EVENT_EFFECT_COMPLETE           = 10,
	SCREEN_EVENT_DISPLAY                   = 11,
	SCREEN_EVENT_IDLE                      = 12,

	/*
	 * These are set to match the INPUT_EVENT_* enums to avoid having to remap
	 * the event types when they come in from libinputevents.
	 */
	SCREEN_EVENT_MTOUCH_TOUCH              = 100,
	SCREEN_EVENT_MTOUCH_MOVE               = 101,
	SCREEN_EVENT_MTOUCH_RELEASE            = 102,
};

enum {
	SCREEN_BLIT_END                        = 0,
	SCREEN_BLIT_SOURCE_X                   = 1,
	SCREEN_BLIT_SOURCE_Y                   = 2,
	SCREEN_BLIT_SOURCE_WIDTH               = 3,
	SCREEN_BLIT_SOURCE_HEIGHT              = 4,
	SCREEN_BLIT_DESTINATION_X              = 5,
	SCREEN_BLIT_DESTINATION_Y              = 6,
	SCREEN_BLIT_DESTINATION_WIDTH          = 7,
	SCREEN_BLIT_DESTINATION_HEIGHT         = 8,
	SCREEN_BLIT_GLOBAL_ALPHA               = 9,
	SCREEN_BLIT_TRANSPARENCY               = 10,
	SCREEN_BLIT_SCALE_QUALITY              = 11,
};

enum {
	SCREEN_WAIT_IDLE                       = (1 << 0),
	SCREEN_PROTECTED                       = (1 << 1),
	SCREEN_DONT_FLUSH                      = (1 << 2), /* internal use only */
	SCREEN_POST_RESUME                     = (1 << 3), /* internal use only */
	SCREEN_DEINTERLACE_TOP_FIELD_FIRST     = (1 << 4),
	SCREEN_DEINTERLACE_BOTTOM_FIELD_FIRST  = (1 << 5),
};

enum {
	SCREEN_REQUEST_PACKET                  = 0,
	SCREEN_BLIT_PACKET                     = 1,
	SCREEN_INPUT_PACKET                    = 2,
};

/*
 * Blits
 */
int screen_blit(screen_context_t ctx, screen_buffer_t dst, screen_buffer_t src, const int *attribs);
int screen_flush_blits(screen_context_t ctx, int flags);

/*
 * Buffers
 */
int screen_create_buffer(screen_buffer_t *pbuf);
int screen_destroy_buffer(screen_buffer_t buf);
int screen_get_buffer_property_cv(screen_buffer_t buf, int pname, int len, char *param);
int screen_get_buffer_property_iv(screen_buffer_t buf, int pname, int *param);
int screen_get_buffer_property_llv(screen_buffer_t buf, int pname, long long *param);
int screen_get_buffer_property_pv(screen_buffer_t buf, int pname, void **param);
int screen_set_buffer_property_cv(screen_buffer_t buf, int pname, int len, const char *param);
int screen_set_buffer_property_iv(screen_buffer_t buf, int pname, const int *param);
int screen_set_buffer_property_llv(screen_buffer_t buf, int pname, const long long *param);
int screen_set_buffer_property_pv(screen_buffer_t buf, int pname, void **param);

/*
 * Contexts
 */
int screen_create_context(screen_context_t *pctx, int flags);
int screen_destroy_context(screen_context_t ctx);
int screen_flush_context(screen_context_t ctx, int flags);
int screen_get_context_property_cv(screen_context_t ctx, int pname, int len, char *param);
int screen_get_context_property_iv(screen_context_t ctx, int pname, int *param);
int screen_get_context_property_llv(screen_context_t ctx, int pname, long long *param);
int screen_get_context_property_pv(screen_context_t ctx, int pname, void **param);
int screen_set_context_property_cv(screen_context_t ctx, int pname, int len, const char *param);
int screen_set_context_property_iv(screen_context_t ctx, int pname, const int *param);
int screen_set_context_property_llv(screen_context_t ctx, int pname, const long long *param);
int screen_set_context_property_pv(screen_context_t ctx, int pname, void **param);

/*
 * Displays
 */
int screen_get_display_property_cv(screen_display_t disp, int pname, int len, char *param);
int screen_get_display_property_iv(screen_display_t disp, int pname, int *param);
int screen_get_display_property_llv(screen_display_t disp, int pname, long long *param);
int screen_get_display_property_pv(screen_display_t disp, int pname, void **param);
int screen_set_display_property_cv(screen_display_t disp, int pname, int len, const char *param);
int screen_set_display_property_iv(screen_display_t disp, int pname, const int *param);
int screen_set_display_property_llv(screen_display_t disp, int pname, const long long *param);
int screen_set_display_property_pv(screen_display_t disp, int pname, void **param);
int screen_get_display_modes(screen_display_t display, int max, screen_display_mode_t *param);
int screen_wait_vsync(screen_display_t display);

/*
 * Effects
 */
int screen_prepare_effect(screen_window_t win, int effect);
int screen_set_effect_property_iv(screen_window_t win, int pname, const int *param);
int screen_set_effect_property_fv(screen_window_t win, int pname, const float *param);
int screen_start_effect(screen_window_t win, float duration, int notify);
int screen_stop_effect(screen_window_t win, float duration, int notify);

/*
 * Events
 */
int screen_create_event(screen_event_t *pev);
int screen_destroy_event(screen_event_t ev);
int screen_get_event(screen_context_t ctx, screen_event_t ev, uint64_t timeout);
int screen_get_event_property_cv(screen_event_t ev, int pname, int len, char *param);
int screen_get_event_property_iv(screen_event_t ev, int pname, int *param);
int screen_get_event_property_llv(screen_event_t ev, int pname, long long *param);
int screen_get_event_property_pv(screen_event_t ev, int pname, void **param);
int screen_inject_event(screen_display_t disp, screen_event_t ev);
int screen_send_event(screen_context_t ctx, screen_event_t ev, pid_t pid);
int screen_set_event_property_cv(screen_event_t ev, int pname, int len, const char *param);
int screen_set_event_property_iv(screen_event_t ev, int pname, const int *param);
int screen_set_event_property_llv(screen_event_t ev, int pname, const long long *param);
int screen_set_event_property_pv(screen_event_t ev, int pname, void **param);

/*
 * Pixmaps
 */
int screen_attach_pixmap_buffer(screen_pixmap_t pix, screen_buffer_t buf);
int screen_create_pixmap(screen_pixmap_t *ppix, screen_context_t ctx);
int screen_create_pixmap_buffer(screen_pixmap_t pix);
int screen_destroy_pixmap(screen_pixmap_t pix);
int screen_destroy_pixmap_buffer(screen_pixmap_t pix);
int screen_get_pixmap_property_cv(screen_pixmap_t pix, int pname, int len, char *param);
int screen_get_pixmap_property_iv(screen_pixmap_t pix, int pname, int *param);
int screen_get_pixmap_property_llv(screen_pixmap_t pix, int pname, long long *param);
int screen_get_pixmap_property_pv(screen_pixmap_t pix, int pname, void **param);
int screen_set_pixmap_property_cv(screen_pixmap_t pix, int pname, int len, const char *param);
int screen_set_pixmap_property_iv(screen_pixmap_t pix, int pname, const int *param);
int screen_set_pixmap_property_llv(screen_pixmap_t pix, int pname, const long long *param);
int screen_set_pixmap_property_pv(screen_pixmap_t pix, int pname, void **param);

/*
 * Windows
 */
int screen_attach_window_buffers(screen_window_t win, int count, screen_buffer_t *buf);
int screen_create_window(screen_window_t *pwin, screen_context_t ctx);
int screen_create_window_type(screen_window_t *pwin, screen_context_t ctx, int type);
int screen_create_window_buffers(screen_window_t win, int count);
int screen_create_window_group(screen_window_t win, const char *name);
int screen_destroy_window(screen_window_t win);
int screen_destroy_window_buffers(screen_window_t win);
int screen_get_window_property_cv(screen_window_t win, int pname, int len, char *param);
int screen_get_window_property_iv(screen_window_t win, int pname, int *param);
int screen_get_window_property_llv(screen_window_t win, int pname, long long *param);
int screen_get_window_property_pv(screen_window_t win, int pname, void **param);
int screen_join_window_group(screen_window_t win, const char *name);
int screen_post_window(screen_window_t win, screen_buffer_t buf, int count, const int *dirty_rects, int flags);
int screen_read_window(screen_window_t win, screen_buffer_t buf, int count, const int *save_rects, int flags);
int screen_ref_window(screen_window_t win);
int screen_set_window_property_cv(screen_window_t win, int pname, int len, const char *param);
int screen_set_window_property_iv(screen_window_t win, int pname, const int *param);
int screen_set_window_property_llv(screen_window_t win, int pname, const long long *param);
int screen_set_window_property_pv(screen_window_t win, int pname, void **param);
int screen_share_window_buffers(screen_window_t win, screen_window_t share);
int screen_unref_window(screen_window_t win);

/*
 * Debugging
 */

int screen_print_packet(int type, void *packet, FILE *fd);

__END_DECLS

#endif /* _SCREEN_SCREEN_H_ */

__SRCVERSION("$URL: http://community.qnx.com/svn/repos/vivantesync/QNX/source/trunk/lib/screen/public/screen/screen.h $ $Rev: 451 $")
