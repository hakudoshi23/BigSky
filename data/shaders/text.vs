attribute vec3 a_vertex;

uniform mat4 u_mvp;

void main() {
	gl_Position = u_mvp * vec4(a_vertex, 1);
}