// Sprite.h: interface for the Sprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__E81E9131_EE3F_4106_BA18_2311413BFA2E__INCLUDED_)
#define AFX_SPRITE_H__E81E9131_EE3F_4106_BA18_2311413BFA2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////
//生育图的长和宽
#define boreWidth 56
#define boreHeight 71
#define boreWidth2 48
#define boreHeight2 64
#define ExplodeWidth 12
#define ExplodeHeight 12
#define PopoWidth	44
#define PopoHeight	41
#define ExplosionWidth 40
#define ExplosionHeight 40
//枚举方向
enum DIRECTION
{
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT
};
////////////////////////////////////////////////
//************************************
///CSprite Class 
///Comm: Base class
//////////////////////////////////////////////////
class CSprite  
{
public:
	 
	CSprite();
	 //Set m_width ,m_height and m_active 
	void Create( int w, int h, BOOL active )
	{
		m_width = w;
		m_height = h; 
		m_active = active;
	}
	 
	 //检测是否碰撞
	BOOL HitTest( CSprite& sprite );	
	 //画图
	void Draw(HDC *TMemDC,HDC *hTemp);
	//传递图片
	void BitBlt(HBITMAP *bmp){ m_bmp = bmp; }

public:
	//位图
	 HBITMAP *m_bmp;
	 //是否活动的
	BOOL m_active;
	 //位置
	int	m_x;
	 
	int	m_y;
	 //宽度和高度
	int	m_width;
	 
	int	m_height;
};
///////////////////////////////////////////////////////
//奖品 呵呵 >_< 
//
enum BONUS
{
	BONUS_LIFE,
	BONUS_CLOCK,
	BONUS_SHOE,
	BONUS_FIRESPEED,
	BONUS_FastTurtle,
	BONUS_SlowTurtle,
};

/////////////////////////////////////////////////////////
//++++++++++++++++CBounds 类 继承了 CSprite 类
//不活动类
///////////////////////////////////////////////////
class CBonus : public CSprite
{
public:
	
 

void CBonus::Draw(HDC *TMemDC,HDC *hTemp)
{
	static BOOL bShow;//上下偏移
	static m_frame=0; 
	
	if( !m_active )
		return;
	
	
	
int left = m_frame * m_width;
SelectObject(*hTemp,*m_bmp);
::TransparentBlt(*TMemDC,m_x,m_y+bShow*m_height/10,m_width,m_height,*hTemp,left,0,m_width,m_height,RGB(255,0,255));
	if( timeGetTime() - m_flickerTime > 200 )
	{
		m_flickerTime = timeGetTime();
		bShow = !bShow;
		if(++m_frame>2)
			m_frame=0;
	}
//奖品的时间
	if( timeGetTime() - m_lastTime > 15000 )
		m_active = FALSE;
}
















 //奖励类型
	BONUS	m_type;
//记录上次时间	 
	DWORD	m_lastTime;
//记录闪烁时间 
	DWORD	m_flickerTime;
};
///////////////////////////////////
//
//
///////////////////////////////////
class CExlpode : public CBonus
{
public:
	void Draw(HDC *TMemDC,HDC *hTemp);

 //
	BOOL HitTest( CSprite& sprite );
	BOOL Exploding();
	void Boom(HDC *TMemDC,HDC *hTemp);
	void BitBlt(HBITMAP *bmp,HBITMAP *bmp_explosion)
	{
		m_explosion=bmp_explosion;
		CSprite::BitBlt(bmp);
	}
	void SetExplode(int x,int y)
	{	
		m_frame=0;
		m_x=x-PopoWidth/2;
		m_y=y-PopoHeight/2;
		m_flickerTime=m_lastTime=timeGetTime();
		m_width=PopoWidth;
		m_height=PopoHeight;
		m_active=TRUE;
		m_explode=FALSE;
	}
	HBITMAP *m_explosion;
	int m_frame;
	BOOL m_explode;
};
////////////////////////////////////
//******************************
/////生育 CBore 继承了 CSprite.
/////////////////////////////
class CBore : public CSprite
{
public:
	 
