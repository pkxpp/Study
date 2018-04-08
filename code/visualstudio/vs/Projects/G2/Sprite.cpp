// Sprite.cpp: implementation of the Sprite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sprite.h"
#include <Windows.h>
#include "Input.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
 
//
//
#define	MAX_X		700
#define MAX_Y		500
#define BULLET_H	13
#define BULLET_W	13
#define TILE_H		10	
#define TILE_W		10
/////////////////////////////////////////////////
//CSprite 
//
///////////////////////////////////////////////////
////////////////////////////////////////////////
CSprite::CSprite()
{

}

///++++++++++++++++碰撞检测
BOOL CSprite::HitTest( CSprite& sprite )
{
	if( !m_active || !sprite.m_active )
		return FALSE;//如果其中一个为无效，还许检测吗
	
	RECT rc1, rc2, temp;
	//rc1 是Sprite1 的矩形
	rc1.left = (int)m_x;
	rc1.right = rc1.left + m_width;
	rc1.top = (int)m_y;
	rc1.bottom = rc1.top + m_height;
	//rc2 是Sprite2 的矩形
	rc2.left = (int)sprite.m_x;
	rc2.right = rc2.left + sprite.m_width;
	rc2.top = (int)sprite.m_y;
	rc2.bottom = rc2.top + sprite.m_height;
//IntersectRect函数是判断矩形有无交集（return bool型）
	return IntersectRect( &temp, &rc1, &rc2 );
}

//++++++++++++++++画图
void CSprite::Draw(HDC *TMemDC,HDC *hTemp)
{
	if( !m_active )
		return;//如果已无效的
	::BitBlt(*TMemDC,0,0,m_width,m_height,*hTemp,0,0,SRCCOPY);
}
//////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////
void CExlpode::Draw(HDC *TMemDC,HDC *hTemp)
	{
	int left=m_frame*m_width;
SelectObject(*hTemp,*m_bmp);
::TransparentBlt(*TMemDC,m_x,m_y,m_width,m_height,*hTemp,left,0,m_width,m_height,RGB(255,0,255));
//0.15秒的动画
 if( timeGetTime() - m_flickerTime > 200 )
		{
		m_flickerTime = timeGetTime();
		if(++m_frame>2)
			m_frame=0;
		}

	}
////////////////////////////////////////////////
//
BOOL CExlpode::Exploding()
{//水泡时间控制
if( timeGetTime() - m_lastTime> 5000)
	{
//	m_flickerTime = timeGetTime();
	m_explode=true;
	 return true;
	}
	 return false;//如果已无效的
}
void CExlpode::Boom(HDC *TMemDC,HDC *hTemp)
{
SelectObject(*hTemp,*m_explosion);
	{
		::TransparentBlt(*TMemDC,m_x,m_y,ExplosionWidth,ExplosionHeight,*hTemp,m_frame*ExplosionWidth,4*ExplosionHeight,ExplosionWidth,ExplosionHeight,RGB(255,0,255));
		::TransparentBlt(*TMemDC,m_x,m_y-ExplosionHeight,ExplosionWidth,ExplosionHeight,*hTemp,m_frame*ExplosionWidth,0,ExplosionWidth,ExplosionHeight,RGB(255,0,255));
		::TransparentBlt(*TMemDC,m_x,m_y+ExplosionHeight,ExplosionWidth,ExplosionHeight,*hTemp,m_frame*ExplosionWidth,1*ExplosionHeight,ExplosionWidth,ExplosionHeight,RGB(255,0,255));
		::TransparentBlt(*TMemDC,m_x-ExplosionWidth,m_y,ExplosionWidth,ExplosionHeight,*hTemp,m_frame*ExplosionWidth,2*ExplosionHeight,ExplosionWidth,ExplosionHeight,RGB(255,0,255));
		::TransparentBlt(*TMemDC,m_x+ExplosionWidth,m_y,ExplosionWidth,ExplosionHeight,*hTemp,m_frame*ExplosionWidth,3*ExplosionHeight,ExplosionWidth,ExplosionHeight,RGB(255,0,255));
	}
if( timeGetTime() - m_flickerTime > 100 )
		{
		m_flickerTime = timeGetTime();
		if(++m_frame>14)
			{
				PlaySound("sound\\explode.wav",NULL,SND_FILENAME|SND_ASYNC);
				m_explode=false;
				m_active=false;
			}
		}
}
//////////////////////////////////////////////////////
//BOOL HitTest( CSprite& sprite )
//OverLoaded -----------------------
///////////////////////////////////////////////////
BOOL CExlpode::HitTest( CSprite& sprite )
{
	if( !m_active || !sprite.m_active )
		return FALSE;//如果其中一个为无效，还许检测吗
	
	  RECT rc1,rc2,rc3;
	  rc1.left=m_x-ExplosionWidth;
	  rc1.top=m_y;
	  rc1.right=m_x+ExplosionWidth*2;
	  rc1.bottom=m_y+ExplosionHeight;
									
	  rc2.left=m_x;
	  rc2.top=m_y-ExplosionHeight;
	  rc2.right=m_x+ExplosionWidth;
	  rc2.bottom=m_y+ExplosionHeight*2;
									 
	 rc3.left=sprite.m_x;
	 rc3.top=sprite.m_y;
	 rc3.right=sprite.m_x+sprite.m_width;
	 rc3.bottom=sprite.m_y+sprite.m_height;
	return (IntersectRect(&rc1,&rc1,&rc3)||IntersectRect(&rc2,&rc2,&rc3));
		 
}



