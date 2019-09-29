//***************************************************************************************
// RenderStates.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines render state objects.  
//***************************************************************************************

#ifndef RENDERSTATES_H
#define RENDERSTATES_H

#include "d3dUtil.h"

class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* NoCullRS;
	 
	static ID3D11BlendState* AlphaToCoverageBS;
	static ID3D11BlendState* TransparentBS;

	static ID3D11BlendState* TransparentEffectBS;

	static ID3D11BlendState* DeathColroBS;
	static ID3D11BlendState* NoRenderTargetWritesBS;

	
	// Depth/stencil states
	// exercise 10.8
	static ID3D11DepthStencilState* DeathStencilColorDSS;
	static ID3D11DepthStencilState* DeathStencilColorGreenDSS;
	// exercise 10.7
	static ID3D11DepthStencilState* DepthWriteOffDSS;
};

#endif // RENDERSTATES_H