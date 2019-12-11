//***************************************************************************************
// RenderStates.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "RenderStates.h"

ID3D11RasterizerState*		RenderStates::WireframeRS = 0;
ID3D11RasterizerState*		RenderStates::NoCullRS    = 0;
	 
ID3D11BlendState*			RenderStates::AlphaToCoverageBS		= 0;
ID3D11BlendState*			RenderStates::TransparentBS			= 0;
ID3D11BlendState*			RenderStates::TransparentEffectBS   = 0;
ID3D11BlendState*			RenderStates::DeathColroBS			= 0;
ID3D11BlendState*			RenderStates::NoRenderTargetWritesBS	= 0;

ID3D11DepthStencilState*	RenderStates::DepthWriteOffDSS			= 0;
ID3D11DepthStencilState*	RenderStates::DeathStencilColorDSS		= 0;
ID3D11DepthStencilState*	RenderStates::DeathStencilColorGreenDSS	= 0;


void RenderStates::InitAll(ID3D11Device* device)
{
	//
	// WireframeRS
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&wireframeDesc, &WireframeRS));

	//
	// NoCullRS
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&noCullDesc, &NoCullRS));

	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = {0};
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));

	//
	// TransparentBS
	//

	D3D11_BLEND_DESC transparentDesc = {0};
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA/*D3D11_BLEND_SRC_ALPHA*/;
	transparentDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA/*D3D11_BLEND_INV_SRC_ALPHA*/;
	transparentDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	// exercise 9.6
	//transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_ALPHA;

	HR(device->CreateBlendState(&transparentDesc, &TransparentBS));

	//
	// exercise 10.7
	//
	D3D11_BLEND_DESC TransparentEffectDesc = {0};
	TransparentEffectDesc.AlphaToCoverageEnable = false;
	TransparentEffectDesc.IndependentBlendEnable = false;

	TransparentEffectDesc.RenderTarget[0].BlendEnable = true;
	TransparentEffectDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
	TransparentEffectDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
	TransparentEffectDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	TransparentEffectDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	TransparentEffectDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	TransparentEffectDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	TransparentEffectDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&transparentDesc, &TransparentEffectBS));

	//
	// 10.8 NoRenderTargetWritesBS
	//
	D3D11_BLEND_DESC noRenderTargetWritesDesc = {0};
	noRenderTargetWritesDesc.AlphaToCoverageEnable = false;
	noRenderTargetWritesDesc.IndependentBlendEnable = false;

	noRenderTargetWritesDesc.RenderTarget[0].BlendEnable    = false;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	noRenderTargetWritesDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	noRenderTargetWritesDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	noRenderTargetWritesDesc.RenderTarget[0].RenderTargetWriteMask = 0;

	HR(device->CreateBlendState(&noRenderTargetWritesDesc, &NoRenderTargetWritesBS));

	//
	// 10.8 DeathColroBS
	//
	D3D11_BLEND_DESC deathColroBSDesc = {0};
	deathColroBSDesc.AlphaToCoverageEnable = false;
	deathColroBSDesc.IndependentBlendEnable = false;

	deathColroBSDesc.RenderTarget[0].BlendEnable = true;
	deathColroBSDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA/*D3D11_BLEND_SRC_ALPHA*/;
	deathColroBSDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA/*D3D11_BLEND_INV_SRC_ALPHA*/;
	deathColroBSDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	deathColroBSDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	deathColroBSDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	deathColroBSDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_GREEN /*| D3D11_COLOR_WRITE_ENABLE_ALPHA*/;

	HR(device->CreateBlendState(&deathColroBSDesc, &DeathColroBS));
	//////////////////////////////////////////////////////////////////////////

	//
	// 10.7 DepthWriteOffDSS
	//
	D3D11_DEPTH_STENCIL_DESC depthWriteOffDesc;
	ZeroMemory(&depthWriteOffDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthWriteOffDesc.DepthEnable = true;
	depthWriteOffDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthWriteOffDesc.DepthFunc = D3D11_COMPARISON_LESS;

	HR(device->CreateDepthStencilState(&depthWriteOffDesc, &DepthWriteOffDSS));


	//
	// exercise 10.8 DeathColorBS
	//
	D3D11_DEPTH_STENCIL_DESC deathColorDSSDesc = {0};
	deathColorDSSDesc.DepthEnable      = true;
	deathColorDSSDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ZERO;
	deathColorDSSDesc.DepthFunc        = D3D11_COMPARISON_LESS; 
	deathColorDSSDesc.StencilEnable    = true;
	deathColorDSSDesc.StencilReadMask  = 0xff;
	deathColorDSSDesc.StencilWriteMask = 0xff;

	deathColorDSSDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	deathColorDSSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	deathColorDSSDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_INCR;
	deathColorDSSDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_GREATER_EQUAL;

	// We are not rendering backfacing polygons, so these settings do not matter.
	deathColorDSSDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
	deathColorDSSDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
	deathColorDSSDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_REPLACE;
	deathColorDSSDesc.BackFace.StencilFunc         = D3D11_COMPARISON_GREATER_EQUAL;

	HR(device->CreateDepthStencilState(&deathColorDSSDesc, &DeathStencilColorDSS));

	D3D11_DEPTH_STENCIL_DESC deathColorGreenDSSDesc = {0};
	deathColorGreenDSSDesc.DepthEnable      = true;
	deathColorGreenDSSDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ZERO;
	deathColorGreenDSSDesc.DepthFunc        = D3D11_COMPARISON_LESS; 
	deathColorGreenDSSDesc.StencilEnable    = true;
	deathColorGreenDSSDesc.StencilReadMask  = 0xff;
	deathColorGreenDSSDesc.StencilWriteMask = 0xff;

	deathColorGreenDSSDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	deathColorGreenDSSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	deathColorGreenDSSDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_INCR;
	deathColorGreenDSSDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// We are not rendering backfacing polygons, so these settings do not matter.
	deathColorGreenDSSDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
	deathColorGreenDSSDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
	deathColorGreenDSSDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_REPLACE;
	deathColorGreenDSSDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;

	HR(device->CreateDepthStencilState(&deathColorDSSDesc, &DeathStencilColorGreenDSS));
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(WireframeRS);
	ReleaseCOM(NoCullRS);
	ReleaseCOM(AlphaToCoverageBS);
	ReleaseCOM(TransparentBS);
	ReleaseCOM(TransparentEffectBS);
	ReleaseCOM(DeathColroBS);
	ReleaseCOM(NoRenderTargetWritesBS);

	ReleaseCOM(DeathStencilColorDSS);
	ReleaseCOM(DeathStencilColorGreenDSS);
}