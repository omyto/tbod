attribute vec3 a_posL;
attribute vec3 a_normL;
uniform mat4 u_WVP;
uniform mat4 u_W;
varying vec3 v_posW;
varying vec3 v_normW;
void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	vec4 normL = vec4(a_normL, 0.0);
	gl_Position = u_WVP * posL;
	v_posW = (u_W * posL).xyz;
	v_normW = (u_W * normL).xyz;
}
   