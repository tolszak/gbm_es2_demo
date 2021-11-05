/*
 * Copyright (c) 2012 Arvin Schnell <arvin.schnell@gmail.com>
 * Copyright (c) 2012 Rob Clark <rob@ti.com>
 * Copyright (c) 2016 Dongseong Hwang <dongseong.hwang@intel.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/* Based on a egl cube test app originally written by Arvin Schnell */

#include <cmath>
#include <memory>

#include "drm_modesetter.h"
#include "gbm_es2_demo.h"
#include "matrix.h"
#include "stb_image.h"

namespace demo {
ES2CubeImpl::~ES2CubeImpl()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteProgram(program_);
}

unsigned int texture;

bool ES2CubeImpl::Initialize(std::string card, bool atomic)
{
    std::unique_ptr<ged::DRMModesetter> drm =
        ged::DRMModesetter::Create(card, atomic);
    if (!drm) {
        fprintf(stderr, "failed to create DRMModesetter.\n");
        return false;
    }

    egl_ = ged::EGLDRMGlue::Create(
        std::move(drm), std::bind(&ES2CubeImpl::DidSwapBuffer, this,
                                  std::placeholders::_1, std::placeholders::_2));
    if (!egl_) {
        fprintf(stderr, "failed to create EGLDRMGlue.\n");
        return false;
    }
    display_size_ = egl_->GetDisplaySize();

    // Need to do the first mode setting before page flip.
    if (!InitializeGL())
        return false;
    return true;
}

bool ES2CubeImpl::Run()
{
    return egl_->Run();
}

#define esLogMessage printf

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0)
        return 0;

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);

            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            esLogMessage("Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool ES2CubeImpl::InitializeGL()
{
//    static const GLfloat vVertices[] = {
//        // front
//        0, 0, 0,
//        1, 0, 0,
//        1, 1, 0,
//        0, 1, 0,
//    };
    if (!InitializeGLProgram())
        return false;

//    printf("[%i], %i\n", __LINE__, glGetError());
//    fflush(stdout);
//    modelviewmatrix_ = glGetUniformLocation(program_, "modelviewMatrix");
//    modelviewprojectionmatrix_ =
//        glGetUniformLocation(program_, "modelviewprojectionMatrix");
//    normalmatrix_ = glGetUniformLocation(program_, "normalMatrix");
//    sourceTexture_ = glGetUniformLocation(program_, "sourceTexture");
//    printf("Source uniform loc: %i\n", sourceTexture_);

//    glViewport(0, 0, display_size_.width, display_size_.height);
//    glEnable(GL_CULL_FACE);

//    GLintptr positionsoffset = 0;
//    glGenBuffers(1, &vbo_);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
//    glBufferData(GL_ARRAY_BUFFER,
//                 sizeof(vVertices), 0,
//                 GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
//                          reinterpret_cast<const void *>(positionsoffset));
//    glEnableVertexAttribArray(0);

//    glGenTextures(1, &texture);
//    printf("[%i], %i, Texture: %i\n", __LINE__, glGetError(), texture);
//    fflush(stdout);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    printf("[%i], %i\n", __LINE__, glGetError());
//    fflush(stdout);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    printf("[%i], %i\n", __LINE__, glGetError());
//    fflush(stdout);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    printf("[%i], %i\n", __LINE__, glGetError());
//    fflush(stdout);

//    int width = 0, height = 0, nrChannels = 0;
//    unsigned char *data = stbi_load("/home/tolszak/workspace/glut-example/VK21-1.jpg", &width, &height, &nrChannels, 0);

//    if (data)
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//    printf("[%i], %i\n", __LINE__, glGetError());
//    fflush(stdout);
////    glEnable(GL_TEXTURE_2D);
////    glBindTexture(GL_TEXTURE_2D, 0);

//    glClearColor(0.0, 0.0, 0.0, 1.0);
//    glClear(GL_COLOR_BUFFER_BIT);
    return true;
}

GLuint CreateSimpleTexture2D()
{
    // Texture object handle
    GLuint textureId;

    // Generate a texture object
    glGenTextures(1, &textureId);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, textureId);
    glGenTextures(1, &texture);

    int width = 0, height = 0, nrChannels = 0;
    unsigned char *data = stbi_load("NumbersGrid.png", &width, &height, &nrChannels, 0);

    if (data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        abort();

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureId;
}

bool ES2CubeImpl::InitializeGLProgram()
{
    char vShaderStr[] =
        "#version 300 es                            \n"
        "precision highp float;                            \n"
        "layout(location = 0) in vec4 a_position;   \n"
        "layout(location = 1) in vec2 a_texCoord;   \n"
        "out vec2 v_texCoord;                       \n"
        "void main()                                \n"
        "{                                          \n"
        "   gl_Position = a_position;               \n"
        "   v_texCoord = a_texCoord;                \n"
        "}                                          \n";

    char fShaderStr[] =
        "#version 300 es                                     \n"
        "precision highp float;                            \n"
        "in vec2 v_texCoord;                                 \n"
        "layout(location = 0) out vec4 outColor;             \n"
        "uniform sampler2D s_texture;                        \n"
        "void main()                                         \n"
        "{                                                   \n"
        "  outColor = texture2D( s_texture, v_texCoord );      \n"
        "}  												 \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

    // Create the program object
    programObject = glCreateProgram();

    if (programObject == 0)
        return 0;

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    // Link the program
    glLinkProgram(programObject);

    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint infoLen = 0;

        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);

            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            esLogMessage("Error linking program:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteProgram(programObject);
        return false;
    }

    // Store the program object
    program_ = programObject;

    // Get the sampler location
    samplerlocation_ = glGetUniformLocation(program_, "s_texture");

    // Load the texture
    texture = CreateSimpleTexture2D();

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return true;
}

