Texture2D colorMap : register( t0 );
SamplerState colorSampler : register( s0 );

cbuffer cbChangesEveryFrame : register( b0 )
{
	matrix worldMatrix;
};

cbuffer cbNeverChanges : register( b1 )
{
	matrix viewMatrix;
};

cbuffer cbChangeOnResize : register( b2 )
{
	matrix projMatrix;
};

struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
};

PS_Input VS_Main( VS_Input vertex )
{
	PS_Input vsOut = ( PS_Input )0;
	vsOut.pos = mul( vertex.pos, worldMatrix );
	vsOut.pos = mul( vsOut.pos, viewMatrix );
	vsOut.pos = mul( vsOut.pos, projMatrix );
	vsOut.tex0 = vertex.tex0;
	return vsOut;
}

float4 PS_Main( PS_Input frag ) : SV_TARGET
{
	return 1.0f - colorMap.Sample( colorSampler, frag.tex0 );
}

technique11 ColorInversion
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VS_Main() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PS_Main() ) );
	}
}