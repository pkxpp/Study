// KDlgSynSprite.cpp : implementation file
//

#include "stdafx.h"
#include "SprReader.h"
#include "KDlgSynSprite.h"


// KDlgSynSprite dialog

IMPLEMENT_DYNAMIC(KDlgSynSprite, CDialog)

KDlgSynSprite::KDlgSynSprite(CWnd* pParent /*=NULL*/)
	: CDialog(KDlgSynSprite::IDD, pParent)
{

}

KDlgSynSprite::~KDlgSynSprite()
{
}

void KDlgSynSprite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KDlgSynSprite, CDialog)
END_MESSAGE_MAP()


// KDlgSynSprite message handlers
