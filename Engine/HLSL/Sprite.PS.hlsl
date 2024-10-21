#include "Sprite.2d.hlsli"

struct Material {
	float4 color;
	float4x4 uvTransform;
	float2 drawRange;
	int isDiscard;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	if (transformedUV.x > gMaterial.drawRange.x) {
		discard;
	}
	
	if (transformedUV.y > gMaterial.drawRange.y) {
		discard;
	}
	
	output.color = textureColor * gMaterial.color;
	output.color = clamp(output.color, 0.0f, 1.0f);
	
	if (output.color.a <= 0.1f) {
		discard;
	}
	
	return output;
}