/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_azavea_gdal_LockedDataset */

#ifndef _Included_com_azavea_gdal_LockedDataset
#define _Included_com_azavea_gdal_LockedDataset
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_azavea_gdal_LockedDataset
 * Method:    getTransform
 * Signature: ()[D
 */
JNIEXPORT jdoubleArray JNICALL Java_com_azavea_gdal_LockedDataset_getTransform
  (JNIEnv *, jobject);

/*
 * Class:     com_azavea_gdal_LockedDataset
 * Method:    getPixels
 * Signature: ([I[IILcom/azavea/gdal/GDALDataType;Ljava/nio/ByteBuffer;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_azavea_gdal_LockedDataset_getPixels
  (JNIEnv *, jobject, jintArray, jintArray, jint, jobject, jobject);

/*
 * Class:     com_azavea_gdal_LockedDataset
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_azavea_gdal_LockedDataset_close
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif