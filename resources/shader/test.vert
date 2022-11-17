#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_normal;
layout (location = 2) in vec2 a_tex_coord;

out vec2 out_tex_coord;
smooth out float out_tex_coord_affine;

uniform mat4 model_transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec2 res = vec2(800 / 10, 600 / 10); // Resolution used for vertex wobble

    vec4 vertex_view_m = view * model_transform * vec4(a_pos, 1.0);
    vec4 vertex_projected = projection * vertex_view_m;

    // Simulating vertex wobble
    vertex_projected.xyz = vertex_projected.xyz / vertex_projected.w;
    vertex_projected.xy = floor(res * vertex_projected.xy) / res;
    vertex_projected.xyz *= vertex_projected.w;

    gl_Position = vertex_projected;

    // Simulating affine mapping
    float dist = length(vertex_view_m);
    float affine = dist + ((vertex_projected.w * 8.0) / dist) * 0.5;

    out_tex_coord = a_tex_coord * affine;
    out_tex_coord_affine = affine;
}