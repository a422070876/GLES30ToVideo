//
// Created by gutou on 2017/4/18.
//

#include "hm_types.h"

static void init_frame(hm_gles_model *gles_model){
    hm_gles_glsl_video_program_init(gles_model);
    hm_gles_init_texture(gles_model);
    hm_gles_init_video_vertex_buffers(gles_model);
}
static void free_frame(hm_gles_model *gles_model){
    hm_gles_delete_video_vertex_buffers(gles_model);
    hm_gles_delete_texture(gles_model);
    hm_gles_glsl_video_program_delete(gles_model);
}
static void draw_frame(hm_gles_model *gles_model){
    hm_gles_draw_texture(gles_model);
}
hm_gles_model * hm_gles_model_create() {

    hm_gles_model *gles_model = (hm_gles_model *) malloc(sizeof(hm_gles_model));


    gles_model->video_program = malloc(sizeof(hm_video_glsl_program));

    gles_model->textures = malloc(sizeof(GLuint));


    gles_model->st_matrix = malloc(sizeof(float) * 16);


    gles_model->vertex_data = malloc(sizeof(float) * 12);
    gles_model->texture_vertex_data = malloc(sizeof(float) * 8);
    gles_model->video_bos = malloc(sizeof(GLuint) * 2);

    gles_model->left = 0;
    gles_model->right = 0;
    gles_model->top = 0;
    gles_model->bottom = 0;



    gles_model->vertex_data[0] = 1.0f;
    gles_model->vertex_data[1] = -1.0f;
    gles_model->vertex_data[2] = 0.0f;
    gles_model->vertex_data[3] = -1.0f;
    gles_model->vertex_data[4] = -1.0f;
    gles_model->vertex_data[5] = 0.0f;
    gles_model->vertex_data[6] = 1.0f;
    gles_model->vertex_data[7] = 1.0f;
    gles_model->vertex_data[8] = 0.0f;
    gles_model->vertex_data[9] = -1.0f;
    gles_model->vertex_data[10] = 1.0f;
    gles_model->vertex_data[11] = 0.0f;


    gles_model->texture_vertex_data[0] = 1.0f;
    gles_model->texture_vertex_data[1] = 0.0f;
    gles_model->texture_vertex_data[2] = 0.0f;
    gles_model->texture_vertex_data[3] = 0.0f;
    gles_model->texture_vertex_data[4] = 1.0f;
    gles_model->texture_vertex_data[5] = 1.0f;
    gles_model->texture_vertex_data[6] = 0.0f;
    gles_model->texture_vertex_data[7] = 1.0f;

    gles_model->init_frame = init_frame;
    gles_model->free_frame = free_frame;

    gles_model->draw_frame = draw_frame;

    return gles_model;
}

void hm_gles_model_free(hm_gles_model *gles_model) {
    free(gles_model->video_bos);
    free(gles_model->texture_vertex_data);
    free(gles_model->vertex_data);
    free(gles_model->st_matrix);
    free(gles_model->textures);
    free(gles_model->video_program);
    free(gles_model);
}