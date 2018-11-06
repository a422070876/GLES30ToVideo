//
// Created by gutou on 2017/4/18.
//

#include <string.h>
#include "hm_types.h"


static const char *head = "#extension GL_OES_EGL_image_external : require\n";
#define STR(s) #s
static const char *vs = STR(
        attribute vec4 position;
        attribute vec2 texcoord;
        varying vec2 tx;
        void main() {
            tx = texcoord;
            gl_Position = position;
        }
);
static const char *fs_video = STR(
        uniform samplerExternalOES tex_v;
        uniform highp mat4 st_matrix;
        varying highp vec2 tx;
        void main() {
            highp vec2 tx_transformed = (st_matrix * vec4(tx, 0, 1.0)).xy;
            highp vec4 video = texture2D(tex_v, tx_transformed);
            gl_FragColor = video;
        }
);
static const char *fs_frame = STR(
        uniform sampler2D tex_v;
        varying highp vec2 tx;
        void main() {
            gl_FragColor = texture2D(tex_v, tx);
        }
);



static GLuint loadShader(GLenum shaderType, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;
    shader = glCreateShader(shaderType);
    if (shader == 0) return 0;
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 0) {
            char *infoLog = (char *) malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            LOGD("compile shader error ==>\n%s\n\n%s\n", shaderSrc, infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}


static GLuint loadProgram(const char *vsSrc, const char *fsSrc) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vsSrc);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fsSrc);
    if (vertexShader == 0 || fragmentShader == 0) return 0;
    GLint linked;
    GLuint pro = glCreateProgram();
    if (pro == 0) {
        LOGD("create program error!");
    }
    glAttachShader(pro, vertexShader);
    glAttachShader(pro, fragmentShader);
    glLinkProgram(pro);
    glGetProgramiv(pro, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen;
        glGetProgramiv(pro, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 0) {
            char *infoLog = (char *) malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(pro, infoLen, NULL, infoLog);
            LOGD("link program error ==>\n%s\n", infoLog);
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(pro);
        return 0;
    }
    return pro;
}



static void init_video(hm_video_glsl_program *program) {
    size_t head_size = strlen(head);
    size_t body_size = strlen(fs_video);
    char fs[head_size + body_size];
    strcpy(fs, head);
    strcat(fs, fs_video);
    GLuint pro = loadProgram(vs, fs);
    program->program = pro;
    program->positon_location = glGetAttribLocation(pro, "position");
    program->texcoord_location = glGetAttribLocation(pro, "texcoord");
    program->tex_v = glGetUniformLocation(pro, "tex_v");
    program->st_matrix = glGetUniformLocation(pro, "st_matrix");
}

static void init_frame(hm_video_glsl_program *program) {
    GLuint pro = loadProgram(vs, fs_frame);
    program->program = pro;
    program->positon_location = glGetAttribLocation(pro, "position");
    program->texcoord_location = glGetAttribLocation(pro, "texcoord");
    program->tex_v = glGetUniformLocation(pro, "tex_v");
}


void hm_gles_glsl_video_program_init(hm_gles_model *gles_model) {
    init_video(gles_model->video_program);
    init_frame(gles_model->frame_program);
}

void hm_gles_glsl_video_program_delete(hm_gles_model *gles_model) {
    glDeleteProgram(gles_model->video_program->program);
    glDeleteProgram(gles_model->frame_program->program);
}