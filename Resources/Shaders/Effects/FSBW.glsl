precision mediump float;
uniform sampler2D u_texture0;
#define u_textureColor u_texture0
varying vec2 v_uv;
void main()
{
	vec4 color = texture2D(u_textureColor,v_uv);
	float bw = (color.x + color.y + color.z)/3.0;
	gl_FragColor = vec4( bw, bw, bw, 1.0);
}