//////////////////////////////////////////////////
///生育 ++CBore++ 继承了 CSprite.
///
///
////////////////////////////////////////////////////
//++++++++++++++++++++++++++++++++++++++++++++++++
//生育图的画法
void CBore::Draw(HDC *TMemDC,HDC *hTemp)
{
	int left = m_frame * m_width;
SelectObject(*hTemp,*m_bmp);
::TransparentBlt(*TMemDC,m_x,m_y,m_width,m_height,*hTemp,left,0,m_width,m_height,RGB(255,0,255));
//0.15秒的动画
 if( timeGetTime() - m_time > 100 )
	{
		m_time = timeGetTime();
		if( m_bAdvance )
		{
			if( ++m_frame > 9 )
			{
				m_frame = 1;
				m_bAdvance = FALSE;
			}
		}
		else
		{
			if( --m_frame < 0 )
			{
				m_frame = 1;
				m_bAdvance = TRUE;
			}
		}
	}
}
//////////////////////////////////////////////////
//CBullet 继承了CSprite 
//
//
//////////////////////////////////////////////////
//++++++++++++++++++++++++++++++++++++++++++
CBullet::CBullet()
{
	m_dir = DIR_UP;
	m_speed = 0;
}
/////////////////////////////////////////////
//++++++++++++++++++++++++++++++++++++++++
//子弹的移动
//对月界的子弹设为无效（del）
////////////////////////////////////////////////
BOOL CBullet::Move()
{
	if( !m_active )
		return FALSE;
	
	int offset = m_speed;
	switch( m_dir )
	{
	case DIR_UP:
		m_y -= offset;
		break;
	case DIR_DOWN:
		m_y += offset;
		break;
	case DIR_LEFT:
		m_x -= offset;
		break;
	case DIR_RIGHT:
		m_x += offset;
		break;
	}
	//如果没越界就return true
	if( m_x >= 0 && m_x + m_width <= MAX_X &&
		m_y >= 0 && m_y + m_height <= MAX_Y )
		return TRUE;
//越界就调整
	if( m_x < 0 )
		m_x = 0;
	else if( m_x + m_width > MAX_X )
		m_x = MAX_X - m_width;
	if( m_y < 0 )
		m_y = 0;
	else if( m_y + m_height > MAX_Y )
		m_y = MAX_Y - m_height;

	return FALSE;
}
//子弹的Draw ++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++
void CBullet::Draw(HDC *TMemDC,HDC *hTemp)
{

	if( !m_active )
		return;
	SelectObject(*hTemp,*m_bmp);
::TransparentBlt(*TMemDC,m_x,m_y,m_width,m_height,*hTemp,0,0,m_width,m_height,RGB(0,0,255));

}



////////////////////////////////////////
//************************************
//CTank
///////////////////////////////////////
//+++++++++++++++++++++
CTank::CTank()
{
	m_bullet[0].Create( BULLET_W, BULLET_H, FALSE );
	m_bullet[1].Create( BULLET_W, BULLET_H, FALSE );	
	m_time = 0;
	m_frame = 0;
}
//////////改变方向+++++++++++
/////////////++++++++++++等待修改数据
void CTank::ChangeDirection( DIRECTION dir )
{
	if( m_dir != dir )//如果不同方向
	{
		m_dir = dir;
		int row = (int)m_y / TILE_H;
		int col = (int)m_x / TILE_W;
		int xoffset = (int)m_x % TILE_W;
		int yoffset = (int)m_y % TILE_H;
		 
		if( xoffset <= 10 )//如果偏移小于10
			m_x = (col * TILE_W + 2);
		else if( xoffset < TILE_W - 6 )//如果偏移小于30大于10
			m_x = (col * TILE_W + 18);
		else //偏移大于30小于36
			m_x = (col * TILE_W + 34);

		if( yoffset <= 10 )
			m_y = (row * TILE_H + 2);
		else if( yoffset < TILE_H - 6 )
			m_y = (row * TILE_H + 18);
		else
 
			m_y = (row * TILE_H + 34);
	}
}
///////

