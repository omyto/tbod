precision mediump float;
uniform sampler2D u_texture0;
#define u_texture u_texture0
varying vec4 v_color;
varying vec2 v_uv;
void main()
{
	gl_FragColor = texture2D(u_texture,v_uv);
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//gl_FragColor = v_color;
}
