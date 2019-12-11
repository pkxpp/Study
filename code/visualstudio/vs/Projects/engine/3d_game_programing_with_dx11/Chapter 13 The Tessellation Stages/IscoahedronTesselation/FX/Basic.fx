#include "LightHelper.fx"
 
cbuffer cbPerFrame
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;
 
	float  gFogStart;
	float  gFogRange;
	float4 gFogColor;
};
 
cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
};
 
// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
 
SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
 
	AddressU = WRAP;
	AddressV = WRAP;
};
 
struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
};
 
struct VertexOut
{
    float3 PosL    : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex     : TEXCOORD;
};
 
struct GeoOut
{
    float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex     : TEXCOORD;
    float  FogLerp : FOG;
};
 
VertexOut VS(VertexIn vin)
{
    VertexOut vout;
 
    vout.PosL    = vin.PosL;
    vout.NormalL = vin.NormalL;
    vout.Tex     = vin.Tex;
 
    return vout;
}
 
void SubdivideZero(VertexOut inVerts[3], out VertexOut outVerts[3])
{
    outVerts[0] = inVerts[0];
    outVerts[1] = inVerts[1];
    outVerts[2] = inVerts[2];
}
 
void Subdivide(VertexOut inVerts[3], out VertexOut outVerts[6])
{
    VertexOut m[3];
 
    // Compute edge midpoints.
    m[0].PosL = 0.5f*(inVerts[0].PosL + inVerts[1].PosL);
    m[1].PosL = 0.5f*(inVerts[1].PosL + inVerts[2].PosL);
    m[2].PosL = 0.5f*(inVerts[2].PosL + inVerts[0].PosL);
 
    // Project onto unit sphere
    m[0].PosL = normalize(m[0].PosL);
    m[1].PosL = normalize(m[1].PosL);
    m[2].PosL = normalize(m[2].PosL);
 
    // Derive normals.
    m[0].NormalL = m[0].PosL;
    m[1].NormalL = m[1].PosL;
    m[2].NormalL = m[2].PosL;
 
    // Interpolate texture coordinates.
    m[0].Tex = 0.5f*(inVerts[0].Tex + inVerts[1].Tex);
    m[1].Tex = 0.5f*(inVerts[1].Tex + inVerts[2].Tex);
    m[2].Tex = 0.5f*(inVerts[2].Tex + inVerts[0].Tex);
 
    outVerts[0] = inVerts[0];
    outVerts[1] = m[0];
    outVerts[2] = m[2];
    outVerts[3] = m[1];
    outVerts[4] = inVerts[2];
    outVerts[5] = inVerts[1];
}
 