	void Draw(HDC *TMemDC,HDC *hTemp);
	 
	void Bore()
	{
		m_active = TRUE;
		m_bAdvance = TRUE;
		m_frame = 0;
		m_time = timeGetTime();
	}

	//偏移
	int	  m_frame;
	//时间
	DWORD m_time;
	//高级（控制动画循环）
	BOOL  m_bAdvance;
};
/////////////////////////////////////
//************************************
//子弹类 继承 CSprite
/////////////////////////////////////
class CBullet: public CSprite
{
public: 
	CBullet();
	 
	BOOL Move();
	 
	void Draw(HDC *TMemDC,HDC *hTemp);

	//方向
	DIRECTION	m_dir;
	 //速度
	float		m_speed;
};

///////////////////////////////////////
//************************************
//坦克类 继承了CBullet（继承了CSprite）
/////////////////////////////////////////// 
class CTank: public CBullet
{
public:
	 
	CTank();
	 //创建
	void Create( int w, int h, BOOL active )
	{
		CSprite::Create( w, h, active );
		//设为有生育功能 (长和宽)
		m_bore.Create( boreWidth, boreHeight, TRUE );
	}	
	 //传递 三幅图 -》坦克，子弹，生育
	void BitBlt( HBITMAP *tank, HBITMAP *bullet, HBITMAP *bore )
	{
		m_bmp = tank;
		m_bullet[0].BitBlt( bullet );
		m_bullet[1].BitBlt( bullet );
		m_bore.BitBlt( bore );
	}
	//改变方向
	void ChangeDirection( DIRECTION dir );
	//移动和判断（m_frame is 图位移-动画效果）
	BOOL Move(){ m_frame = m_frame++; return CBullet::Move(); }
	////测试是否两个精灵碰撞了（运动中）
	BOOL HitTest( CTank& sprite, int oldx, int oldy );
	//检测与精灵碰撞
	BOOL HitTest( CSprite& sprite ){ return CSprite::HitTest(sprite); }
	//
	void Draw(HDC *TMemDC,HDC *hTemp);
	//子弹（两个）
	CBullet m_bullet[2];
	//时间
	DWORD	m_time;
	//位图偏移（主要是同方向有两图，交替使用有动画效果）
	int		m_frame;
	//最长时间
	DWORD	m_maxTime;
	//保护时间
	DWORD	m_shieldTime;
	//闪烁时间
	DWORD	m_flickerTime;
	//是否在生育
	BOOL	m_bBoring;
	//生育实例
	CBore	m_bore;
};
//////////////////////////////////////////////
///
//敌人类 继承 CTank类
//
////////////////////////////////////////////////
class CEnemy : public CTank
{
public:
	 
	void ChangeDirection();
 
	BOOL Fire();
 
	void Reborn();
 
	void Draw(HDC *TMemDC,HDC *hTemp);


 
};

//玩家类
class CPlayer : public CTank
{
public:
	CPlayer();
	void Create( int w, int h, BOOL active )
	{
		CSprite::Create( w, h, active );
		//设为有生育功能 (长和宽)
		m_bore.Create( boreWidth2, boreHeight2, TRUE );
	}	

	//处理玩家的输入
	BOOL ProcessInput( WORD input, DWORD time );
	BOOL Fire();
	BOOL SetExplode();
	
//	BOOL SetExplode();
	void Draw(HDC *TMemDC,HDC *hTemp);
	void Reborn();
	//生命
	int		m_nLife;
	//分数
	int		m_nScore;
	CExlpode m_explode[2];
};

#endif // !defined(AFX_SPRITE_H__E81E9131_EE3F_4106_BA18_2311413BFA2E__INCLUDED_)
