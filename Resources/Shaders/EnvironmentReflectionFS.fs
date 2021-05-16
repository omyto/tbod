precision mediump float;
uniform samplerCube u_cubeTexture;
uniform vec3 u_posCamW;
varying vec3 v_posW;
varying vec3 v_normW;
void main()
{
	vec3 toEye = u_posCamW - v_posW;
	vec3 reflectDir = reflect(normalize(-toEye), normalize(v_normW));
	gl_FragColor = textureCube(u_cubeTexture, reflectDir);
}
