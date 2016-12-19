#include "jaumoon.h"
#include "ui_jaumoon.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <string>
#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <fstream>
#include <wchar.h>
#include <windows.h>

using namespace std;

#include "db_operate.h"
#include "str_operate.h"
#include "interface.h"

JauMoon::JauMoon(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JauMoon)
{
    ui->setupUi(this);

    //init
    m_iMode = MODE_STR;
    m_iOperate = 0;
    m_logFile = NULL;
    m_iDel = false;
    m_iOld = true;
    infInit();
}

JauMoon::~JauMoon()
{
    delete ui;
}

void JauMoon::on_radioButton_1_clicked()
{
//    lineEditSdir->show();
}

void JauMoon::on_getstrRadioButton_clicked()
{
    ui->dirGroupBox->setEnabled(true);
    ui->sdirLabel->setEnabled(true);
    ui->sdirLineEdit->setEnabled(true);
    ui->sdirToolButton->setEnabled(true);
    ui->tdirLabel->setEnabled(false);
    ui->tdirLineEdit->setEnabled(false);
    ui->tdirToolButton->setEnabled(false);

    //
    ui->operateTabWidget->setEnabled(false);

    m_iMode = MODE_DIR; //打开路径

    m_iFunc = FUNC_GETSTRING;
}

void JauMoon::on_sdirLineEdit_textChanged(const QString &arg1)
{
    ui->exePushButton->setEnabled(true);
    m_sDir = ui->sdirLineEdit->text();
}

void JauMoon::on_tdirLineEdit_textChanged(const QString &arg1)
{
    m_tDir = ui->tdirLineEdit->text();
}

void JauMoon::on_sdirToolButton_clicked()
{
    QString strDirectory;
    if( MODE_STR == m_iMode)
        strDirectory = ui->sdirLineEdit->text();
    else
        strDirectory = getStringFromForm(m_iMode);
    if(strDirectory.length() == 0)
    {
        QMessageBox::information(NULL, tr("Directory"), tr("You didn't select any dirctory"));
    }
    else
    {
//        QMessageBox::information(NULL, tr("Directory"), tr("You selected") + directory);
        ui->sdirLineEdit->setText(strDirectory);
    }
    m_sDir = strDirectory;
}

void JauMoon::on_transRadioButton_clicked()
{
    ui->dirGroupBox->setEnabled(true);
    ui->sdirLabel->setEnabled(true);
    ui->sdirLineEdit->setEnabled(true);
    ui->sdirToolButton->setEnabled(true);

    ui->tdirLabel->setEnabled(true);
    ui->tdirLineEdit->setEnabled(true);
    ui->tdirToolButton->setEnabled(true);

    ui->operateTabWidget->setEnabled(false);
    //
    m_iFunc = FUNC_TRANSLATE;
    m_iMode = MODE_DIR;
}

void JauMoon::on_updateRadioButton_clicked()
{
    //默认是字符串方式更新字典，只需要输入一个文件
    ui->dirGroupBox->setEnabled(true);
    ui->sdirLabel->setEnabled(true);
    ui->sdirLineEdit->setEnabled(true);
    ui->sdirToolButton->setEnabled(true);
    ui->tdirLabel->setEnabled(true);
    ui->tdirLineEdit->setEnabled(true);
    ui->tdirToolButton->setEnabled(true);

    ui->operateTabWidget->setEnabled(true);
    ui->operateTabWidget->setCurrentIndex(0);
    ui->charRadioButton->setEnabled(true);
    ui->fileRadioButton->setEnabled(true);
    ui->batRadioButton->setEnabled(true);
    ui->charRadioButton->setChecked(true); //默认方式
    ui->exportRadioButton->setEnabled(false);
    ui->convertRadioButton->setEnabled(false);
    ui->checkbracketsRadioButton->setEnabled(false);
    ui->checkvlistRadioButton->setEnabled(false);

    ui->otherGroupBox->setEnabled(true);
    ui->delCheckBox->setEnabled(false);
    ui->otherCheckBox->setEnabled(false);

    //
    m_iFunc = FUNC_UPDATE;
}