//+++++++++++++++测试是否两个精灵碰撞了
BOOL CTank::HitTest( CTank& sprite, int oldx, int oldy )
{
	if( !m_active || !sprite.m_active ||
		m_bBoring || sprite.m_bBoring )
		return FALSE;
	
	int x1 = (int)sprite.m_x;
	int y1 = (int)sprite.m_y;
	int x2 = x1 + sprite.m_width;
	int y2 = y1 + sprite.m_height;
	int newx = (int)m_x;
	int newy = (int)m_y;
	switch( m_dir )
	{
	case DIR_UP:
//(newx <= x2 && newx + m_width >= x1 )
//判断在竖直方向有交集 
//oldy 是P1(本身)前一步的左上角 下面同理
		if( newx <= x2 && newx + m_width >= x1 &&
			oldy >= y2 && newy <= y2 )
			return TRUE;
		break;
	case DIR_DOWN:
		if( newx <= x2 && newx + m_width >= x1 &&
			oldy + m_height <= y1 && newy + m_height >= y1 )
			return TRUE;
		break;
	case DIR_RIGHT:
		if( newy <= y2 && newy + m_height >= y1 &&
			oldx + m_width <= x1 && newx + m_width >= x1 )
			return TRUE;
		break;
	case DIR_LEFT:
		if( newy <= y2 && newy + m_height >= y1 &&
			oldx >= x2 && newx <= x2 )
			return TRUE;
		break;
	}

	return FALSE;
}
///////
///////////////////////////////////////////
//********************************
//Ememys 类 继承了 CTank类（CBullte）(Sprite)
//////////////////////////////////////////////


////+++++++敌人就随机方向走吧
void CEnemy::ChangeDirection()
{
	CTank::ChangeDirection( (DIRECTION)(rand() % 4) );
}
//++++++++有弹吃啦。。//数据要改一下
BOOL CEnemy::Fire()
{

//如果第一颗子弹存在，而且离上次发弹时差还在150mm，就不发弹了
	if( m_bullet[0].m_active ||
		timeGetTime() - m_time < 150 )
		return FALSE;
//发弹了。。。
	m_time = timeGetTime();
	m_bullet[0].m_active = TRUE;//对一颗弹初始化
	m_bullet[0].m_speed = 6;
	m_bullet[0].m_dir = m_dir;//弹道方向还不是跟移动方向一致吗
//	m_bullet[0].m_width CTank()构造函数已初始了，因bullet->CTank->CEnemy
//而CEnemy 在 CGame 里有一个 CEnemy m_enemy[] ;所以以载入
	//调整子弹位置
	switch( m_dir )
	{ 
	case DIR_UP:
		m_bullet[0].m_x = m_x + m_width/2 - BULLET_W/2;
		m_bullet[0].m_y = m_y;
		break;
	case DIR_DOWN:
		m_bullet[0].m_x = m_x + m_width/2 - BULLET_W/2;
		m_bullet[0].m_y = m_y + m_height - BULLET_H;
		break;
	case DIR_LEFT:
		m_bullet[0].m_x = m_x;
		m_bullet[0].m_y = m_y + m_height/2 - BULLET_H/2;
		break;
	case DIR_RIGHT:
		m_bullet[0].m_x = m_x + m_width - BULLET_W;
		m_bullet[0].m_y = m_y + m_height/2 - BULLET_H/2;
	}
	return TRUE;
}
////++++++++++++++++++
//敌人重生
void CEnemy::Reborn()
{ 
	m_active = TRUE;
//临产中
	m_bBoring = TRUE;
//出生地
	m_bore.m_x = m_x - 2;
	m_bore.m_y = m_y - 2;
	m_bore.Bore();
	m_shieldTime = timeGetTime();
}

