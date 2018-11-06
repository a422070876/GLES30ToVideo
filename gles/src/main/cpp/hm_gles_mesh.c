//
// Created by gutou on 2017/4/18.
//

#include "hm_types.h"

void hm_gles_init_video_vertex_buffers(hm_gles_model *gles_model){
    glGenBuffers(2,gles_model->video_bos);

    glBindBuffer(GL_ARRAY_BUFFER,gles_model->video_bos[0]);
    glBufferData(GL_ARRAY_BUFFER, 12* sizeof(float),gles_model->vertex_data,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,gles_model->video_bos[1]);
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float),gles_model->texture_vertex_data,GL_STATIC_DRAW);

    glGenBuffers(2,gles_model->frame_bos);

    glBindBuffer(GL_ARRAY_BUFFER,gles_model->frame_bos[0]);
    glBufferData(GL_ARRAY_BUFFER, 12* sizeof(float),gles_model->vertex_data,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,gles_model->frame_bos[1]);
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float),gles_model->texture_vertex_data,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}
void hm_gles_delete_video_vertex_buffers(hm_gles_model *gles_model){
    glDeleteBuffers(2,gles_model->video_bos);
}
void hm_gles_video_bind_vertex_buffers(hm_video_glsl_program *video_program,GLuint *video_bos,
                                      float *st_matrix){
    glBindBuffer(GL_ARRAY_BUFFER,video_bos[0]);
    glEnableVertexAttribArray((GLuint) video_program->positon_location);
    glVertexAttribPointer((GLuint) video_program->positon_location, 3, GL_FLOAT, GL_FALSE,
                          0, 0);

    glBindBuffer(GL_ARRAY_BUFFER,video_bos[1]);
    glEnableVertexAttribArray((GLuint) video_program->texcoord_location);
    glVertexAttribPointer((GLuint) video_program->texcoord_location,2,GL_FLOAT,GL_FALSE,0,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glUniformMatrix4fv(video_program->st_matrix, 1, false, st_matrix);
}
void hm_gles_frame_bind_vertex_buffers(hm_video_glsl_program *frame_program,GLuint *frame_bos){
    glBindBuffer(GL_ARRAY_BUFFER,frame_bos[0]);
    glEnableVertexAttribArray((GLuint) frame_program->positon_location);
    glVertexAttribPointer((GLuint) frame_program->positon_location, 3, GL_FLOAT, GL_FALSE,
                          0, 0);

    glBindBuffer(GL_ARRAY_BUFFER,frame_bos[1]);
    glEnableVertexAttribArray((GLuint) frame_program->texcoord_location);
    glVertexAttribPointer((GLuint) frame_program->texcoord_location,2,GL_FLOAT,GL_FALSE,0,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}