void JauMoon::on_querytRadioButton_clicked()
{
    ui->dirGroupBox->setEnabled(true);
    ui->tdirLabel->setEnabled(false);
    ui->tdirLineEdit->setEnabled(false);
    ui->tdirToolButton->setEnabled(false);

    ui->operateTabWidget->setEnabled(false);
    ui->otherGroupBox->setEnabled(true);
    ui->delCheckBox->setEnabled(true);
    ui->oldCheckBox->setEnabled(false);
    ui->otherCheckBox->setEnabled(false);

    //
    m_iFunc = FUNC_QUERY;
}

void JauMoon::on_traveRadioButton_clicked()
{
    ui->dirGroupBox->setEnabled(true);
    ui->sdirLabel->setEnabled(true);
    ui->sdirLineEdit->setEnabled(true);
    ui->sdirToolButton->setEnabled(true);
    ui->tdirLabel->setEnabled(true);
    ui->tdirLineEdit->setEnabled(true);
    ui->tdirToolButton->setEnabled(true);

    ui->operateTabWidget->setEnabled(true);
    ui->operateTabWidget->setCurrentIndex(1);
    ui->exportRadioButton->setEnabled(true);
    ui->convertRadioButton->setEnabled(true);
    ui->checkbracketsRadioButton->setEnabled(true);
    ui->checkvlistRadioButton->setEnabled(true);
    ui->charRadioButton->setEnabled(false);
    ui->fileRadioButton->setEnabled(false);
    ui->batRadioButton->setEnabled(false);

    ui->otherGroupBox->setEnabled(true);
    ui->delCheckBox->setEnabled(true);
    ui->oldCheckBox->setEnabled(false);
    ui->otherCheckBox->setEnabled(false);

    //
    m_iFunc = FUNC_TRAVERSE;
	m_iMode = MODE_STR;

    //默认是导出export
    ui->exportRadioButton->setChecked(true);
}

//get string from form
QString JauMoon::getStringFromForm(int intMode)
{
    QString resultString;
    //获取三种结果：0-字符串，1-文件名，2-路径名
    switch(intMode)
    {
    case MODE_STR:
        //直接从lineEdit中获得
//        resultString = ui->
        break;
    case MODE_FILE:
    case MODE_BAT:
        resultString = QFileDialog::getOpenFileName(this, tr("Open Directory"), ".", tr("Text Files(*.txt)"));
        break;
    case MODE_DIR:
        resultString = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ".");
        break;
    default:
        break;
    }

    return resultString;
}

void JauMoon::on_tdirToolButton_clicked()
{
    QString strDirectory;
    if(MODE_STR == m_iMode)
    {
        strDirectory = ui->tdirLineEdit->text();
    }
    else
        strDirectory = getStringFromForm(m_iMode);
    if(strDirectory.length() == 0)
    {
        QMessageBox::information(NULL, tr("Directory"), tr("You didn't select any dirctory"));
    }
    else
    {
//        QMessageBox::information(NULL, tr("Directory"), tr("You selected") + directory);
        ui->tdirLineEdit->setText(strDirectory);
    }
    m_tDir = strDirectory;
}

void JauMoon::on_charRadioButton_clicked()
{
    //Enable
    ui->dirGroupBox->setEnabled(true);
    ui->sdirLabel->setEnabled(true);
    ui->sdirLineEdit->setEnabled(true);
    ui->sdirToolButton->setEnabled(true);
    ui->tdirLabel->setEnabled(true);
    ui->tdirLineEdit->setEnabled(true);
    ui->tdirToolButton->setEnabled(true);

    //char
    m_iMode = MODE_STR;
 //   QMessageBox::information(NULL, tr("Directory"), tr("You didn't select any dirctory"));
}

void JauMoon::on_fileRadioButton_clicked()
{
    ui->dirGroupBox->setEnabled(true);
    ui->sdirLabel->setEnabled(true);
    ui->sdirLineEdit->setEnabled(true);
    ui->sdirToolButton->setEnabled(true);
    ui->tdirLabel->setEnabled(true);
    ui->tdirLineEdit->setEnabled(true);
    ui->tdirToolButton->setEnabled(true);
    m_iMode = MODE_FILE; //file
}