void SubdivideTwice(VertexOut inVerts[3], out VertexOut outVerts[15])
{
    VertexOut m[12];
 
    // Compute edge midpoints.
    m[0].PosL = 0.75f * inVerts[0].PosL + 0.25 * inVerts[2].PosL;
    m[1].PosL = 0.5f  * inVerts[0].PosL + 0.5  * inVerts[2].PosL;
    m[2].PosL = 0.25f * inVerts[0].PosL + 0.75 * inVerts[2].PosL;
    m[3].PosL = 0.75f * inVerts[2].PosL + 0.25 * inVerts[1].PosL;
    m[4].PosL = 0.5f  * inVerts[2].PosL + 0.5  * inVerts[1].PosL;
    m[5].PosL = 0.25f * inVerts[2].PosL + 0.75 * inVerts[1].PosL;
    m[6].PosL = 0.75f * inVerts[1].PosL + 0.25 * inVerts[0].PosL;
    m[7].PosL = 0.5f  * inVerts[1].PosL + 0.5  * inVerts[0].PosL;
    m[8].PosL = 0.25f * inVerts[1].PosL + 0.75 * inVerts[0].PosL;
    m[9].PosL = 0.5f  * m[4].PosL       + 0.5  * m[7].PosL;
    m[10].PosL = 0.5f * m[1].PosL       + 0.5  * m[7].PosL;
    m[11].PosL = 0.5f * m[1].PosL       + 0.5  * m[4].PosL;
 
    // Project onto unit sphere
    m[0].PosL = normalize(m[0].PosL);
    m[1].PosL = normalize(m[1].PosL);
    m[2].PosL = normalize(m[2].PosL);
    m[3].PosL = normalize(m[3].PosL);
    m[4].PosL = normalize(m[4].PosL);
    m[5].PosL = normalize(m[5].PosL);
    m[6].PosL = normalize(m[6].PosL);
    m[7].PosL = normalize(m[7].PosL);
    m[8].PosL = normalize(m[8].PosL);
    m[9].PosL = normalize(m[9].PosL);
    m[10].PosL = normalize(m[10].PosL);
    m[11].PosL = normalize(m[11].PosL);
 
    // Derive normals.
    m[0].NormalL = m[0].PosL;
    m[1].NormalL = m[1].PosL;
    m[2].NormalL = m[2].PosL;
    m[3].NormalL = m[3].PosL;
    m[4].NormalL = m[4].PosL;
    m[5].NormalL = m[5].PosL;
    m[6].NormalL = m[6].PosL;
    m[7].NormalL = m[7].PosL;
    m[8].NormalL = m[8].PosL;
    m[9].NormalL = m[9].PosL;
    m[10].NormalL = m[10].PosL;
    m[11].NormalL = m[11].PosL;
 
    // Interpolate texture coordinates.
    m[0].Tex = 0.75f * inVerts[0].Tex + 0.25 * inVerts[2].Tex;
    m[1].Tex = 0.5f  * inVerts[0].Tex + 0.5  * inVerts[2].Tex;
    m[2].Tex = 0.25f * inVerts[0].Tex + 0.75 * inVerts[2].Tex;
    m[3].Tex = 0.75f * inVerts[2].Tex + 0.25 * inVerts[1].Tex;
    m[4].Tex = 0.5f  * inVerts[2].Tex + 0.5  * inVerts[1].Tex;
    m[5].Tex = 0.25f * inVerts[2].Tex + 0.75 * inVerts[1].Tex;
    m[6].Tex = 0.75f * inVerts[1].Tex + 0.25 * inVerts[0].Tex;
    m[7].Tex = 0.5f  * inVerts[1].Tex + 0.5  * inVerts[0].Tex;
    m[8].Tex = 0.25f * inVerts[1].Tex + 0.75 * inVerts[0].Tex;
    m[9].Tex = 0.5f  * m[4].Tex + 0.5  * m[7].Tex;
    m[10].Tex = 0.5f * m[1].Tex + 0.5  * m[7].Tex;
    m[11].Tex = 0.5f * m[1].Tex + 0.5  * m[4].Tex;
 
    outVerts[0] = inVerts[0];
    outVerts[1] = m[0];
    outVerts[2] = m[1];
    outVerts[3] = m[2];
    outVerts[4] = inVerts[2];
    outVerts[5] = m[3];
    outVerts[6] = m[4];
    outVerts[7] = m[5];
    outVerts[8] = inVerts[1];
    outVerts[9] = m[6];
    outVerts[10] = m[7];
    outVerts[11] = m[8];
    outVerts[12] = m[9];
    outVerts[13] = m[10];
    outVerts[14] = m[11];
}
 
void OutputSubdivisionZero(VertexOut v[3], inout TriangleStream<GeoOut> triStream)
{
    GeoOut gout[3];
 
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        // Transorm to world space space.
        gout[i].PosW = mul(float4(v[i].PosL, 1.0f), gWorld).xyz;
        gout[i].NormalW = mul(v[i].NormalL, (float3x3)gWorldInvTranspose);
        // Transform to homogeneous clip space.
        gout[i].PosH = mul(float4(v[i].PosL, 1.0f), gWorldViewProj);
        gout[i].Tex = v[i].Tex;
    }
    [unroll]
    for (int j = 0; j < 3; ++j)
    {
        triStream.Append(gout[j]);
    }
}
void OutputSubdivision(VertexOut v[6], inout TriangleStream<GeoOut> triStream)
{
    GeoOut gout[6];
 
    [unroll]
    for (int i = 0; i < 6; ++i)
    {
        // Transorm to world space space.
        gout[i].PosW = mul(float4(v[i].PosL, 1.0f), gWorld).xyz;
        gout[i].NormalW = mul(v[i].NormalL, (float3x3)gWorldInvTranspose);
        // Transform to homogeneous clip space.
        gout[i].PosH = mul(float4(v[i].PosL, 1.0f), gWorldViewProj);
        gout[i].Tex = v[i].Tex;
    }
    [unroll]
    for (int j = 0; j < 5; ++j)
    {
        triStream.Append(gout[j]);
    }
    triStream.RestartStrip();
    triStream.Append(gout[1]);
    triStream.Append(gout[5]);
    triStream.Append(gout[3]);
}
void OutputSubdivisionTwice(VertexOut v[15], inout TriangleStream<GeoOut> triStream)
{
    GeoOut gout[15];
 
    [unroll]
    for (int i = 0; i < 15; ++i)
    {
        // Transorm to world space space.
        gout[i].PosW = mul(float4(v[i].PosL, 1.0f), gWorld).xyz;
        gout[i].NormalW = mul(v[i].NormalL, (float3x3)gWorldInvTranspose);
        // Transform to homogeneous clip space.
        gout[i].PosH = mul(float4(v[i].PosL, 1.0f), gWorldViewProj);
        gout[i].Tex = v[i].Tex;
    }
    triStream.Append(gout[0]);
    triStream.Append(gout[11]);
    triStream.Append(gout[1]);
    triStream.Append(gout[13]);
    triStream.Append(gout[2]);
    triStream.Append(gout[14]);
    triStream.Append(gout[3]);
    triStream.Append(gout[5]);
    triStream.Append(gout[4]);
    triStream.RestartStrip();
    triStream.Append(gout[11]); 
    triStream.Append(gout[10]);
    triStream.Append(gout[13]);
    triStream.Append(gout[12]);
    triStream.Append(gout[14]);
    triStream.Append(gout[6]);
    triStream.Append(gout[5]);
    triStream.RestartStrip();
    triStream.Append(gout[10]);
    triStream.Append(gout[9]);
    triStream.Append(gout[12]);
    triStream.Append(gout[7]);
    triStream.Append(gout[6]);
    triStream.RestartStrip();
    triStream.Append(gout[9]);
    triStream.Append(gout[8]);
    triStream.Append(gout[7]);
}
[maxvertexcount(24)]
void GS(triangle VertexOut gin[3], inout TriangleStream<GeoOut> triStream)
{
 
    if (length(gEyePosW) >= 30)
    {
        VertexOut v[3];
        SubdivideZero(gin, v);
        OutputSubdivisionZero(v, triStream);
    }
    else if ((length(gEyePosW) >= 15))
    {
        VertexOut v[6];
        Subdivide(gin, v);
        OutputSubdivision(v, triStream);
    }else
    {
        VertexOut v[15];
        SubdivideTwice(gin, v);
        OutputSubdivisionTwice(v, triStream);
    }
}
 
