precision mediump float;
uniform sampler2D u_texture0;
#define u_textureColor u_texture0
varying vec2 v_uv;
float u_width = 960.0;
float u_height = 720.0;
const float thresholdMin = 0.25;
const float thresholdMax = 0.75;
const float thresholdFactor = 2.0;

float threshold(float val)
{
	/*
	if (val < thresholdValue)
	{
		return 0.0;
	}
	else
	{
		return 1.0;
	}
	*/
	if (val < thresholdMin)
	{
		return 0.0;
	}
	if (val > thresholdMax)
	{
		return 1.0;
	}
	return val;
}

//Averaged pixel intensity from 3 color channels
float avgIntensity(vec4 pix)
{
	return (pix.r + pix.g + pix.b)/3.0;
}

vec4 getPixel(vec2 uv, float dx, float dy)
{
	return texture2D(u_textureColor, uv + vec2(dx, dy));
}

//Returns pixel color
float IsEdge(vec2 uv)
{
	float dxTex = 1.0 / u_width;
	float dyTex = 1.0 / u_height;
	float pix[9];
	int k = -1;
	float delta;

	//Read neighboring pixel intensities
	for (int i = -1; i < 2; i++)
	{
		for(int j = -1; j < 2; j++)
		{
			k++;
			pix[k] = avgIntensity(getPixel(uv, float(i) * dxTex, float(j) * dyTex));
		}
	}

	//Average color differences around neighboring pixels
	delta = (abs(pix[1] - pix[7]) + abs(pix[5] - pix[3]) + abs(pix[0] - pix[8])+ abs(pix[2] - pix[6])) / 4.0;

	float val = clamp(thresholdFactor * delta, 0.0, 1.0);
	return threshold(val);
}

void main()
{
	float bw = IsEdge(v_uv);
	gl_FragColor = vec4(bw, bw, bw, 1.0);
}
