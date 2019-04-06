#pragma once

#include"..\Dx11DemoBase.h"
#include<xnamath.h>
#include <d3dx11effect.h>
//#include "d3dx11effect.h"

class ColorShiftDemo : public Dx11DemoBase
{
public:
	ColorShiftDemo( );
	virtual ~ColorShiftDemo( );
	bool LoadContent( );
	void UnloadContent( );
	void Update( float dt );
	void Render( );

private:
	ID3DX11Effect* effect_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;

	ID3D11ShaderResourceView* colorMap_;
	ID3D11SamplerState* colorMapSampler_;

	XMMATRIX viewMatrix_;
	XMMATRIX projMatrix_;
};