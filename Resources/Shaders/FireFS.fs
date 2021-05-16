precision mediump float;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
#define u_fireTexture u_texture0
#define u_dispTexture u_texture1
#define u_maskTexture u_texture2
varying vec2 v_uv;
uniform float u_time;

float dMax = 0.25;
void main()
{
	vec2 disp = texture2D(u_dispTexture, vec2(v_uv.x, v_uv.y + u_time)).rg;
	vec2 offset = (2.0 * disp - 1.0) * dMax;
	vec2 newUV = v_uv + offset;
	vec4 fireColor = texture2D(u_fireTexture, newUV);
	vec4 alphaValue = texture2D(u_maskTexture, v_uv);
	//if (alphaValue.r < 0.3) discard;
	gl_FragColor = fireColor * vec4(1.0, 1.0, 1.0, alphaValue.r);
}