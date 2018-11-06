//
// Created by gutou on 2017/4/18.
//

#include "hm_types.h"

void hm_gles_init_texture(hm_gles_model *gles_model){
    glGenTextures(1,gles_model->textures);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, gles_model->textures[0]);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
void hm_gles_delete_texture(hm_gles_model *gles_model){
    glDeleteTextures(1,gles_model->textures);
}
static void hm_sdk_bind_video_texture(hm_video_glsl_program *video_program, GLuint video_texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, video_texture);
    glUniform1i(video_program->tex_v, 0);
}
void hm_gles_draw_texture(hm_gles_model *gles_model){
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(gles_model->left, gles_model->top, gles_model->right, gles_model->bottom);
    glUseProgram(gles_model->video_program->program);
    hm_sdk_bind_video_texture(gles_model->video_program,gles_model->textures[0]);
    hm_sdk_video_bind_vertex_buffers(gles_model->video_program,gles_model->video_bos,
                                     gles_model->st_matrix);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


}
