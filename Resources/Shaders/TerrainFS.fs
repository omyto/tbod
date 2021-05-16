precision mediump float;
varying vec2 v_uv;
varying vec2 v_uvTiling;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform sampler2D u_texture4;
#define u_grassTexture	u_texture1
#define u_dirtTexture	u_texture2
#define u_rockTexture	u_texture3
#define u_terrainBlendTexture u_texture4
uniform vec3 u_posCamW;
varying vec3 v_posW;

uniform vec4 fogColor;
uniform float fogStart;
uniform float fogRange;

//vec4 fogColor = vec4(1.0, 1.0, 1.0, 1.0);
//float fogStart = 10.0;
//float fogRange = 15.0;

void main()
{
	vec4 grass = texture2D(u_grassTexture, v_uvTiling);
	vec4 dirt = texture2D(u_dirtTexture, v_uvTiling);
	vec4 rock = texture2D(u_rockTexture, v_uvTiling);
	vec4 blend = texture2D(u_terrainBlendTexture, v_uv);
	vec4 terainColor = (grass*blend.x + dirt*blend.y + rock*blend.z)/(blend.x + blend.y + blend.z);
	
	float distanceCamToFrag = distance(u_posCamW,v_posW);	
	float factor = clamp((distanceCamToFrag - fogStart)/fogRange, 0.0, 1.0);
	
	vec4 finalColor = mix(terainColor, fogColor, factor);
	//vec4 finalColor = (1.0 - factor)*terainColor + factor*fogColor;
		
	gl_FragColor = finalColor;
}
