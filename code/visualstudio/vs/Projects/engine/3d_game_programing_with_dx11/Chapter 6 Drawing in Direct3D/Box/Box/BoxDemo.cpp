//*********************************************************************
// BoxDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates rendering a colored box.
//
// Controls:
// Hold the left mouse button down and move the mouse to rotate.
// Hold the right mouse button down to zoom in and out.
//
//*********************************************************************
#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"
struct Vertex
{
	XMFLOAT3 Pos;
	//XMFLOAT4 Color;
	// exersize 6.10
	XMCOLOR Color;
};

// exercise 6.2
struct Vertex1
{
	XMFLOAT3 Pos;
};

struct Vertex2
{
	XMFLOAT4 Color;
};


static D3DX11INLINE UINT ArgbToAbgr(UINT argb)
{
	BYTE A = (argb >> 24) & 0xff;
	BYTE R = (argb >> 16) & 0xff;
	BYTE G = (argb >> 8) & 0xff;
	BYTE B = (argb >> 0) & 0xff;
	return (A << 24) | (B << 16) | (G << 8) | (R << 0);
}

class BoxApp : public D3DApp
{
public:
BoxApp(HINSTANCE hInstance);
~BoxApp();
bool Init();
void OnResize();
void UpdateScene(float dt);
void DrawScene();
void OnMouseDown(WPARAM btnState, int x, int y);
void OnMouseUp(WPARAM btnState, int x, int y);
void OnMouseMove(WPARAM btnState, int x, int y);
private:
	void BuildGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();
private:
	ID3D11Buffer* mBoxVB;
	ID3D11Buffer* mBoxVB1;
	ID3D11Buffer* mBoxIB;
	ID3DX11Effect* mFX;
	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;
	ID3DX11EffectScalarVariable* mfxTime;
	ID3D11InputLayout* mInputLayout;
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	float mTheta;
	float mPhi;
	float mRadius;
	POINT mLastMousePos;

	//GameTimer m_Timer;
	DWORD64 m_dwTotalTime;

	ID3D11RasterizerState* mWireframeRS;
};
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	BoxApp theApp(hInstance);
	if(!theApp.Init())
		return 0;
	return theApp.Run();
} 

BoxApp::BoxApp(HINSTANCE hInstance)
	: D3DApp(hInstance), mBoxVB(0), mBoxVB1(0), mBoxIB(0), mFX(0), mTech(0),
	mfxWorldViewProj(0), mInputLayout(0), mfxTime(0), m_dwTotalTime(0),
	mTheta(1.5f*MathHelper::Pi), mPhi(0.25f*MathHelper::Pi), mRadius(5.0f)
	, mWireframeRS(0)
{
	mMainWndCaption = L"Box Demo";
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);
}
	
BoxApp::~BoxApp()
{
	ReleaseCOM(mBoxVB);
	ReleaseCOM(mBoxVB1);
	ReleaseCOM(mBoxIB);
	ReleaseCOM(mFX);
	ReleaseCOM(mInputLayout);

	ReleaseCOM(mWireframeRS);
}

bool BoxApp::Init()
{
	if(!D3DApp::Init())
		return false;
	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();

	// Ïß¿òÄ£Ê½
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_SOLID;
	// ²Ã¼ô
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(md3dDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));

	return true;
}

void BoxApp::OnResize()
{
	D3DApp::OnResize();
	// The window resized, so update the aspect ratio and recomputed
	// the projection matrix.
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi,
		AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void BoxApp::UpdateScene(float dt)
{
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);
	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);
} 
void BoxApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView,
		reinterpret_cast<const float*>(&Colors::Blue));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView,
		D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	md3dImmediateContext->RSSetState(mWireframeRS);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	UINT stride1 = sizeof(Vertex1);
	UINT offset1 = 0;
	UINT stride2 = sizeof(Vertex2);
	UINT offset2 = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB,
		&stride, &offset);
	/*md3dImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB,
		&stride1, &offset1);*/
	//md3dImmediateContext->IASetVertexBuffers(1, 1, &mBoxVB1,
	//	&stride2, &offset2);
	md3dImmediateContext->IASetIndexBuffer(mBoxIB,
		DXGI_FORMAT_R32_UINT, 0);
	// Set constants
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX worldViewProj = world*view*proj;
	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
	//mTimer.Tick();
	mfxTime->SetFloat(mTimer.TotalTime());
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc( &techDesc );
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		// 36 indices for the box.
		md3dImmediateContext->DrawIndexed(36, 0, 0);
	} 
	
	HR(mSwapChain->Present(0, 0));
} 

void BoxApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;
	SetCapture(mhMainWnd);
} 

void BoxApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
} 

void BoxApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(
			0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(
			0.25f*static_cast<float>(y - mLastMousePos.y));
		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;
		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi-0.1f);
	}else if( (btnState & MK_RBUTTON) != 0 )
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f*static_cast<float>(y - mLastMousePos.y);
		// Update the camera radius based on input.
		mRadius += dx - dy;
		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
	} 
	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void BoxApp::BuildGeometryBuffers()
{
	// Create vertex buffer
	/*Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&Colors::White },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&Colors::Black },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&Colors::Red },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), (const float*)&Colors::Green },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), (const float*)&Colors::Blue },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), (const float*)&Colors::Yellow },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), (const float*)&Colors::Cyan },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&Colors::Magenta }
	};*/

	// exercise 6.10
	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), ArgbToAbgr(0xFFFFFFFF) },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), ArgbToAbgr(0xFF000000) },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), ArgbToAbgr(0xFFFF0000) },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), ArgbToAbgr(0xFFFF0000) },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), ArgbToAbgr(0xFF0000FF) },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), ArgbToAbgr(0xFFFFFF00) },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), ArgbToAbgr(0xFF00FFFF) },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), ArgbToAbgr(0xFFFF00FF) }
	};
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB));

	//////////////////////////////////////////////////////////////////////////
	// exercise 6.2
	/*Vertex1 vertices1[] =
	{
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f)},
	{ XMFLOAT3(-1.0f, +1.0f, -1.0f)},
	{ XMFLOAT3(+1.0f, +1.0f, -1.0f)},
	{ XMFLOAT3(+1.0f, -1.0f, -1.0f)},
	{ XMFLOAT3(-1.0f, -1.0f, +1.0f)},
	{ XMFLOAT3(-1.0f, +1.0f, +1.0f)},
	{ XMFLOAT3(+1.0f, +1.0f, +1.0f)},
	{ XMFLOAT3(+1.0f, -1.0f, +1.0f)}
	};
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex1) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices1;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB));

	Vertex2 vertices2[] =
	{
	{ (const float*)&Colors::White },
	{ (const float*)&Colors::Black },
	{ (const float*)&Colors::Red },
	{ (const float*)&Colors::Green },
	{ (const float*)&Colors::Blue },
	{ (const float*)&Colors::Yellow },
	{ (const float*)&Colors::Cyan },
	{ (const float*)&Colors::Magenta }
	};

	D3D11_BUFFER_DESC vbd2;
	vbd2.Usage = D3D11_USAGE_IMMUTABLE;
	vbd2.ByteWidth = sizeof(Vertex2) * 8;
	vbd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd2.CPUAccessFlags = 0;
	vbd2.MiscFlags = 0;
	vbd2.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData2;
	vinitData2.pSysMem = vertices2;
	HR(md3dDevice->CreateBuffer(&vbd2, &vinitData2, &mBoxVB1));*/
	//////////////////////////////////////////////////////////////////////////

	// Create the index buffer
	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB));
} 

void BoxApp::BuildFX()
{
	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(L"FX/color.fx", 0, 0, 0,
		"fx_5_0", shaderFlags,
		0, 0, &compiledShader, &compilationMsgs, 0);
	// compilationMsgs can store errors or warnings.
	if(compilationMsgs != 0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		ReleaseCOM(compilationMsgs);
	} 
	//Even if there are no compilationMsgs, check to make sure there
	// were no other errors.
	if(FAILED(hr))
	{
		DXTrace(__FILE__, (DWORD)__LINE__, hr,
			L"D3DX11CompileFromFile", true);
	} 
	HR(D3DX11CreateEffectFromMemory(
		compiledShader->GetBufferPointer(),
		compiledShader->GetBufferSize(),
		0, md3dDevice, &mFX));
	// Done with compiled shader.
	ReleaseCOM(compiledShader);
	mTech = mFX->GetTechniqueByName("ColorTech");
	mfxWorldViewProj = mFX->GetVariableByName(
		"gWorldViewProj")->AsMatrix();
	mfxTime = mFX->GetVariableByName(
		"gTime")->AsScalar();
} 

void BoxApp::BuildVertexLayout(){
	// Create the vertex input layout.
	/*D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
	};*/

	// exercise 6.1
	//struct Vertex
	//{
	//	XMFLOAT3 Pos;
	//	XMFLOAT3 Tangent;
	//	XMFLOAT3 Normal;
	//	XMFLOAT2 Tex0;
	//	XMFLOAT2 Tex1;
	//	XMCOLOR Color;
	//};
	/*
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 52,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
	};*/

	// exercise 6.2
	/*D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,
	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};*/

	// exercise 6.10
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 2,
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout));
}