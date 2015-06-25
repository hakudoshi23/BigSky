attribute vec3 a_vertex;
attribute vec4 a_color;
attribute vec2 a_uv;

uniform mat4 u_mvp;

varying vec3 v_position;
varying vec4 v_color;
varying vec2 v_uv;

void main()
{
	v_uv = a_uv;
	v_color = a_color;
	gl_Position = u_mvp * vec4( a_vertex, 1.0 );
}