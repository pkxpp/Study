#include<d3d11.h>
#include<d3dx11.h>
#include"GameSprite.h"
GameSprite::GameSprite( ) : rotation_( 0 ){
	scale_.x = scale_.y = 1.0f;
}
GameSprite::~GameSprite( )
{}
XMMATRIX GameSprite::GetWorldMatrix( )
{
	XMMATRIX translation = XMMatrixTranslation( position_.x, position_.y, 0.0f);
	XMMATRIX rotationZ = XMMatrixRotationZ( rotation_ );
	XMMATRIX scale = XMMatrixScaling( scale_.x, scale_.y, 1.0f );
	return translation * rotationZ * scale;
}
void GameSprite::SetPosition( XMFLOAT2& position )
{
	position_ = position;
}
void GameSprite::SetRotation( float rotation )
{
	rotation_ = rotation;
}
void GameSprite::SetScale( XMFLOAT2& scale )
{
	scale_ = scale;
}