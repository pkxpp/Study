#include "jaumoon.h"
#include "ui_jaumoon.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <string>
#include <QTextCodec>

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
    m_iMode = 0;
    m_iOperate = 0;
    m_logFile = NULL;
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
    ui->tdirLabel->setEnabled(false);
    ui->tdirLineEdit->setEnabled(false);
    ui->tdirToolButton->setEnabled(false);
}

void JauMoon::on_sdirLineEdit_textChanged(const QString &arg1)
{
    ui->exePushButton->setEnabled(true);
}

void JauMoon::on_sdirToolButton_clicked()
{
    QString strDirectory;
    if( 0 == m_iMode)
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
}

void JauMoon::on_updateRadioButton_clicked()
{
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
    ui->exportRadioButton->setEnabled(false);
    ui->convertRadioButton->setEnabled(false);
    ui->checkbracketsRadioButton->setEnabled(false);
    ui->checkvlistRadioButton->setEnabled(false);

    ui->otherGroupBox->setEnabled(true);
    ui->delCheckBox->setEnabled(false);
    ui->otherCheckBox->setEnabled(false);
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
}

//get string from form
QString JauMoon::getStringFromForm(int intMode)
{
    QString resultString;
    //获取三种结果：0-字符串，1-文件名，2-路径名
    switch(intMode)
    {
    case 0:
        //直接从lineEdit中获得
//        resultString = ui->
        break;
    case 1:
        resultString = QFileDialog::getOpenFileName(this, tr("Open Directory"), ".", tr("Text Files(*.txt)"));
        break;
    case 2:
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
    if(0 == m_iMode)
        strDirectory = ui->tdirLineEdit->text();
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
}

void JauMoon::on_charRadioButton_clicked()
{
    m_iMode = 0;
 //   QMessageBox::information(NULL, tr("Directory"), tr("You didn't select any dirctory"));
}

void JauMoon::on_fileRadioButton_clicked()
{
    m_iMode = 1;
}

void JauMoon::on_batRadioButton_clicked()
{
    m_iMode = 2;
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

void JauMoon::on_exePushButton_clicked()
{
    //
    QMessageBox::information(NULL, tr("Test"), tr("Test"));
    char szDir[256];
	char szTmp[256] = "E:\\我的文档\\work\\TransWork\\chinese_setting";
	char *szTmp1;
    QString strDir;

	QTextCodec *codec = QTextCodec::codecForName("utf8");
	if(codec)
	{
		strDir = codec->toUnicode(QByteArray(szTmp));
	}
	strDir = tr("E:\\我的文档\\work\\TransWork\\chinese_setting");
    string strTmp = strDir.toStdString();

	szTmp1 = strDir.toAscii().data();
//    QMessageBox::information(NULL, tr("strTmp"), tr("S:")+strTmp);
    memcpy(szDir, strTmp.c_str(), sizeof(strTmp.c_str()));
    m_iFunc = 0;
    switch(m_iFunc)
    {
    case 0:
        infGetString(szDir);
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }
}


//private function
void JauMoon::funcGetString()
{

}
