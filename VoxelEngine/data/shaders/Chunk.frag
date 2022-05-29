#version 330 core

in vec2		v_uv;
in float	v_light;
out vec4	color;

uniform sampler2D u_image;

void main()
{
	vec4 base_color = texture(u_image, v_uv);
	color = base_color * v_light;
	color.a = base_color.a;
}
