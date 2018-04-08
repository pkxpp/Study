// KCDlgSynSpr.cpp : implementation file
//

#include "stdafx.h"
#include "SprReader.h"
#include "KCDlgSynSpr.h"
#include "KCDialogRename.h"

KStrcutSynSpr g_KSSynSpr;
// KCDlgSynSpr dialog

IMPLEMENT_DYNAMIC(KCDlgSynSpr, CDialog)

KCDlgSynSpr::KCDlgSynSpr(CWnd* pParent /*=NULL*/)
	: CDialog(KCDlgSynSpr::IDD, pParent)
	, m_nFrame(0)
	, m_Interval(_T(""))
{
	g_KSSynSpr.nFrame = 0;
	m_nFrame = 0;
	memset(&m_SprHead, 0, sizeof(m_SprHead)); 
}

KCDlgSynSpr::~KCDlgSynSpr()
{
}

void KCDlgSynSpr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);

	//Style
	DWORD dwStyle;
	dwStyle = m_ListCtrl.GetStyle();
	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT);
	//
	m_ListCtrl.InsertColumn(0, "序号", LVCFMT_LEFT, 40);
	m_ListCtrl.InsertColumn(1, "文件名", LVCFMT_LEFT, 100);
	m_ListCtrl.InsertColumn(2, "来源位置", LVCFMT_LEFT, 350);
	DDX_Text(pDX, IDC_EDITINTERVAL, m_Interval);
}


BEGIN_MESSAGE_MAP(KCDlgSynSpr, CDialog)
	ON_BN_CLICKED(IDC_BUTTONADD, &KCDlgSynSpr::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTONSYN, &KCDlgSynSpr::OnBnClickedButtonsyn)
	ON_BN_CLICKED(IDC_BUTLEAVE, &KCDlgSynSpr::OnBnClickedButleave)
	ON_BN_CLICKED(IDC_BUTTONDELETE, &KCDlgSynSpr::OnBnClickedButtonDeleteAll)
	ON_BN_CLICKED(IDC_BUTTONMove, &KCDlgSynSpr::OnBnClickedButtonMove)
	ON_BN_CLICKED(IDC_BUTTONUP, &KCDlgSynSpr::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTONSOWN, &KCDlgSynSpr::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTONRENAME, &KCDlgSynSpr::OnBnClickedButtonRename)
END_MESSAGE_MAP()


// KCDlgSynSpr message handlers


void KCDlgSynSpr::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT, _T("png (*.png)|*.png|所有文件(*.*)|*.*||"), this);

	dlg.m_ofn.nMaxFile = MAXFILENUM;
	char *pc = new char[MAXFILENUM];
	dlg.m_ofn.lpstrFile = pc;
	dlg.m_ofn.lpstrFile[0] = NULL;
	CString strFilePath;
//	MessageBox(m_FileName.GetBuffer(), "ERROR", MB_OK);
	CString strFileName = m_FileName;	//防止后续打开却没有选文件的情况
	CString strFirstName = m_FileName;
	int len = 0; // for string handle

	if(dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();
		while( NULL != pos)
		{
//			g_KSSynSpr.nFrame++;
			
			strFilePath = dlg.GetNextPathName(pos);
			//可以判断文件名字是否符合条件

			//将文件例如列表框 -- 
			int n = strFilePath.ReverseFind('\\');
			CString strPath = strFilePath.Left(n+1);	//得到路径，包括最后一个'\'
			CString strName = strFilePath.Right(strFilePath.GetAllocLength()- n-1);		// 得到去掉路径的文件全名
			/*test
//			int row = m_ListCtrl.InsertItem(m_nFrame, strPath.GetBuffer());
//			char str[4000];
//			itoa(row, str, 10);
//			MessageBox(str, "ERROR", MB_OK);
			*/
			char strTemp[32];
			itoa(m_nFrame+1, strTemp, 10);
			m_ListCtrl.InsertItem(m_nFrame, strTemp); 
			m_ListCtrl.SetItemText(m_nFrame, 1, strName.GetBuffer());
			m_ListCtrl.SetItemText(m_nFrame, 2, strPath.GetBuffer());

			m_nFrame++;									//统计文件个数（帧总数）
			if(m_nFrame == 1)
			{
				strFirstName = strFilePath;
//				MessageBox(strFirstName, "strFirstName", MB_OK);
			}
		}
//		char temp[32];
//		itoa(g_KSSynSpr.nFrame, temp, 10);
//		MessageBox(temp, "ERROR", MB_OK);


		/**
		 * （2012-09-12）为了使用第一个名字为默认的修改名字，将以下代码出现的strFilePath改为strFirstName，其他没变
		 */
		if(strFirstName.IsEmpty())
			return;

		 // 得到文件名的前缀,eg: xxx.0.png 得到.0之前的位置
		/**
		 * （2012-09-04）如果完整路径中包含有'.'的话，下面的程序有为问题
		 */
//		int len = strFilePath.Find('.');	//得到'.'之前的长度
//		strFileName = strFilePath.Left(len);

		len = strFirstName.ReverseFind('.');	// 得到xxx.n.png的第一个'.'
		strFirstName = strFirstName.Left(len);
		while(strFirstName.ReverseFind('.') > strFirstName.ReverseFind('\\'))	//路径名中还有'.'
		{
			len = strFirstName.ReverseFind('.');
			strFirstName = strFirstName.Left(len);
		}

		/**/
//		strFileName += ".spr";
//		MessageBox(strFileName.GetBuffer(), "ERROR", MB_OK);
	}

	//赋值给全局变量
	m_FileName = strFirstName;
	MessageBox(m_FileName, "m_FileName", MB_OK);
	//得到最后的文件名部分
	len = m_FileName.ReverseFind('\\');
	g_FileName = m_FileName.Right(m_FileName.GetAllocLength() - len -1);
