#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in float a_texID;

uniform mat4 u_projection;
uniform mat4 u_view;

out vec4 v_color;
out vec2 v_uv;
out float v_texID;

void main(void)
{
	v_color = a_color;
	v_uv = a_uv;
	v_texID = a_texID;

	gl_Position = u_projection * u_view * vec4(a_position, 1.0);
}