#version 420

#define MAX_ARRAY 128

// Vertex Shader
attribute vec4 Vertex;
attribute vec3 Normal;
attribute vec2 TexCoord;
attribute vec2 Index;
attribute vec2 Weight;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 Bone[MAX_ARRAY]; // Array of bones that you compute (animate) on the CPU and you upload to the shader
// --------------------
varying vec2 TexCoord0;
varying vec3 EyeNormal;
// --------------------
void main()
{
    vec4 newVertex;
    vec4 newNormal;
    int index;
    // --------------------
    index=int(Index.x); // Cast to int
    newVertex = (Bone[index] * Vertex) * Weight.x;
    newNormal = (Bone[index] * vec4(Normal, 0.0)) * Weight.x;
    index=int(Index.y); //Cast to int
    newVertex = (Bone[index] * Vertex) * Weight.y + newVertex;
    newNormal = (Bone[index] * vec4(Normal, 0.0)) * Weight.y + newNormal;
    EyeNormal = vec3(uView * newNormal);
    gl_Position = uProj * vec4(newVertex.xyz, 1.0);
    TexCoord0 = TexCoord;
}