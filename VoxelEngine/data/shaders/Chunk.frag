#version 330 core

in vec2 v_uv;
out vec4 color;

uniform sampler2D u_image;

void main()
{
	color = texture(u_image, v_uv);
}
