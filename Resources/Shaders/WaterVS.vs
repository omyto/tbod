attribute vec3 a_posL;
attribute vec3 a_normL;
attribute vec3 a_binormL;
attribute vec3 a_tangentL;
attribute vec2 a_uv;

uniform mat4 u_WVP;
uniform mat4 u_W;

varying vec3 v_posW;
varying vec3 v_normW;
varying vec3 v_binormW;
varying vec3 v_tangentW;
varying vec2 v_uv;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = u_WVP * posL;
		
	vec4 normL = vec4(a_normL, 0.0);
	vec4 binormL = vec4(a_binormL, 0.0);
	vec4 tangentL = vec4(a_tangentL, 0.0);

	v_posW = (u_W * posL).xyz;
	
	v_normW = (u_W * normL).xyz;
	v_binormW = (u_W * binormL).xyz;
	v_tangentW = (u_W * tangentL).xyz;
	/*
	v_normW = a_normL;
	v_binormW = a_binormL;
	v_tangentW = a_tangentL;
	*/
	v_uv = a_uv;
}