#include "stdafx.h"
#include "structs.h"


D3DMATERIAL9 myd3d::InitMtrl(D3DCOLOR a,D3DCOLOR d,D3DCOLOR s,D3DCOLOR e,float p)
{
	D3DMATERIAL9 mtrl;
/*	mtrl.Ambient.a=((float)(a&(0xff<<24)))/255.0f;
	mtrl.Ambient.r=((float)(a&(0xff<<16)))/255.0f;
	mtrl.Ambient.g=((float)(a&(0xff<<8)))/255.0f;
	mtrl.Ambient.b=((float)(a&(0xff)))/255.0f;
	
	mtrl.Diffuse.a=((float)(d&(0xff<<24)))/255.0f;
	mtrl.Diffuse.r=((float)(d&(0xff<<16)))/255.0f;
	mtrl.Diffuse.g=((float)(d&(0xff<<8)))/255.0f;
	mtrl.Diffuse.b=((float)(d&(0xff)))/255.0f;

	mtrl.Emissive.a=((float)(e&(0xff<<24)))/255.0f;
	mtrl.Emissive.r=((float)(e&(0xff<<16)))/255.0f;
	mtrl.Emissive.g=((float)(e&(0xff<<8)))/255.0f;
	mtrl.Emissive.b=((float)(e&(0xff)))/255.0f;

	mtrl.Specular.a=((float)(s&(0xff<<24)))/255.0f;
	mtrl.Specular.r=((float)(s&(0xff<<16)))/255.0f;
	mtrl.Specular.g=((float)(s&(0xff<<8)))/255.0f;
	mtrl.Specular.b=((float)(s&(0xff)))/255.0f;*/


	D3DCOLOR_VALUE(mtrl.Ambient.a,mtrl.Ambient.r,mtrl.Ambient.g,mtrl.Ambient.b,a)

	D3DCOLOR_VALUE(mtrl.Diffuse.a,mtrl.Diffuse.r,mtrl.Diffuse.g,mtrl.Diffuse.b,d)

	D3DCOLOR_VALUE(mtrl.Emissive.a,mtrl.Emissive.r,mtrl.Emissive.g,mtrl.Emissive.b,e)

	D3DCOLOR_VALUE(mtrl.Specular.a,mtrl.Specular.r,mtrl.Specular.g,mtrl.Specular.b,s)


	mtrl.Power=p;
	return mtrl;
}

D3DLIGHT9 myd3d::InitDirectionalLight(D3DXVECTOR3* direction,D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));
	light.Type=D3DLIGHT_DIRECTIONAL;
	light.Ambient=*color*0.4f;
	light.Diffuse=*color;
	light.Specular=*color*0.6f;
	light.Direction=*direction;

	return light;
}

D3DLIGHT9 myd3d::InitPointLight(D3DXVECTOR3* position,D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));
	light.Type=D3DLIGHT_POINT;
	light.Ambient=*color*0.4f;
	light.Diffuse=*color;
	light.Specular=*color*0.6f;
	light.Position=*position;

	return light;
}

D3DLIGHT9 myd3d::InitSpotLight(D3DXVECTOR3* position,D3DXVECTOR3* direction,D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light,sizeof(light));
	light.Type=D3DLIGHT_SPOT;
	light.Ambient=*color*0.4f;
	light.Diffuse=*color;
	light.Specular=*color*0.6f;
	light.Direction=*direction;
	light.Position=*position;

	return light;
}

void myd3d::ComputeNormal(D3DXVECTOR3* p0, 
     D3DXVECTOR3* p1, 
     D3DXVECTOR3* p2, 
	 D3DXVECTOR3* out)
{
	D3DXVECTOR3 u = *p1 - *p0; 
	D3DXVECTOR3 v = *p2 - *p0; 
	D3DXVec3Cross(out, &u, &v); 
	D3DXVec3Normalize(out, out);
}