//	MessageBox(g_FileName, "g_FileName", MB_OK);
//	g_KSSynSpr.nFrame  = ;
}

void KCDlgSynSpr::OnBnClickedButtonsyn()
{
	// TODO: Add your control notification handler code here
	SprConverter spr;
	CString strInfo;	//MessageBox显示的信息
	CString strInt;		//临时变量 -- 用来实现int to char

	// 信息准备
	strInfo = "名字或者长度存在问题：\n";
	strInfo +=  "文件名：" + m_FileName;	
	strInt.Format("\n帧总数：%d\n", m_nFrame);
	strInfo += strInt;

	// 检查名字 和 长度 
	if( m_FileName.IsEmpty() || m_nFrame == 0){
		MessageBox(strInfo.GetBuffer(), "ERROR", MB_OK);
		return;
	}

	//信息提示
	strInfo.Empty();
	strInt.Empty();
	strInfo = "合成文件名： ";
	strInfo += m_FileName + ".spr";
	strInt.Format("\n帧总数：%d\n", m_nFrame);
	strInfo += strInt;

	/// 更多信息的设置
	UpdateData(true);		// 更新窗体上的值到变量
//	MessageBox(m_Interval.GetBuffer(), "xxx", MB_OK);
	m_SprHead.Interval = atoi(m_Interval.GetBuffer());
	
	int n = MessageBox(strInfo.GetBuffer(), "提示", MB_OKCANCEL);
	if(n == IDOK){
		//开始合成
		if(spr.CreateNewSprFile(m_FileName.GetBuffer(), m_nFrame, m_SprHead))
			MessageBox("合成成功！", "ERROR", MB_OK);
		else
			MessageBox("合成失败！", "ERROR", MB_OK);

		//清零
		m_nFrame = 0;
		m_FileName.Empty();
		m_ListCtrl.DeleteAllItems();
	}
//	CDialog::OnOK();
}

void KCDlgSynSpr::OnBnClickedButleave()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void KCDlgSynSpr::OnBnClickedButtonDeleteAll()
{
	// TODO: Add your control notification handler code here
	m_nFrame = 0;
	m_FileName.Empty();
	m_ListCtrl.DeleteAllItems();
}

void KCDlgSynSpr::OnBnClickedButtonMove()
{
	// TODO: Add your control notification handler code here
	//得到选中的行号，哪个item
	for( int i = 0; i < m_ListCtrl.GetItemCount(); )
	{
		if( m_ListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			m_ListCtrl.DeleteItem(i);
			m_nFrame--;

			//可以一次删除多个选中的情况
			i = 0;
			continue;
		}
		CString strTemp;
		strTemp.Format("%d", i+1);
		m_ListCtrl.SetItemText(i, 0, strTemp);
		i++;
	}
}

