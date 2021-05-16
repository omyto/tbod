precision mediump float;
uniform sampler2D u_texture0;
#define u_colorTexture u_texture0
uniform vec4 u_step;
varying vec2 v_uv;
//vec4 u_step = vec4(0.1, 0.1, 1.0, 1.0);
void main()
{
	vec4 color1 = texture2D(u_colorTexture, v_uv);
	vec4 color2 = texture2D(u_colorTexture, vec2(v_uv.x + u_step.x, v_uv.y));
	vec4 color3 = texture2D(u_colorTexture, vec2(v_uv.x - u_step.x, v_uv.y));
	vec4 color4 = texture2D(u_colorTexture, vec2(v_uv.x, v_uv.y + u_step.y));
	vec4 color5 = texture2D(u_colorTexture, vec2(v_uv.x, v_uv.y - u_step.y));
	vec4 color6 = texture2D(u_colorTexture, vec2(v_uv.x + u_step.z, v_uv.y + u_step.w));
	vec4 color7 = texture2D(u_colorTexture, vec2(v_uv.x - u_step.z, v_uv.y + u_step.w));
	vec4 color8 = texture2D(u_colorTexture, vec2(v_uv.x - u_step.z, v_uv.y - u_step.w));
	vec4 color9 = texture2D(u_colorTexture, vec2(v_uv.x + u_step.z, v_uv.y - u_step.w));
	
	gl_FragColor = (color1 * 2.0 + color2 + color3 + color4 + color5 + color6 + color7 + color8 + color9) * 0.1;
}