float4 PS(GeoOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform bool gAlphaClip, uniform bool gFogEnabled) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);
 
	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;
 
	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);
 
	// Normalize.
	toEye /= distToEye;
	
    // Default to multiplicative identity.
    float4 texColor = float4(1, 1, 1, 1);
    if(gUseTexure)
	{
		// Sample texture.
		texColor = gDiffuseMap.Sample( samAnisotropic, pin.Tex );
 
		if(gAlphaClip)
		{
			// Discard pixel if texture alpha < 0.1.  Note that we do this
			// test as soon as possible so that we can potentially exit the shader 
			// early, thereby skipping the rest of the shader code.
			clip(texColor.a - 0.1f);
		}
	}
	 
	//
	// Lighting.
	//
	float4 litColor = texColor;
	if( gLightCount > 0  )
	{
		// Start with a sum of zero.
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);
 
		// Sum the light contribution from each light source.  
		[unroll]
		for(int i = 0; i < gLightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye, 
				A, D, S);
			ambient += A;
			diffuse += D;
			spec    += S;
		}
		// Modulate with late add.
		litColor = texColor*(ambient + diffuse) + spec;
	}
	//
	// Fogging
	//
	if( gFogEnabled )
	{
		float fogLerp = saturate( (distToEye - gFogStart) / gFogRange ); 
		// Blend the fog color and the lit color.
		litColor = lerp(litColor, gFogColor, fogLerp);
	}
	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;
    return litColor;
}
technique11 Light1
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader( CompileShader( ps_5_0, PS(1, false, false, false) ) );
    }
}
technique11 Light2
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(2, false, false, false) ) );
    }
}
technique11 Light3
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(3, false, false, false) ) );
    }
}
technique11 Light0Tex
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(0, true, false, false) ) );
    }
}
technique11 Light1Tex
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(1, true, false, false) ) );
    }
}
technique11 Light2Tex
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(2, true, false, false) ) );
    }
}
technique11 Light3Tex
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(3, true, false, false) ) );
    }
}
technique11 Light0TexAlphaClip
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(0, true, true, false) ) );
    }
}
technique11 Light1TexAlphaClip
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(1, true, true, false) ) );
    }
}
technique11 Light2TexAlphaClip
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(2, true, true, false) ) );
    }
}
technique11 Light3TexAlphaClip
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(3, true, true, false) ) );
    }
}
technique11 Light1Fog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(1, false, false, true) ) );
    }
}
technique11 Light2Fog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(2, false, false, true) ) );
    }
}
technique11 Light3Fog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(3, false, false, true) ) );
    }
}
technique11 Light0TexFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(0, true, false, true) ) );
    }
}
technique11 Light1TexFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(1, true, false, true) ) );
    }
}
technique11 Light2TexFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(2, true, false, true) ) );
    }
}
technique11 Light3TexFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(3, true, false, true) ) );
    }
}
technique11 Light0TexAlphaClipFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(0, true, true, true) ) );
    }
}
technique11 Light1TexAlphaClipFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(1, true, true, true) ) );
    }
}
technique11 Light2TexAlphaClipFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(2, true, true, true) ) );
    }
}
technique11 Light3TexAlphaClipFog
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( CompileShader(gs_5_0, GS()) );
        SetPixelShader( CompileShader( ps_5_0, PS(3, true, true, true) ) );
    }
}