void JauMoon::on_batRadioButton_clicked()
{
    //只需要一个存放所有文件的文件
    ui->tdirLabel->setEnabled(false);
    ui->tdirLineEdit->setEnabled(false);
    ui->tdirToolButton->setEnabled(false);
    //
    m_iMode = MODE_BAT;
}

void JauMoon::on_exportRadioButton_clicked()
{
    m_iOperate = OPER_EXPORT;
}

void JauMoon::on_convertRadioButton_clicked()
{
    m_iOperate = OPER_CONVERT;
}

void JauMoon::on_checkvlistRadioButton_clicked()
{
    m_iOperate = OPER_CHECK_VLIST;
}

void JauMoon::on_checkbracketsRadioButton_clicked()
{
    m_iOperate = OPER_CHECK_BRACKETS;
}

void UTF8ToGBK(const char *pUtf8Str, int wUtf8Len,
                  char **ppGBK, int &wGBKLen)
{
    int wUnicodeLen = 0;
    wchar_t *pUnicodeStr = NULL;
    wUnicodeLen = MultiByteToWideChar(  CP_UTF8, 0,
                                        pUtf8Str, wUtf8Len,
                                        NULL, 0);

    pUnicodeStr = new wchar_t[wUnicodeLen];
    MultiByteToWideChar(  CP_UTF8, 0,
                          pUtf8Str, wUtf8Len,
                          pUnicodeStr, wUnicodeLen);

    wGBKLen=(WORD)WideCharToMultiByte(CP_ACP, 0, pUnicodeStr, wUnicodeLen,
                                      NULL,0,
                                      NULL,NULL);
    *ppGBK=new char[wGBKLen+1];
    memset(*ppGBK, 0, wGBKLen+1);
    WideCharToMultiByte(       CP_ACP, 0,
                               pUnicodeStr, -1,
                               *ppGBK,wGBKLen,
                               NULL,NULL);
    wGBKLen = strlen(*ppGBK);
    delete pUnicodeStr;
}

