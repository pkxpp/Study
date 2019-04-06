#include "MultiTextureDemo.h"
#include <xnamath.h>

struct VertexPos
{
	XMFLOAT3 pos;
	XMFLOAT2 tex0;
};


MultiTextureDemo::MultiTextureDemo( ) : effect_( 0 ), inputLayout_( 0 ),
    vertexBuffer_( 0 ), indexBuffer_( 0 ), colorMap_( 0 ), colorMapSampler_( 0 )
{

}


MultiTextureDemo::~MultiTextureDemo( )
{}

bool MultiTextureDemo::LoadContent( )
{
	ID3DBlob* buffer = 0;
	bool compileResult = CompileD3DShader( "../BlankD3DWindow/MultiTextureDemo/TextureMap.fx", 0,"fx_5_0", &buffer );
	if( compileResult == false )
	{
		DXTRACE_MSG( "Error compiling the effect shader!" );
		return false;
	}
	HRESULT d3dResult;
	d3dResult = D3DX11CreateEffectFromMemory( buffer->GetBufferPointer( ),
		buffer->GetBufferSize( ), 0, d3dDevice_, &effect_ );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Error creating the effect shader!" );
		if( buffer )
			buffer->Release( );
		return false;
	}
	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] ={
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int totalLayoutElements = ARRAYSIZE( solidColorLayout );
    ID3DX11EffectTechnique* multiTexTechnique;
    multiTexTechnique = effect_->GetTechniqueByName( "MultiTexture" );
    ID3DX11EffectPass* effectPass = multiTexTechnique->GetPassByIndex( 0 );
	D3DX11_PASS_SHADER_DESC passDesc;
	D3DX11_EFFECT_SHADER_DESC shaderDesc;
	effectPass->GetVertexShaderDesc( &passDesc );
	passDesc.pShaderVariable->GetShaderDesc(passDesc.ShaderIndex, &shaderDesc);
	d3dResult = d3dDevice_->CreateInputLayout( solidColorLayout,
		totalLayoutElements, shaderDesc.pBytecode, shaderDesc.BytecodeLength, &inputLayout_ );
	buffer->Release( );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Error creating the input layout!" );
		return false;
	}

    VertexPos vertices[] =
	{
		{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
	};
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof( VertexPos ) * 24;
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory( &resourceData, sizeof( resourceData ) );
	resourceData.pSysMem = vertices;
	d3dResult = d3dDevice_->CreateBuffer( &vertexDesc, &resourceData, &vertexBuffer_ );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to create vertex buffer!" );
		return false;
	}

	WORD indices[] =
	{
		3, 1, 0, 2, 1, 3,
		6, 4, 5, 7, 4, 6,
		11, 9, 8, 10, 9, 11,
		14, 12, 13, 15, 12, 14,
		19, 17, 16, 18, 17, 19,
		22, 20, 21, 23, 20, 22
	};
	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory( &indexDesc, sizeof( indexDesc ) );
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof( WORD ) * 36;
	indexDesc.CPUAccessFlags = 0;
	resourceData.pSysMem = indices;
	d3dResult = d3dDevice_->CreateBuffer( &indexDesc, &resourceData, &indexBuffer_ );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to create index buffer!" );
		return false;
	}
	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_, "../BlankD3DWindow/MultiTextureDemo/decal.dds", 0, 0, &colorMap_, 0 );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to load the texture image!" );
		return false;
	}
	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_, "../BlankD3DWindow/MultiTextureDemo/decal2.dds", 0, 0, &secondMap_, 0 );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to load the texture image!" );
		return false;
	}
	D3D11_SAMPLER_DESC colorMapDesc;
	ZeroMemory( &colorMapDesc, sizeof( colorMapDesc ) );
	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;
	d3dResult = d3dDevice_->CreateSamplerState( &colorMapDesc, &colorMapSampler_ );
	if( FAILED( d3dResult ) )
	{
		DXTRACE_MSG( "Failed to create color map sampler state!" );
		return false;
	}
	
	viewMatrix_ = XMMatrixIdentity( );
	projMatrix_ = XMMatrixPerspectiveFovLH( XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f );

	return true;
}

void MultiTextureDemo::UnloadContent( )
{
	if( colorMapSampler_ ) colorMapSampler_->Release( );
	if( colorMap_ ) colorMap_->Release( );
	if( effect_ ) effect_->Release( );
	if( inputLayout_ ) inputLayout_->Release( );
	if( vertexBuffer_ ) vertexBuffer_->Release( );
	if( indexBuffer_ ) indexBuffer_->Release( );

	colorMapSampler_ = 0;
	colorMap_ = 0;
    secondMap_ = 0;
	effect_ = 0;
	inputLayout_ = 0;
	vertexBuffer_ = 0;
	indexBuffer_ = 0;
}

void MultiTextureDemo::Update( float dt )
{

}

void MultiTextureDemo::Render( )
{
	if( d3dContext_ == 0 )
		return;
	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );
	d3dContext_->ClearDepthStencilView( depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	unsigned int stride = sizeof( VertexPos );
	unsigned int offset = 0;
	d3dContext_->IASetInputLayout( inputLayout_ );
	d3dContext_->IASetVertexBuffers( 0, 1, &vertexBuffer_, &stride, &offset );
	d3dContext_->IASetIndexBuffer( indexBuffer_, DXGI_FORMAT_R16_UINT, 0 );
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw( 0.0f, 0.7f, 0.7f );
	XMMATRIX translationMat = XMMatrixTranslation( 0.0f, 0.0f, 6.0f );
	XMMATRIX worldMat = rotationMat * translationMat;
	ID3DX11EffectShaderResourceVariable* colorMap;
	colorMap = effect_->GetVariableByName( "colorMap" )->AsShaderResource( );
	colorMap->SetResource( colorMap_ );
    ID3DX11EffectShaderResourceVariable* colorMap2;
    colorMap2 = effect_->GetVariableByName( "secondMap" )->AsShaderResource( );
    colorMap2->SetResource( secondMap_ );
	ID3DX11EffectSamplerVariable* colorMapSampler;
	colorMapSampler = effect_->GetVariableByName("colorSampler")->AsSampler( );
	colorMapSampler->SetSampler( 0, colorMapSampler_ );
	ID3DX11EffectMatrixVariable* worldMatrix;
	worldMatrix = effect_->GetVariableByName( "worldMatrix" )->AsMatrix( );
	worldMatrix->SetMatrix( ( float* )&worldMat );
	ID3DX11EffectMatrixVariable* viewMatrix;
	viewMatrix = effect_->GetVariableByName( "viewMatrix" )->AsMatrix( );
	viewMatrix->SetMatrix( ( float* )&viewMatrix_ );
	ID3DX11EffectMatrixVariable* projMatrix;
	projMatrix = effect_->GetVariableByName( "projMatrix" )->AsMatrix( );
	projMatrix->SetMatrix( ( float* )&projMatrix_ );
    ID3DX11EffectTechnique* multiTexTechnique;
    multiTexTechnique = effect_->GetTechniqueByName( "MultiTexture" );

    D3DX11_TECHNIQUE_DESC techDesc;
    multiTexTechnique->GetDesc( &techDesc );

    for( unsigned int p = 0; p < techDesc.Passes; p++ )
    {
        ID3DX11EffectPass* pass = multiTexTechnique->GetPassByIndex( p );
		if( pass != 0 )
		{
			pass->Apply( 0, d3dContext_ );
			d3dContext_->DrawIndexed( 36, 0, 0 );
		}
	}
	swapChain_->Present( 0, 0 );
}