#include <jni.h>
#include <string>
#include <android/log.h>

extern "C"{
#include "hm_types.h"
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_hyq_hm_gles_GLES_initGLES(JNIEnv *env, jobject instance) {
    hm_gles_model* gles_model = hm_gles_model_create();
    return (jlong) gles_model;
}extern "C"
JNIEXPORT void JNICALL
Java_com_hyq_hm_gles_GLES_initFrame__J(JNIEnv *env, jobject instance, jlong gles) {

    // TODO

    hm_gles_model* gles_model = (hm_gles_model *) gles;
    gles_model->init_frame(gles_model);
}extern "C"
JNIEXPORT void JNICALL
Java_com_hyq_hm_gles_GLES_release(JNIEnv *env, jobject instance, jlong gles) {

    // TODO
    hm_gles_model* gles_model = (hm_gles_model *) gles;
    hm_gles_model_free(gles_model);
}extern "C"
JNIEXPORT void JNICALL
Java_com_hyq_hm_gles_GLES_setRect(JNIEnv *env, jobject instance, jlong gles, jint left, jint top, jint right,
                                  jint bottom) {

    // TODO
    hm_gles_model* gles_model = (hm_gles_model *) gles;
    gles_model->left = left;
    gles_model->top = top;
    gles_model->right = right;
    gles_model->bottom = bottom;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_hyq_hm_gles_GLES_getTexture(JNIEnv *env, jobject instance, jlong gles) {

    // TODO
    hm_gles_model* gles_model = (hm_gles_model *) gles;
    return gles_model->textures[0];
}extern "C"
JNIEXPORT void JNICALL
Java_com_hyq_hm_gles_GLES_drawFrame(JNIEnv *env, jobject instance, jlong gles,
                                                     jfloatArray mSTMatrix_) {
    jfloat *mSTMatrix = env->GetFloatArrayElements(mSTMatrix_, NULL);

    // TODO
    hm_gles_model* gles_model = (hm_gles_model *) gles;
    env->GetFloatArrayRegion(mSTMatrix_, 0, 16, gles_model->st_matrix);
    env->ReleaseFloatArrayElements(mSTMatrix_, mSTMatrix, 0);
    env->DeleteLocalRef(mSTMatrix_);
    gles_model->draw_frame(gles_model);

}extern "C"
JNIEXPORT void JNICALL
Java_com_hyq_hm_gles_GLES_releaseFrame(JNIEnv *env, jobject instance, jlong gles) {

    // TODO
    hm_gles_model* gles_model = (hm_gles_model *) gles;
    gles_model->free_frame(gles_model);
}