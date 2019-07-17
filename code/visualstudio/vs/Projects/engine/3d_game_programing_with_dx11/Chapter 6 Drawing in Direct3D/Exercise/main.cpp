//***************************************************************************************
// ShapesDemo.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates drawing simple geometric primitives in wireframe mode.
//
// Controls:
//		Hold the left mouse button down and move the mouse to rotate.
//      Hold the right mouse button down to zoom in and out.
//
//***************************************************************************************

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
 
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class ExerciseApp : public D3DApp
{
public:
ExerciseApp(HINSTANCE hInstance);
~ExerciseApp();
bool Init();
void OnResize();
void UpdateScene(float dt);
void DrawScene();
void OnMouseDown(WPARAM btnState, int x, int y);
void OnMouseUp(WPARAM btnState, int x, int y);
void OnMouseMove(WPARAM btnState, int x, int y);
private:
	void BuildGeometryBuffers();
	void BuildPyramidBuffers();
	void BuildFX();
	void BuildVertexLayout();

private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;
	ID3DX11Effect* mFX;
	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;
	ID3D11InputLayout* mInputLayout;

	ID3D11RasterizerState* mWireframeRS;

	// Define transformations from local spaces to world space.
	//XMFLOAT4X4 mSphereWorld[10];
	//XMFLOAT4X4 mCylWorld[10];
	//XMFLOAT4X4 mBoxWorld;
	XMFLOAT4X4 mGridWorld;
	//XMFLOAT4X4 mCenterSphere;
	XMFLOAT4X4 mPyramidWorld;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	int mBoxVertexOffset;
	int mPyramidVertexOffset;
	int mGridVertexOffset;
	int mSphereVertexOffset;
	int mCylinderVertexOffset;

	UINT mBoxIndexOffset;
	UINT mPyramidIndexOffset;
	UINT mGridIndexOffset;
	UINT mSphereIndexOffset;
	UINT mCylinderIndexOffset;

	UINT mBoxIndexCount;
	UINT mGridIndexCount;
	UINT mSphereIndexCount;
	UINT mCylinderIndexCount;
	UINT mPyramidIndexCount;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	ExerciseApp theApp(hInstance);
	if(!theApp.Init())
		return 0;
	return theApp.Run();
} 

ExerciseApp::ExerciseApp(HINSTANCE hInstance)
: D3DApp(hInstance), mVB(0), mIB(0), mFX(0), mTech(0),
  mfxWorldViewProj(0), mInputLayout(0), mWireframeRS(0),
  mTheta(1.5f*MathHelper::Pi), mPhi(0.1f*MathHelper::Pi), mRadius(15.0f)
{
	mMainWndCaption = L"Shapes Demo";
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mGridWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	XMMATRIX pyramidScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX pyramidOffset = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&mPyramidWorld, XMMatrixMultiply(pyramidScale, pyramidOffset));
}
	
ExerciseApp::~ExerciseApp()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	ReleaseCOM(mFX);
	ReleaseCOM(mInputLayout);
	ReleaseCOM(mWireframeRS);
}

bool ExerciseApp::Init()
{
	if(!D3DApp::Init())
		return false;

	//BuildGeometryBuffers();
	BuildPyramidBuffers();
	BuildFX();
	BuildVertexLayout();

	
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	//wireframeDesc.FillMode = D3D11_FILL_SOLID; /*D3D11_FILL_WIREFRAME;*/ // Ïß¿òÄ£Ê½
	wireframeDesc.FillMode = D3D11_FILL_SOLID;
	// ²Ã¼ô
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(md3dDevice->CreateRasterizerState(&wireframeDesc, &mWireframeRS));

	return true;
}

void ExerciseApp::OnResize()
{
	D3DApp::OnResize();

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void ExerciseApp::UpdateScene(float dt)
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

void ExerciseApp::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(mInputLayout);
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	md3dImmediateContext->RSSetState(mWireframeRS);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	// Set constants
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX viewProj = view*proj;

	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the grid.
		XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
		mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&(world*viewProj)));
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);

		// Draw the pyramid.
		//world = XMLoadFloat4x4(&mPyramidWorld);
		//mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&(world*viewProj)));
		//mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		//md3dImmediateContext->DrawIndexed(mPyramidIndexCount, mPyramidIndexOffset, mPyramidVertexOffset);
	}

	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the pyramid.
		XMMATRIX world = XMLoadFloat4x4(&mPyramidWorld);
		mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&(world*viewProj)));
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mPyramidIndexCount, mPyramidIndexOffset, mPyramidVertexOffset);
	}

	HR(mSwapChain->Present(0, 0));
} 

void ExerciseApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;
	SetCapture(mhMainWnd);
} 

void ExerciseApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
} 

void ExerciseApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi   += dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi-0.1f);
	}
	else if( (btnState & MK_RBUTTON) != 0 )
	{
		// Make each pixel correspond to 0.01 unit in the scene.
		float dx = 0.01f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.01f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 3.0f, 200.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void ExerciseApp::BuildGeometryBuffers()
{
	GeometryGenerator::MeshData grid;
	GeometryGenerator geoGen;

	geoGen.CreateGrid(20.0f, 30.0f, 60, 40, grid);

	mGridVertexOffset = 0;

	mGridIndexCount = grid.Indices.size();

	// Cache the starting index for each object in the concatenated
	mGridIndexOffset = 0; /*mBoxIndexCount;*/

	UINT totalVertexCount = grid.Vertices.size();

	UINT totalIndexCount = mGridIndexCount;
	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//
	std::vector<Vertex> vecVertices(totalVertexCount);
	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);
	UINT k = 0;

	
	for(size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vecVertices[k].Pos = grid.Vertices[i].Position;
		vecVertices[k].Color = black;
	} 


	Vertex vertices[] =
	{
		{ XMFLOAT3(-3.0f, -1.5f, 0.0f)},
		{ XMFLOAT3(-2.0f, +0.5f, 0.0f)},
		{ XMFLOAT3(-1.5f, -0.5f, 0.0f)},
		{ XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{ XMFLOAT3(0.5f, -0.5f, 0.0f)},
		{ XMFLOAT3(1.5f, 0.0f, 0.0f)},
		{ XMFLOAT3(2.0f, -0.5f, 0.0f)},
		{ XMFLOAT3(3.0f, 1.5f, 0.0f)},
		{ XMFLOAT3(3.5f, 0.5f, 0.0f)}
	};

	int n1 = sizeof(vertices);
	int n2 = sizeof(Vertex);
	totalVertexCount = sizeof(vertices) / sizeof(Vertex);

	UINT indices[] = {
		0, 1, 2, 
		3, 5, 4, 
		6, 7, 8
	};	
	/*UINT indices[] = {
		0, 1, 2, 
		1, 3, 2,
		2, 3, 4,
		3, 5, 4,
		4, 5, 6,
		5, 7, 6
	};	*/
	totalIndexCount = sizeof(indices)/ sizeof(UINT);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	//vinitData.pSysMem = &vecVertices[0];
	vinitData.pSysMem = vertices;
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//
	std::vector<UINT> vecIndices;
	vecIndices.insert(vecIndices.end(), grid.Indices.begin(), grid.Indices.end());

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
	//iinitData.pSysMem = &vecIndices[0];
	iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}
 
void ExerciseApp::BuildFX()
{
	std::ifstream fin("fx/color.fxo", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
		0, md3dDevice, &mFX));

	mTech    = mFX->GetTechniqueByName("ColorTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void ExerciseApp::BuildVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
    D3DX11_PASS_DESC passDesc;
    mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &mInputLayout));
}

void ExerciseApp::BuildPyramidBuffers()
{
	GeometryGenerator::MeshData grid;
	GeometryGenerator geoGen;

	geoGen.CreateGrid(20.0f, 30.0f, 60, 40, grid);

	mGridVertexOffset = 0;
	mGridIndexCount = grid.Indices.size();

	// Cache the starting index for each object in the concatenated
	mGridIndexOffset = 0; /*mBoxIndexCount;*/

	// pyramid
	Vertex pyramidVertices[] =
	{
		{ XMFLOAT3(0.0f, 2.f, 0.0f), (const float*)&Colors::Red},
		{ XMFLOAT3(1.0f, 0.f, 0.0f), (const float*)&Colors::Green},
		{ XMFLOAT3(0.0f, 0.f, -1.0f), (const float*)&Colors::Green},
		{ XMFLOAT3(-1.0f, 0.f, 0.0f), (const float*)&Colors::Green},
		{ XMFLOAT3(0.0f, 0.f, 1.0f), (const float*)&Colors::Green}
	};

	UINT pyramidIndices[] = {
		0, 1, 2, 
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
		1, 3, 2, 
		1, 4, 3
	};

	mPyramidVertexOffset = grid.Vertices.size();
	mPyramidIndexOffset = mGridIndexCount;
	UINT pyramidVertexCount = sizeof(pyramidVertices)/sizeof(Vertex);
	mCylinderIndexCount = sizeof(pyramidIndices)/sizeof(UINT);


	UINT totalVertexCount = grid.Vertices.size() + pyramidVertexCount;
	UINT totalIndexCount = mGridIndexCount + mCylinderIndexCount;
	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//
	std::vector<Vertex> vecVertices(totalVertexCount);
	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);
	UINT k = 0;

	
	for(size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vecVertices[k].Pos = grid.Vertices[i].Position;
		vecVertices[k].Color = black;
	} 

	for(size_t i = 0; i < pyramidVertexCount; ++i, ++k)
	{
		vecVertices[k] = pyramidVertices[i];
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vecVertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//
	std::vector<UINT> vecIndices;
	vecIndices.insert(vecIndices.end(), grid.Indices.begin(), grid.Indices.end());
	vecIndices.insert(vecIndices.end(), pyramidIndices, pyramidIndices + mCylinderIndexCount);

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &vecIndices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}