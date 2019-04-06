#include "d3dx9.h"

#define MAX_FRAMERATE  500

#define D3DCOLOR_VALUE(a,r,g,b,color)\
	a=((float)((color&(0xff<<24))>>24))/255.0f;\
	r=((float)((color&(0xff<<16))>>16))/255.0f;\
	g=((float)((color&(0xff<<8))>>8))/255.0f;\
	b=((float)(color&(0xff)))/255.0f;\


namespace myd3d
{

	struct ColorVertex
	{
		ColorVertex(){}
		ColorVertex(float x, float y, float z, D3DCOLOR color){_x = x;_y = y;_z = z;_color = color;}

		float _x, _y, _z;
		D3DCOLOR _color;
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	};

	struct NormalVertex 
	{ 
		NormalVertex(){}
		NormalVertex(float x, float y, float z, float nx, float ny, float nz){_x = x;_y = y;_z = z;_nx = nx;_ny = ny;_nz = nz;}

		float _x, _y, _z; 
		float _nx, _ny, _nz; 
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL; 
	};

	struct TextureVertex 
	{ 
		TextureVertex(){}
		TextureVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		{_x = x;_y = y;_z = z;_nx = nx;_ny = ny;_nz = nz;_u = u;_v = v;}

		float _x, _y, _z; 
		float _nx, _ny, _nz;
		// texture coordinates
		float _u, _v; 
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1; 
	};

	D3DMATERIAL9 InitMtrl(D3DCOLOR a,D3DCOLOR d,D3DCOLOR s,D3DCOLOR e,float p);   //≤ƒ÷ ≥ı ºªØ
	
//	const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE; 

	//color
	const D3DXCOLOR WHITE( D3DCOLOR_XRGB(255, 255, 255) ); 
	const D3DXCOLOR BLACK( D3DCOLOR_XRGB( 0, 0, 0) ); 
	const D3DXCOLOR RED( D3DCOLOR_XRGB(255, 0, 0) ); 
	const D3DXCOLOR GREEN( D3DCOLOR_XRGB( 0, 255, 0) ); 
	const D3DXCOLOR BLUE( D3DCOLOR_XRGB( 0, 0, 255) ); 
	const D3DXCOLOR YELLOW( D3DCOLOR_XRGB(255, 255, 0) ); 
	const D3DXCOLOR CYAN( D3DCOLOR_XRGB( 0, 255, 255) ); 
	const D3DXCOLOR MAGENTA( D3DCOLOR_XRGB(255, 0, 255) ); 

	//materal
	const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE,WHITE,WHITE,BLACK,8.0f);

	const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED,RED,RED,BLACK,8.0f);
	const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN,GREEN,GREEN,BLACK,8.0f);
	const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE,BLUE,BLUE,BLACK,8.0f);

	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW,YELLOW,YELLOW,BLACK,8.0f);


	//functions
	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction,D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position,D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position,D3DXVECTOR3* direction,D3DXCOLOR* color);

	void ComputeNormal(D3DXVECTOR3* p0, 
		D3DXVECTOR3* p1, 
		D3DXVECTOR3* p2, 
		D3DXVECTOR3* out);

}