void ES2CubeImpl::DidSwapBuffer(GLuint gl_framebuffer, unsigned long usec)
{
//    printf("DidSwapBUffer\n");
    Draw(usec);

    static int num_frames = 0;
    static unsigned long lasttime = 0;
    static const size_t one_sec = 1000000;
    num_frames++;
    unsigned long elapsed = usec - lasttime;
    if (elapsed > one_sec) {
        printf("FPS: %4f \n", num_frames / ((double)elapsed / one_sec));
        num_frames = 0;
        lasttime = usec;
    }
}

GLfloat scale = 13;
void ES2CubeImpl::Draw(unsigned long usec)
{
    glClearColor(1, 1, 1, 1);
    glClearDepthf(1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    GLfloat coord = 1.f * scale;

    GLfloat vVertices[] = { -coord, coord, 0.0f,   // Position 0
                            0.0f, 1.0f,             // TexCoord 0
                            -coord, -coord, 0.0f,     // Position 1
                            0.0f, 0.0f,             // TexCoord 1
                            coord, -coord, 0.0f,     // Position 2
                            1.0f, 0.0f,             // TexCoord 2
                            coord, coord, 0.0f,       // Position 3
                            1.0f, 1.0f              // TexCoord 3
    };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    glViewport(0, 0, display_size_.width, display_size_.height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    glUseProgram(program_);

    // Load the vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT,
                          GL_FALSE, 5 * sizeof(GLfloat), vVertices);
//    // Load the texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT,
                          GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the sampler texture unit to 0
    glUniform1i(samplerlocation_, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
    static int LastSaved = -1;
    int currentSecond = usec / 1000 / 1000;
    if (currentSecond > LastSaved + 1) {
        scale++;
        int size = sizeof(float);
        printf("Changing scale to %f, sizeof(float): %i\n", scale, size);
        fflush(stdout);
        LastSaved = currentSecond + 1;
    }
}
}  // namespace demo
