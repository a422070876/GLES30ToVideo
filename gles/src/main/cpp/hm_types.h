//
// Created by gutou on 2017/4/18.
//

#ifndef HM_TYPES_H
#define HM_TYPES_H
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdbool.h>
#include <android/log.h>
#define LOG_TAG "GLES"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

typedef struct struct_hm_video_glsl_program {
    GLuint program;
    GLint positon_location;
    GLint texcoord_location;
    GLint tex_v;
    GLint st_matrix;
} hm_video_glsl_program;


typedef struct struct_hm_gles_model {
    GLuint *textures;
    GLuint *video_bos;
    float *vertex_data;
    float *texture_vertex_data;
    hm_video_glsl_program *video_program;
    float *st_matrix;

    int left,top,right,bottom;


    void (*init_frame)(struct struct_hm_gles_model* gles_model);
    void (*free_frame)(struct struct_hm_gles_model* gles_model);

    void (*draw_frame)(struct struct_hm_gles_model* gles_model);

} hm_gles_model;
hm_gles_model * hm_gles_model_create();
void hm_gles_model_free(hm_gles_model *gles_model);

void hm_gles_init_video_vertex_buffers(hm_gles_model *gles_model);
void hm_gles_delete_video_vertex_buffers(hm_gles_model *gles_model);
void hm_sdk_video_bind_vertex_buffers(hm_video_glsl_program *video_program,GLuint *video_bos,
                                      float *st_matrix);

void hm_gles_init_texture(hm_gles_model *gles_model);
void hm_gles_delete_texture(hm_gles_model *gles_model);
void hm_gles_draw_texture(hm_gles_model *gles_model);

void hm_gles_glsl_video_program_init(hm_gles_model *gles_model);
void hm_gles_glsl_video_program_delete(hm_gles_model *gles_model);

#endif //HM_HM_AD_TYPES_H
