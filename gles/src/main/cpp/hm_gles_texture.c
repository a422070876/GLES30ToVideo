//
// Created by gutou on 2017/4/18.
//

#include "hm_types.h"

void hm_gles_init_texture(hm_gles_model *gles_model){
    glGenTextures(2,gles_model->textures);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, gles_model->textures[0]);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, gles_model->textures[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gles_model->width, gles_model->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);



    glGenFramebuffers(1, gles_model->frame_buffers);
    glBindFramebuffer(GL_FRAMEBUFFER, gles_model->frame_buffers[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gles_model->textures[1], 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);



}
void hm_gles_delete_texture(hm_gles_model *gles_model){
    glDeleteTextures(1,gles_model->textures);
}
static void hm_gles_bind_video_texture(hm_video_glsl_program *video_program, GLuint video_texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, video_texture);
    glUniform1i(video_program->tex_v, 0);
}
static void hm_gles_bind_frame_texture(hm_video_glsl_program *frame_program, GLuint frame_texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glUniform1i(frame_program->tex_v, 0);
}
void hm_gles_draw_texture(hm_gles_model *gles_model){
    glBindFramebuffer(GL_FRAMEBUFFER, gles_model->frame_buffers[0]);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, gles_model->width, gles_model->height);
    glUseProgram(gles_model->video_program->program);
    hm_gles_bind_video_texture(gles_model->video_program,gles_model->textures[0]);
    hm_gles_video_bind_vertex_buffers(gles_model->video_program,gles_model->video_bos,
                                     gles_model->st_matrix);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(gles_model->left, gles_model->top, gles_model->right, gles_model->bottom);
    glUseProgram(gles_model->frame_program->program);
    hm_gles_bind_frame_texture(gles_model->frame_program,gles_model->textures[1]);
    hm_gles_frame_bind_vertex_buffers(gles_model->frame_program,gles_model->frame_bos);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
