#include "Sprite.h"
#define NUM_BITMAPS 2
#define NUM_EXPLODES 20 
#define NUM_ENEMYS 5
enum GAMESTATE
{
	GS_SPLASH,
	GS_ACTIVE,
	GS_WIN,
	GS_OVER,
};
class CGame  
{
public:
	 
	CGame();
	 
	virtual ~CGame();

	 //初始化
	BOOL Initialize(HINSTANCE hInst);
	  //初始化1
	BOOL	InitApplication();
	 //初始化（2）
	BOOL	DDInit();
	//结束时释放内存
	void	DDTerm();
	//初始化（3）
	BOOL	InitGame();
	//更新截面
	void	UpdateFrame();
	//运行游戏
	int		Run();
	//精灵爆炸
//	void	Explode( CSprite& sprite, BOOL bExplode=FALSE );
	//玩家被打
//	void	PlayerBeenHit( CPlayer& player );
	// 生奖励
	BOOL	BoreBonus();
	//生敌人
	BOOL	BoreEnemy();
	//玩家吃奖励
//	void	EatBonus( CPlayer& player );
	// 敌人吃奖励
//	void	EatBonus( CEnemy& enemy );	
	// 结束 
	void	GameOver();
	//加载图
	BOOL	LoadBitmaps();
	//打字
	void	OutputText( int x, int y, LPCTSTR string );
	//画世界图
	void	DrawWorld();
	//消息处理
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM, LPARAM );

private:
	//实体
	HINSTANCE	m_hInst;
	//窗口句柄
	HWND		m_hWnd;
	//设备环境
	HDC hdcMem,hdcBuff;
	HDC hdc;
	HDC hdcMem_Test;
	//是否活动
	BOOL		m_bActive;
	//是否显帧数
	BOOL		m_bShowStats;
	//帧数
	int frame,fps;
	//矩形
	RECT		m_rcWindow;
	 
	// 图片加载
	HBITMAP		m_pBmpList[NUM_BITMAPS];
	HBITMAP     hbm;
	HBITMAP     hTest_bmp;
	//
	BITMAP bmp,Test_bmp;

	//玩家
//	CPlayer		m_player[2];
	//单人
	BOOL m_bSingle;
	// 
//	CEnemy		m_enemy[NUM_ENEMYS];
	// 
	int			m_nMaxEnemys;
	// 
//	CExplode	m_explode[NUM_EXPLODES];
	// 
//	CBonus		m_bonus;
	// 
	GAMESTATE	m_gameState;
	// 
	DWORD		m_lastTime;
	// 
	int			m_nLevel;
	// 
	int			m_nEnemysLeft;
	// 
	int			m_nEnemys;
	// 
	BOOL		m_bEnemyLocked;
	// 
	DWORD		m_lockTime;
};
