#version 330 core

in vec4 v_color;
in vec2 v_uv;
in float v_texID;

uniform sampler2D u_image[16];

out vec4 color;

void main(void)
{
	int index = int(v_texID);
	color = texture(u_image[index], v_uv) * v_color;
}