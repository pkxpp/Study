//²Î¿¼£ºhttps://blog.csdn.net/csyounth/article/details/24121041
#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "RenderStates.h"

	enum RenderOptions
{
	Lit = 0,
	Lit2 = 1,
};

enum TextureOptions
{
	Wireframe = 0,
	Color = 1,
};
class Icosahedron : public D3DApp
{
public:
	Icosahedron(HINSTANCE hInstance);
	~Icosahedron();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	void BuildIcosahedronGeometryBuffers();

private:
	ID3D11Buffer* mIcosahedronVB;
	ID3D11Buffer* mIcosahedronIB;

	ID3D11ShaderResourceView* mIcosahedronMapSRV;

	DirectionalLight mDirLights[3];
	Material mIcosahedronMat;

	XMFLOAT4X4 mIcosahedronWorld;

	UINT mIcosahedronIndexCount;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	RenderOptions mRenderOptions;
	TextureOptions mTexOptions;

	XMFLOAT3 mEyePosW;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Icosahedron theApp(hInstance);

	if (!theApp.Init())
		return 0;

	return theApp.Run();
}

Icosahedron::Icosahedron(HINSTANCE hInstance)
	: D3DApp(hInstance), mIcosahedronVB(0), mIcosahedronIB(0), mEyePosW(0.0f, 0.0f, 0.0f), mRenderOptions(RenderOptions::Lit),
	mIcosahedronIndexCount(0), mTheta(1.3f*MathHelper::Pi), mPhi(0.4f*MathHelper::Pi), mRadius(80.0f), mTexOptions(TextureOptions::Color)
{
	mMainWndCaption = L"Icosahedron";
	mEnable4xMsaa = true;

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	XMMATRIX icosahedronScale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	XMMATRIX icosahedronOffset = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&mIcosahedronWorld, icosahedronScale*icosahedronOffset);

	mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	mIcosahedronMat.Ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	mIcosahedronMat.Diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	mIcosahedronMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
}

Icosahedron::~Icosahedron()
{
	md3dImmediateContext->ClearState();
	ReleaseCOM(mIcosahedronVB);
	ReleaseCOM(mIcosahedronIB);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}

bool Icosahedron::Init()
{
	if (!D3DApp::Init())
		return false;

	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);
	RenderStates::InitAll(md3dDevice);

	BuildIcosahedronGeometryBuffers();

	return true;
}

void Icosahedron::OnResize()
{
	D3DApp::OnResize();

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void Icosahedron::UpdateScene(float dt)
{
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	mEyePosW = XMFLOAT3(x, y, z);

	// Build the view matrix.
	XMVECTOR pos =	  XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up =	  XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, V);

	//
	// Switch the render mode based in key input
	//
	if (GetAsyncKeyState('1') & 0x8000)
		mRenderOptions = RenderOptions::Lit;

	if (GetAsyncKeyState('2') & 0x8000)
		mRenderOptions = RenderOptions::Lit2;

	if (GetAsyncKeyState('W') & 0x8000)
		mTexOptions = TextureOptions::Wireframe;

	if (GetAsyncKeyState('C') & 0x8000)
		mTexOptions = TextureOptions::Color;
}

void Icosahedron::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX viewProj = view*proj;

	Effects::BasicFX->SetEyePosW(mEyePosW);
	Effects::BasicFX->SetDirLight(mDirLights);

	//
	// Figure out which technique to use
	//
	ID3DX11EffectTechnique* IcosahedronTech = NULL;

	switch (mRenderOptions)
	{
	case RenderOptions::Lit:
		IcosahedronTech = Effects::BasicFX->Light1Tech;
		break;
	case RenderOptions::Lit2:
		IcosahedronTech = Effects::BasicFX->Light2Tech;
		break;
	}

	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;

	//
	// DraW the icosahedron.
	//

	IcosahedronTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{


		md3dImmediateContext->IASetVertexBuffers(0, 1, &mIcosahedronVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mIcosahedronIB, DXGI_FORMAT_R32_UINT, 0);

		// Set per object constants.
		XMMATRIX world = XMLoadFloat4x4(&mIcosahedronWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixIdentity());
		Effects::BasicFX->SetMatrial(mIcosahedronMat);
		//Effects::BasicFX->SetDiffuseMap(mIcosahedronMapSRV);

		switch (mTexOptions)
		{
		case TextureOptions::Color:
			md3dImmediateContext->RSSetState(RenderStates::NoCullRS);
			break;
		case TextureOptions::Wireframe:
			md3dImmediateContext->RSSetState(RenderStates::WireframeRS);
			break;
		}



		IcosahedronTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mIcosahedronIndexCount, 0, 0);

		// Restore default render state.
		md3dImmediateContext->RSSetState(0);
	}

	HR(mSwapChain->Present(0, 0));
}

void Icosahedron::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void Icosahedron::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Icosahedron::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quater of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restric the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.01 unit in the scene.
		float dx = 0.1f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.1f*static_cast<float>(y - mLastMousePos.y);

		//Update the camera radius based on inputs.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 10.0f, 500.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void  Icosahedron::BuildIcosahedronGeometryBuffers()
{
	GeometryGenerator::MeshData icosahedron;

	GeometryGenerator geoGen;
	geoGen.CreateGeosphere(1.0f, 0u, icosahedron);

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer
	//

	std::vector<Vertex::Basic32> vertices(icosahedron.Vertices.size());

	for (UINT i = 0; i < icosahedron.Vertices.size(); ++i)
	{
		vertices[i].Pos = icosahedron.Vertices[i].Position;
		vertices[i].Normal = icosahedron.Vertices[i].Normal;
		vertices[i].Tex = icosahedron.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * icosahedron.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mIcosahedronVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* icosahedron.Indices.size(); 
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &icosahedron.Indices[0];
	mIcosahedronIndexCount = icosahedron.Indices.size();
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIcosahedronIB));
}
