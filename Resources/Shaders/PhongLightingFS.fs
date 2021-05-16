precision mediump float;
uniform sampler2D u_texture0;
#define u_textureObj u_texture0
#define MAX_LIGHT_UNIT 5

uniform vec3 u_posCamW;
varying vec3 v_posW;
varying vec3 v_normW;
varying vec2 v_uv;

uniform vec4 u_ambientColor;
uniform float u_ambientWeight;
struct LightProperties
{
	vec4 color;
	vec3 position;
	bool isDirection;
};
uniform LightProperties u_light[MAX_LIGHT_UNIT];

float specularPower = 32.0;

vec4 totalSpecular = vec4(0.0, 0.0, 0.0, 0.0);
vec4 totalDiffuse = vec4(0.0, 0.0, 0.0, 0.0);
void CalculateSpecularDiffuse(vec4 objColor, vec3 lightPosW, vec4 lightColor, bool isDirection)
{
	vec3 lightDirection = normalize(v_posW - lightPosW);
	if (isDirection)
	{
		lightDirection = normalize(-lightPosW);
	}
	vec4 specularColor = lightColor;
	vec4 diffuseColor = lightColor;

	vec4 diffuseComponent = objColor * lightColor * max(dot(normalize(v_normW), -lightDirection), 0.0);
	
	vec3 toEye = normalize(u_posCamW - v_posW);
	
	vec3 reflectVector = normalize(reflect(lightDirection, normalize(v_normW)));
	
	vec4 specularComponent = pow( max(dot(reflectVector, toEye), 0.0), specularPower ) * specularColor;
	
	totalSpecular += specularComponent;
	totalDiffuse += diffuseComponent;
	
}
void main()
{
	vec4 objColor = texture2D(u_textureObj, v_uv);
	vec4 ambientComponent = objColor * u_ambientColor;
	
	for (int i = 0 ; i < MAX_LIGHT_UNIT ; i++)
	{
		CalculateSpecularDiffuse(objColor, u_light[i].position, u_light[i].color, u_light[i].isDirection);
	}
	
	vec4 finalColor = vec4(((ambientComponent*u_ambientWeight + (1.0 - u_ambientWeight)*totalDiffuse)*objColor + totalSpecular).xyz, objColor.w );
	
	gl_FragColor = finalColor;
}
