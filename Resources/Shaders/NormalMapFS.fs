precision mediump float;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
#define u_objTexture u_texture0
#define u_normalMapTexture u_texture1
#define MAX_LIGHT_UNIT 5

uniform vec3 u_posCamW;
uniform vec4 u_ambientColor;
uniform float u_ambientWeight;

varying vec3 v_normW;
varying vec3 v_binormW;
varying vec3 v_tangentW;
varying vec2 v_uv;
varying vec3 v_posW;
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
void main()
{
	vec3 normal = texture2D(u_normalMapTexture, v_uv).xyz;
	vec3 newNormal = 2.0 * normal - 1.0;
	
	mat3 TBN = mat3(normalize(v_tangentW), normalize(v_binormW), normalize(v_normW));
	
	vec3 normalW = normalize((TBN * newNormal).xyz);
	
	vec4 objColor = texture2D(u_objTexture, v_uv);
	vec4 ambientComponent = objColor * u_ambientColor  * 1.0;
	
	for (int i = 0 ; i < MAX_LIGHT_UNIT ; i++)
	{
		CalculateSpecularDiffuse(objColor, u_light[i].position, u_light[i].color, normalW);
	}
	
	vec4 finalColor = vec4(((ambientComponent*u_ambientWeight + (1.0 - u_ambientWeight)*totalDiffuse)*objColor + totalSpecular).xyz, objColor.w );
	gl_FragColor = finalColor;
}
