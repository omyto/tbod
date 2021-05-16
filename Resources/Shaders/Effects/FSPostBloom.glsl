precision mediump float;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
#define u_textureColor0 u_texture0
#define u_textureColor1 u_texture1
uniform float u_mix;
varying vec2 v_uv;
void main()
{
	vec4 color0 = texture2D(u_textureColor0, v_uv);
	vec4 color1 = texture2D(u_textureColor1, v_uv);
	//gl_FragColor = mix(color1, color0, u_mix);
	gl_FragColor = color0 + color1 * u_mix;
}
