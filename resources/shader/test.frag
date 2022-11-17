#version 330 core

out vec4 FragColor;

in vec2 out_tex_coord;
smooth in float out_tex_coord_affine;

uniform sampler2D our_texture;

void main()
{
    vec2 affine_tex_coords = out_tex_coord / out_tex_coord_affine;

    FragColor = texture(our_texture, affine_tex_coords);
}