void KCDlgSynSpr::OnBnClickedButtonUp()
{
	// TODO: Add your control notification handler code here
	TCHAR szBuf[MAXFILENUM];
	LVITEM lvi;
	lvi.iSubItem = 0;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = szBuf;
	lvi.cchTextMax = MAXFILENUM;
	//得到选中的行号，哪个item
	for( int i = 0; i < m_ListCtrl.GetItemCount(); i++)
	{
		if( m_ListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			// 
			if(i> 0){
				/* //这种有问题
				lvi.iItem = i;
				m_ListCtrl.GetItem(&lvi);
				m_ListCtrl.DeleteItem(i);
				lvi.iItem = i-1;
				m_ListCtrl.InsertItem(&lvi);
				*/
				CString str1, str2, str3;
				//保存
				str1 = m_ListCtrl.GetItemText(i, 0);
				str2 = m_ListCtrl.GetItemText(i, 1);
				str3 = m_ListCtrl.GetItemText(i, 2);
				//删除
				m_ListCtrl.DeleteItem(i);
				//重新插入
				str1.Format("%d", i);
				m_ListCtrl.InsertItem(i-1, str1);
				m_ListCtrl.SetItemText(i-1, 1, str2);
				m_ListCtrl.SetItemText(i-1, 2, str3);

				str1.Format("%d", i+1);
				m_ListCtrl.SetItemText(i, 0, str1);

				//仍然选中这一行
//				m_ListCtrl.SetItemState(i-1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			}
		} 
	}
}

void KCDlgSynSpr::OnBnClickedButtonDown()
{
	// TODO: Add your control notification handler code here
	for( int i = 0; i < m_ListCtrl.GetItemCount(); i++)
	{
		if( m_ListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			// 
			if(i>= 0){
				/* //这种有问题
				lvi.iItem = i;
				m_ListCtrl.GetItem(&lvi);
				m_ListCtrl.DeleteItem(i);
				lvi.iItem = i-1;
				m_ListCtrl.InsertItem(&lvi);
				*/
				CString str1, str2, str3;
				//保存
				str1 = m_ListCtrl.GetItemText(i, 0);
				str2 = m_ListCtrl.GetItemText(i, 1);
				str3 = m_ListCtrl.GetItemText(i, 2);
				//删除
				m_ListCtrl.DeleteItem(i);
				//重新插入
				str1.Format("%d", i+2);
				m_ListCtrl.InsertItem(i+1, str1);
				m_ListCtrl.SetItemText(i+1, 1, str2);
				m_ListCtrl.SetItemText(i+1, 2, str3);

				str1.Format("%d", i+1);
				m_ListCtrl.SetItemText(i, 0,  str1);
				//仍然选中这一行
//				m_ListCtrl.SetItemState(i-1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);


				//返回
				return;
			}
		}
	}
}

void KCDlgSynSpr::OnBnClickedButtonRename()
{
	// TODO: Add your control notification handler code here
	/* for test
	string OldName, NewName, StrCmd;
	OldName = "xxx.spr";
	NewName = "yyy.spr";
	StrCmd = "rename ";
	StrCmd += OldName;
	StrCmd += " ";
	StrCmd += NewName;
//	MessageBox(StrCmd.c_str(), "CMD", MB_OK);
	
	system(StrCmd.c_str());
	*/
	m_pKCDlgRename = new KCDialogRename;
	m_pKCDlgRename->m_pParent = this;
	m_pKCDlgRename->Create(KCDialogRename::IDD);

	m_pKCDlgRename->ShowWindow(SW_SHOW);
}

void KCDlgSynSpr::RenameUpdate(void)
{
		/**/
	CString NewFileName;
	string StrCmd, PreName, OldName, NewName;
	PreName = g_FileName.GetBuffer();
	int SigRename = 0;			// 存储改名是否成功
	int result = 1;
//	MessageBox(PreName.c_str(), "PreName", MB_OK);

	// 修改合成的文件名
	m_FileName = m_FileName.Left(m_FileName.ReverseFind('\\'));
	m_FileName += "\\";
	m_FileName += PreName.c_str();

	for(int i = 0; i < m_ListCtrl.GetItemCount(); i++)
	{
		/*
		CString str1, str2, str3;
		//保存
		str1 = m_ListCtrl.GetItemText(i, 0);
		str2 = m_ListCtrl.GetItemText(i, 1);
		str3 = m_ListCtrl.GetItemText(i, 2);
		*/

		// old name
		OldName = m_ListCtrl.GetItemText(i, 1);

		//new name
		NewFileName.Format("%s.%d.png", PreName.c_str(), i);
		NewName = NewFileName.GetBuffer();
//		MessageBox(NewFileName.GetBuffer(), "SHOW", MB_OK);
		StrCmd = "rename ";
		StrCmd += OldName;
		StrCmd += " ";
		StrCmd += NewName;

//		MessageBox(StrCmd.c_str(), "CMD", MB_OK);
		result = system(StrCmd.c_str());
		SigRename = (SigRename | result);
		if(result !=0)
		{
			MessageBox("改名失败！", "提示", MB_OK);
			return;
		}

		m_ListCtrl.SetItemText(i, 1, NewName.c_str());	
	}

	MessageBox("改名成功！", "提示", MB_OK);

	//修改合成的文件名
	/*
	MessageBox(m_FileName, "FileName1", MB_OK);
	int tlen = m_FileName.GetAllocLength();
	int len = m_FileName.ReverseFind('\\');
	CString StrTest;
	StrTest.Format("%d, %d", tlen, len);
	MessageBox(StrTest, "LEN", MB_OK);
	*/
}
