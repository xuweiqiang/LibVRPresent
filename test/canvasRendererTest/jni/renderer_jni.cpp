#include "test_renderer_canvas_NativeLayer.h"
#include <core/SkBitmap.h>


#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "renderer_jni"
#include <utils/Log.h>


SkBitmap * g_bitmap;

struct fields_t 
{
    jfieldID    context;
    jfieldID    bitmap;
    /* actually in android.view.Surface XXX */
    jfieldID    bitmap_native;
};
static fields_t fields;


void JNICALL Java_test_renderer_canvas_NativeLayer_NativeInit(JNIEnv * env, jclass thiz)
{
	LOGE("NativeInit in\n");
	
    jclass clazz = env->FindClass("test/renderer/canvas/NativeLayer");
    if (clazz == NULL) 
	{
    //    jniThrowException(env, "java/lang/RuntimeException", "Can't find test/renderer/canvas/NativeLayer");
		LOGE("Can't find class test/renderer/canvas/NativeLayer");
        return;
    }

	fields.bitmap = env->GetFieldID(clazz, "mBitmap", "Landroid/graphics/Bitmap;");
    if (fields.bitmap == NULL) 
	{
     //   jniThrowException(env, "java/lang/RuntimeException", "Can't find NativeLayer.mBitmap");
		LOGE("Can't find variable NativeLayer.mBitmap");
        return;
    }

	jclass bitmapClass = env->FindClass("android/graphics/Bitmap");
    if (bitmapClass == NULL) {
    //    jniThrowException(env, "java/lang/RuntimeException", "Can't find android/graphics/Bitmap");
		LOGE("Can't find class android/graphics/Bitmap");
        return;
    }

	fields.bitmap_native = env->GetFieldID(bitmapClass, "mNativeBitmap", "I");
    if (fields.bitmap_native == NULL) {
   //     jniThrowException(env, "java/lang/RuntimeException", "Can't find Bitmap.mNativeBitmap");
		LOGE("Can't find variable Bitmap.mNativeBitmap");
        return;
    }

	LOGI("NativeInit out - succssfully\n");
}

jint JNICALL Java_test_renderer_canvas_NativeLayer_NativeSetup(JNIEnv * env, jobject thiz)
{
	LOGI("NativeSetup in\n");

	// thiz is object of java class 'test/renderer/canvas/NativeLayer'
	jobject bitmap = env->GetObjectField(thiz, fields.bitmap);
	g_bitmap = (SkBitmap *)env->GetIntField(bitmap, fields.bitmap_native);

	return (g_bitmap!= NULL) ? 0 : -1;
}

jint JNICALL Java_test_renderer_canvas_NativeLayer_NativeRender(JNIEnv *, jobject)
{
//	LOGI("NativeRender in\n");
	if (g_bitmap == NULL)
	{
		LOGE("NativeRender - bitmap not initialized!\n");
		return -1;
	}
	unsigned char * data = (unsigned char *)g_bitmap->getPixels();
	if (data == NULL)
	{
		if (!g_bitmap->allocPixels(NULL, NULL))
		{
			LOGE("NativeRender - failed to allocPixels!");
		}
		else
		{
			data = (unsigned char *)g_bitmap->getPixels();
			/*int len = g_bitmap->getSize();
			if (data != NULL && len > 0)
			{
				const unsigned char col = 128;
				memset(data, col, len);
			}*/
		}
	}
	int len = g_bitmap->getSize();
	int width = g_bitmap->width();
	int height = g_bitmap->height();
	//LOGI("NativeRender - data %p, len %d, size %dx%d", data, len, width, height);

	static unsigned char col = 0;
	if (data != NULL && len > 0)
	{
		memset(data, col, len);
		col += 1;
	}
	
	return 0;
}


