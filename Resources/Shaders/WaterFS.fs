precision mediump float;

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
#define waterNormal u_texture0
#define bottomTexture u_texture1
#define displacementTexture u_texture2
#define MAX_LIGHT_UNIT 5

uniform samplerCube u_cubeTexture;
uniform vec3 u_posCamW;
uniform vec4 u_ambientColor;
uniform float u_ambientWeight;
uniform float u_time;
uniform float u_depthAjust;

varying vec3 v_posW;
varying vec3 v_normW;
varying vec3 v_binormW;
varying vec3 v_tangentW;
varying vec2 v_uv;

const float fresnelPower = 3.0;
const vec4 waterColor = vec4(0.0, 0.5, 1.0, 1.0);
void main()
{
	//----------- Displacement -------------
	float dMax = v_uv.x * u_depthAjust;
	vec2 disp = texture2D(displacementTexture, vec2(v_uv.x, v_uv.y + u_time)).rg;
	vec2 offsetDisp = (2.0 * disp - 1.0) * dMax;
	vec2 newUV = v_uv + offsetDisp;
	
	//---------- Depth -------------------
	float depthFactor = clamp(u_depthAjust * v_uv.x, 0.0, 1.0);
	
	//----------- Normal map -----------
	//vec3 normal = texture2D(waterNormal, v_uv).xyz;
	vec3 normal = texture2D(waterNormal, newUV).xyz;
	mat3 TBN = mat3(normalize(v_tangentW), normalize(v_binormW), normalize(v_normW));
	vec3 normalW = normalize((TBN * (2.0 * normal - 1.0)).xyz);
	
	//-----------  Refraction ----------- 
	//vec4 bottomColor = texture2D(bottomTexture, v_uv);
	vec4 bottomColor = texture2D(bottomTexture, newUV);
	vec4 refractionColor = mix(bottomColor, waterColor, depthFactor);
	
	//----------- Reflection -----------
	vec3 toEye = u_posCamW - v_posW;
	vec3 reflectDir = reflect(normalize(-toEye), normalize(normalW));
	vec4 reflectionColor = textureCube(u_cubeTexture, reflectDir);
	
	
	//----------- Fresnel -----------
	float fresnelTerm = pow((1.0 - abs(dot(normalize(normalW), normalize(toEye)))), fresnelPower);
	
	//----------- Final Color -----------
	gl_FragColor = vec4(mix(refractionColor, reflectionColor, fresnelTerm).xyz, 1.0);
	//gl_FragColor = texture2D(displacementTexture, v_uv);
	
	
	
	
	
	/*
	// Light
	vec4 objColor = reflectionColor;
	vec4 ambientComponent = objColor * u_ambientColor  * 1.0;
	for (int i = 0 ; i < MAX_LIGHT_UNIT ; i++)
	{
		CalculateSpecularDiffuse(objColor, u_light[i].position, u_light[i].color, normalW);
	}
	vec4 objColorLightReflect = vec4(((ambientComponent*u_ambientWeight + (1.0 - u_ambientWeight)*totalDiffuse)*objColor + totalSpecular).xyz, objColor.w );
	*/
	
	//gl_FragColor = reflectionColor;
	
	//gl_FragColor = texture2D(u_texture,v_uv);
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

/*
struct LightProperties
{
	vec4 color;
	vec3 position;
};
uniform LightProperties u_light[MAX_LIGHT_UNIT];

float specularPower = 32.0;

vec4 totalSpecular = vec4(0.0, 0.0, 0.0, 0.0);
vec4 totalDiffuse = vec4(0.0, 0.0, 0.0, 0.0);

void CalculateSpecularDiffuse(vec4 objColor, vec3 lightPosW, vec4 lightColor, vec3 normalW)
{
	vec3 lightDirection = normalize(v_posW - lightPosW);
	vec4 specularColor = lightColor;
	vec4 diffuseColor = lightColor;

	vec4 diffuseComponent = objColor * lightColor * max(dot(normalW, -lightDirection), 0.0);
	
	vec3 toEye = normalize(u_posCamW - v_posW);
	
	vec3 reflectVector = normalize(reflect(lightDirection, normalW));
	
	vec4 specularComponent = pow( max(dot(reflectVector, toEye), 0.0), specularPower ) * specularColor;
	
	totalSpecular += specularComponent;
	totalDiffuse += diffuseComponent;
	
}
*/