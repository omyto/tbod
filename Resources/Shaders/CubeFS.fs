precision mediump float;
uniform samplerCube u_cubeTexture;
varying vec3 v_posL;

void main()
{
	gl_FragColor = textureCube(u_cubeTexture, v_posL);
	//gl_FragColor = vec4( 1.0,1.0,0.0,1.0);
}