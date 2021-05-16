precision mediump float;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
#define u_colorTexture u_texture0
#define u_depthTexture u_texture1
#define u_blurTexture u_texture2
uniform float u_near;
uniform float u_far;
uniform float u_fade;
uniform float u_clarity;
uniform vec4 u_step;
varying vec2 v_uv;
float CalculateMiaxFactor(vec2 uv)
{
	float depth = texture2D(u_depthTexture, uv).x;
	float z = -u_far * u_near / (depth * (u_far - u_near) - u_far);
	float k = clamp(abs(z - u_clarity)/ u_fade, 0.0, 1.0);
	return k;
}
void main()
{
	vec2 uv1 = v_uv;
	vec2 uv2 = v_uv + vec2(u_step.x, 0.0);
	vec2 uv3 = v_uv + vec2(u_step.z, u_step.w);
	vec2 uv4 = v_uv + vec2(0.0, u_step.y);
	vec2 uv5 = v_uv + vec2(-u_step.z, u_step.w);
	vec2 uv6 = v_uv + vec2(-u_step.x, 0.0);
	vec2 uv7 = v_uv + vec2(-u_step.z, -u_step.w);
	vec2 uv8 = v_uv + vec2(0.0, -u_step.y);
	vec2 uv9 = v_uv + vec2(u_step.z, -u_step.w);
	
	vec4 color1 = texture2D(u_blurTexture, uv1);
	vec4 color2 = texture2D(u_blurTexture, uv2);
	vec4 color3 = texture2D(u_blurTexture, uv3);
	vec4 color4 = texture2D(u_blurTexture, uv4);
	vec4 color5 = texture2D(u_blurTexture, uv5);
	vec4 color6 = texture2D(u_blurTexture, uv6);
	vec4 color7 = texture2D(u_blurTexture, uv7);
	vec4 color8 = texture2D(u_blurTexture, uv8);
	vec4 color9 = texture2D(u_blurTexture, uv9);
	
	float d1 = CalculateMiaxFactor(uv1);
	float d2 = CalculateMiaxFactor(uv2);
	float d3 = CalculateMiaxFactor(uv3);
	float d4 = CalculateMiaxFactor(uv4);
	float d5 = CalculateMiaxFactor(uv5);
	float d6 = CalculateMiaxFactor(uv6);
	float d7 = CalculateMiaxFactor(uv7);
	float d8 = CalculateMiaxFactor(uv8);
	float d9 = CalculateMiaxFactor(uv9);
	
	float total = 2.0 + d1 + d2 + d3 + d4 + d5 + d6 + d7 + d8 + d9;
	
	vec4 colorOrginal = texture2D(u_colorTexture, v_uv);
	vec4 colorBlur = (color1*2.0 + color2*d2 + color3*d3 + color4*d4 + color5*d5 + color6*d6 + color7*d7 + color8*d8 + color9*d9) / total;
	gl_FragColor = mix(colorOrginal, colorBlur, d1);
}
