//
// engine.h: This file contains the types and functions relative to the engine.
//

#pragma once

#include "Platform.h"

#include "glad/glad.h"

typedef glm::vec2  vec2;
typedef glm::vec3  vec3;
typedef glm::vec4  vec4;
typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;

struct Vertex
{
    vec3 position;
    vec2 uv;
};

struct Image
{
    void* pixels;
    ivec2 size;
    i32 nchannels;
    i32 stride;
};

struct Texture
{
    GLuint handle;
    std::string filepath;
};

struct Program
{
    GLuint handle;
    std::string filepath;
    std::string programName;
    u64 lastWriteTimestamp; // What is this for?
};

enum Mode
{
    Mode_TexturedQuad,
    Mode_Count
};

struct GLInfo
{
    const char* version;
    const char* vendor;
    const char* renderer;
    const char* glslVersion;
    std::string extensions;
};

struct App
{
    // Loop
    f32 deltaTime;
    bool isRunning;

    // Input
    Input input;

    GLInfo glInfo;

    ivec2 displaySize;

    std::vector<Texture> textures;
    std::vector<Program> programs;

    // Program indices
    u32 texturedGeometryProgramIdx;

    // Texture indices
    u32 diceTexIdx;
    u32 whiteTexIdx;
    u32 blackTexIdx;
    u32 normalTexIdx;
    u32 magentaTexIdx;

    // Mode
    Mode mode;

    // Embedded geometry (in-editor simple meshes such as
    // a screen filling quad, a cube, a sphere...)
    GLuint embeddedVertices;
    GLuint embeddedElements;

    // Location of the texture uniform in the textured quad shader
    GLuint programUniformTexture;

    // VAO object to link our screen filling quad with our textured quad shader
    GLuint vao;
};

void Init(App* app);

void ImGuiRender(App* app);

void Update(App* app);

void Render(App* app);