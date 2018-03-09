#version 420

#define MAX_ARRAY 128

// Vertex Shader
attribute vec4 vert;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec2 Index;
attribute vec2 weight;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 Bone[MAX_ARRAY]; // Array of bones that you compute (animate) on the CPU and you upload to the shader
uniform float numBones;
// --------------------
out vec2 texCoord0;
out vec3 eyeNormal;
out vec3 position;
// --------------------
void main()
{
    vec4 newVertex;
    vec4 newNormal;
    int index;
    // --------------------
    index = int(Index.x); // Cast to int
    newVertex = (Bone[index] * vert) * Weight.x;
    newNormal = (Bone[index] * vec4(normal, 0.0)) * weight.x;

    index = int(Index.y); //Cast to int
    newVertex = (Bone[index] * vert) * weight.y + newVertex;
    newNormal = (Bone[index] * vec4(normal, 0.0)) * weight.y + newNormal;

    eyeNormal = vec3(uView * newNormal);
    gl_Position = uProj * vec4(newVertex.xyz, 1.0);
    texCoord0 = texCoord;
}