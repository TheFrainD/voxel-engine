#version 330 core

layout (location = 0) in vec3	a_position;
layout (location = 1) in vec2	a_uv;
layout (location = 2) in float	a_light;

out vec2	v_uv;
out float	v_light;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);

	v_uv = a_uv;
	v_light = a_light;
}
