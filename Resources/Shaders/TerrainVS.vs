attribute vec3 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv;
varying vec2 v_uvTiling;
uniform mat4 u_WVP;
uniform mat4 u_W;
varying vec3 v_posW;
uniform sampler2D u_texture0;
float tilingFactor = 20.0;
float scaleFactor = 15.0;
void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	vec4 heightMap = texture2D(u_texture0, a_uv);
	posL.y = heightMap.y * scaleFactor;
	gl_Position = u_WVP * posL;
	v_uv = a_uv;
	v_uvTiling = a_uv * tilingFactor;
	v_posW = (u_W * posL).xyz;
}
   