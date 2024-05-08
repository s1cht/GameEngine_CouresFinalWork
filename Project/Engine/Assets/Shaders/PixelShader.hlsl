Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer
{
    float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
    float specularPower;
    float4 specularColor;
    float4 partColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;

	float4 color;
    float4 specular;
    float3 reflection;
	float3 lightDir;
	float lightIntensity;
	
	textureColor = shaderTexture.Sample(SampleType, input.tex);

    color = ambientColor;

    specular = float4(0.f, 0.f, 0.f, 0.f);

	lightDir = -lightDirection;

	lightIntensity = saturate(dot(input.normal, lightDir));
	
    if (lightIntensity > 0.f)
    {
        color += (diffuseColor * lightIntensity);
        color = saturate(color);
        reflection = normalize(2.f * lightIntensity * input.normal - lightDir);
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }

    if (textureColor.r == 0.f && textureColor.g == 0.f && textureColor.b == 0.f && textureColor.a == 0.f)
    {
        color = color * partColor;
        printf("My color is %f %f %f %f", color.r, color.g, color.b, color.a);

    }
    else
    {
        color = color * partColor * textureColor;
    }
    color = saturate(color + specular);

    return color;
}