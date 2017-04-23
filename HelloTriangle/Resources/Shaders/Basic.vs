attribute vec3 a_position;
attribute vec2 a_uv;
uniform mat4 u_W;
varying vec2 v_uv;

void main()
{
	vec4 posL = vec4(a_position,1.0);
	gl_Position = u_W * posL;
	v_uv = a_uv;
}