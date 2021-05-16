precision mediump float;
uniform sampler2D u_texture0;
#define u_texture u_texture0
varying vec4 v_color;
varying vec2 v_uv;
void main()
{
	vec4 color = texture2D(u_texture,v_uv);
	//float bw = (color.x + color.y + color.z)/3.0;
	//gl_FragColor = vec4( bw, bw, bw, 1.0);
    gl_FragColor = color;
	//gl_FragColor = texture2D(u_texture,v_uv);
	//gl_FragColor = v_color;
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