std::string JauMoon::ws2s(const std::wstring& ws)
{
    std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

void JauMoon::on_exePushButton_clicked()
{
    //这里的宏参考于源jau中的最大值
    char szSDir[MAX_PATH];
    char szTDir[MAX_PATH];
    char szC[MAX_CHINESE_LEN];
    char szT[MAX_CHINESE_LEN];
//    char szTmp[256];
//    char szTmp[256] = "E:\\我的文档\\work\\TransWork\\chinese_setting";
//    QString strDir;

//    strDir = tr("E:\\我的文档\\work\\TransWork\\chinese_setting");
//    QTextCodec *codec1 = QTextCodec::codecForName("GBK");
//    QString qsGBK1 = codec1->toUnicode(strDir.toLocal8Bit());
//    QByteArray qbGBK1 = codec1->fromUnicode(qsGBK1);

//    strDir = m_sDir;

    //test to check whitch character set
//    FILE *fp;
//    if((fp = fopen("Text", "wb")) == NULL)
//        return;
//    fwrite(strDir.toLocal8Bit().data(), strDir.size(), 1, fp);
//    fclose(fp);

//    ofstream of("TestCodeHex");
 //   of << std::hex << strDir;
 //   of.close();
    //test end

    //utf8-gbk
//    char *pcUtf8 = strDir.toUtf8().data();
//    char *ppGBK;
//    int iGBKLen = 0;
//    UTF8ToGBK(pcUtf8, sizeof(pcUtf8), &ppGBK, iGBKLen);

	/* 和Qt creator中一样，中文没有了
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QByteArray qbUtf8 = strDir.toLocal8Bit();
    QString qsUnicode = codec->toUnicode(qbUtf8);

    codec = QTextCodec::codecForName("GBK");
    QByteArray qbGBK = codec->fromUnicode(qsUnicode);
    QString qsGBK = QString(qbGBK);
	*/

//    wstring wstrTmp = strDir.toStdWString();
	/* 这里在vs中也显示ok
    string strTmp1;
    int nLen = WideCharToMultiByte(CP_ACP, 0, strTmp.c_str(), -1,
                                          NULL,0,
                                          NULL,NULL);
    char *pszDst = new char[nLen];
    if(NULL == pszDst) return;
    WideCharToMultiByte(CP_ACP, 0, strTmp.c_str(), -1, pszDst, nLen, NULL, NULL);
    pszDst[nLen-1] = 0;
    string strDirN = string(pszDst);
    string strDirT = ws2s(strTmp);
	*/

	/* 这地方也是QByteArray在此显示ok*/
    /*QTextCodec *codec2 = QTextCodec::codecForName("GBK");
    QByteArray qbMulti = codec2->fromUnicode(strDir);
    QString qstr1 = QString(qbMulti);
    char *wstr = qbMulti.data();
    sprintf(szDir, wstr, sizeof(wstr));*/

    // deal with sdir
    if(m_sDir.isEmpty())
    {
        QMessageBox::information(NULL, tr("Tips"), tr("sDir is Empty!"));
    }
    else{
        QTextCodec *codecs = QTextCodec::codecForName("GBK");
        QByteArray qbMultis = codecs->fromUnicode(m_sDir);
        char *wstr = qbMultis.data();
        if(MODE_STR == m_iMode)
            sprintf(szC, "%s", wstr);
        else
            sprintf(szSDir, "%s", wstr);
    }

    // deal with tdir
    if(m_tDir.isEmpty())
    {
        QMessageBox::information(NULL, tr("Tips"), tr("tDir is Empty!"));
    }
    else{
        QTextCodec *codect = QTextCodec::codecForName("GBK");
        QByteArray qbMultit = codect->fromUnicode(m_tDir);
        char *wstr = qbMultit.data();
		if(MODE_STR == m_iMode)
            sprintf(szT, "%s", wstr);
        else
            sprintf(szTDir, "%s", wstr);
    }

	/*
	* Thanks this*
	*注：在Qt creator中很多显示为乱码的地方，在vs里面都可以了。Unicode和ANSI的区别吗？
    char *sstr = "E:\\我的文档\\work\\TransWork\\chinese_setting";
    char sza[256];
    memcpy(sza, "E:\\我的文档\\work\\TransWork\\chinese_setting", sizeof("E:\\我的文档\\work\\TransWork\\chinese_setting"));
    memcpy(sza, sstr, sizeof(sstr));
	*/

	/*
    if((fp = fopen("TextCmp", "wb")) == NULL)
        return;
    fwrite(wstr, sizeof(wstr), 1, fp);
    fwrite("\n", sizeof("\n"), 1, fp);
    fwrite(sstr, sizeof(sstr), 1, fp);
    fclose(fp);
	*/

//    memcpy(szDir, strDirT.c_str(), sizeof(strTmp.c_str()));
    switch(m_iFunc)
    {
    case FUNC_GETSTRING:
        infGetString(szSDir);
        break;
    case FUNC_TRANSLATE:
        infTranslate(szSDir, szTDir);
        break;
    case FUNC_UPDATE:
        infUpdate(szSDir, szTDir, szC, szT, m_iOld, 0, m_iMode);
        break;
    case FUNC_QUERY:
        if(infQuery(szC, m_iDel)){
            QMessageBox::information(NULL, "Tips", "Success");
        }
        else
            QMessageBox::information(NULL, "Tips", "Failed");
        break;
    case FUNC_TRAVERSE:
        char szOpera[32];
        switch(m_iOperate)
        {
        case OPER_EXPORT:
            sprintf(szOpera, "%s", "export");
            break;
        case OPER_CONVERT:
            sprintf(szOpera, "%s", "convert");
            break;
        case OPER_CHECK_VLIST:
            sprintf(szOpera, "%s", "check_vlist");
            break;
        case OPER_CHECK_BRACKETS:
            sprintf(szOpera, "%s", "check_brackets");
            break;
        default:
            sprintf(szOpera, "%s", "export");
            break;
        }
        infTraverse(szC, szT, szOpera);
        break;
    default:
        break;
    }
}


//private function
void JauMoon::funcGetString()
{

}

void JauMoon::on_delCheckBox_clicked()
{
    m_iDel = true;
}

void JauMoon::on_oldCheckBox_clicked()
{
    m_iOld = true;
}