////++++++++++++++++++
//敌人的绘制函数
void CEnemy::Draw(HDC *TMemDC,HDC *hTemp)
{
	if( !m_active )
		return;//如果已无效的
//如果临产 就画临产一笔嘛
	if( m_bBoring )
	{
		m_bore.Draw(TMemDC,hTemp);
		//2秒的生育时间后 “出来了”
		if( timeGetTime() - m_shieldTime > 2000 )
			m_bBoring = FALSE;
		return;
	}

int top;
int left;
switch(m_dir)
	{
	case DIR_UP: 
		top=0;
		break;
	case DIR_DOWN:
		top=1;
		break;
	case DIR_LEFT:
		top=2;
		break;
	case DIR_RIGHT:
		top=3;
		break;
	}

left=m_frame%6;
if(m_frame==6)
	m_frame=0;


	SelectObject(*hTemp,*m_bmp);
//	::BitBlt(*TMemDC,m_x,m_y,m_width,m_height,*hTemp,left*m_width,top*m_height,SRCCOPY);
::TransparentBlt(*TMemDC,m_x,m_y,m_width,m_height,*hTemp,left*m_width,top*m_height,m_width,m_height,RGB(255,0,255));
m_bullet[0].Draw(TMemDC,hTemp);
}
/////////////////////////////////////////////////////
//CPlayer 类 玩家类
//
//////////////////////////////////////////////////////
CPlayer::CPlayer()
{
m_nLife=3;
m_nScore=0;
}
////////////////////////////////////////////
//CPlayer::Fire()
//Func:设置子弹的数据，和控制发弹间隔
///////////////////////////////////////
BOOL CPlayer::Fire()
{
//开火相隔200mm
	if( timeGetTime() - m_time < 200 )
		return FALSE;
//找到其中一个不存在的子弹	
	for( int i = 0; i < 2; i ++ )
		if( !m_bullet[i].m_active )
			break;
	if(i==2)
		return FALSE;
	m_time = timeGetTime();
	m_bullet[i].m_active = TRUE;
	m_bullet[i].m_speed = 6;
	m_bullet[i].m_dir = m_dir;
	switch( m_dir )
	{ 
	case DIR_UP:
		m_bullet[i].m_x = m_x + m_width/2 - BULLET_W/2;
		m_bullet[i].m_y = m_y;
		break;
	case DIR_DOWN:
		m_bullet[i].m_x = m_x + m_width/2 - BULLET_W/2;
		m_bullet[i].m_y = m_y + m_height - BULLET_H;
		break;
	case DIR_LEFT:
		m_bullet[i].m_x = m_x;
		m_bullet[i].m_y = m_y + m_height/2 - BULLET_H/2;
		break;
	case DIR_RIGHT:
		m_bullet[i].m_x = m_x + m_width - BULLET_W;
		m_bullet[i].m_y = m_y + m_height/2 - BULLET_H/2;
	}

	return TRUE;
}
/////////////////////////////////////////////////
//	BOOL ProcessInput( WORD input, DWORD time );
BOOL CPlayer::ProcessInput( WORD input, DWORD time )
{
		DIRECTION dir;
		if( input & KEY_UP )
			dir = DIR_UP;
		else if( input & KEY_DOWN )
			dir = DIR_DOWN;
		else if( input & KEY_LEFT )
			dir = DIR_LEFT;
		else if( input & KEY_RIGHT )
			dir = DIR_RIGHT;
	
		if( input & KEY_DIRECTION )
		{
			if( m_dir == dir )
				Move();
			else
				ChangeDirection( dir );
		}
	if( input & KEY_FIRE )
			Fire();
	if( input & KEY_SETEXPLODE )
			SetExplode();
	return TRUE;
}
///////////////////////////////////////
// 暂时没用到
//
/////////////////////////////////////////
void CPlayer::Reborn()
{ 
	m_active = TRUE;
//	m_bShield = TRUE;
	m_bBoring = TRUE;
	m_bore.m_x = m_x - 2;
	m_bore.m_y = m_y - 2;
	m_bore.Bore();
	m_shieldTime = timeGetTime();
	m_dir = DIR_UP;
	m_speed = 4;
	m_nLife=3;
}
//////////////////////////////////////
//Darw ++++++++++++++++++++++++++
//玩家的绘制
//
///////////////////////////////////////
void CPlayer::Draw(HDC *TMemDC,HDC *hTemp)
{
	if( !m_active )
		return;//如果已无效的
//如果临产 就画临产一笔嘛
	if( m_bBoring )
	{
		m_bore.Draw(TMemDC,hTemp);
		//2秒的生育时间后 “出来了”
		if( timeGetTime() - m_shieldTime > 2000 )
			m_bBoring = FALSE;
		return;
	}

int top;
int left;
switch(m_dir)
	{
	case DIR_UP: 
		top=0;
		break;
	case DIR_DOWN:
		top=1;
		break;
	case DIR_LEFT:
		top=2;
		break;
	case DIR_RIGHT:
		top=3;
		break;
	}

left=m_frame%6;
if(m_frame==6)
	m_frame=0;


	SelectObject(*hTemp,*m_bmp);
//	::BitBlt(*TMemDC,m_x,m_y,m_width,m_height,*hTemp,left*m_width,top*m_height,SRCCOPY);
::TransparentBlt(*TMemDC,m_x,m_y,m_width,m_height,*hTemp,left*m_width,top*m_height,m_width,m_height,RGB(255,0,255));
for(int j=0;j<2;j++)
m_bullet[j].Draw(TMemDC,hTemp);
}
/////////////////////////////////////////////
//
//
/////////////////////////////////////////////
BOOL CPlayer::SetExplode()
{

		int i=0;
		while(m_explode[i].m_active)
			{
				i++;
				if(i==2)
					return false;
				
			}
		m_explode[i].SetExplode(m_x+m_width/2,m_y+m_height/2);
		return true;
	
}