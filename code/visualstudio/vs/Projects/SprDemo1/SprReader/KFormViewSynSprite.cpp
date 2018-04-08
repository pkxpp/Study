// KFormViewSynSprite.cpp : implementation file
//

#include "stdafx.h"
#include "SprReader.h"
#include "KFormViewSynSprite.h"


// KFormViewSynSprite dialog

IMPLEMENT_DYNAMIC(KFormViewSynSprite, CDialog)

KFormViewSynSprite::KFormViewSynSprite(CWnd* pParent /*=NULL*/)
	: CDialog(KFormViewSynSprite::IDD, pParent)
{

}

KFormViewSynSprite::~KFormViewSynSprite()
{
}

void KFormViewSynSprite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(KFormViewSynSprite, CDialog)
END_MESSAGE_MAP()


// KFormViewSynSprite message handlers
