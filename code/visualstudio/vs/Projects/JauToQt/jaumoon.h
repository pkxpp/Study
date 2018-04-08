#ifndef JAUMOON_H
#define JAUMOON_H

#include <QMainWindow>
#include <stdlib.h>
#include "interface.h"

// 四种操作
#define OPER_EXPORT 1
#define OPER_CONVERT 2
#define OPER_CHECK_VLIST 3
#define OPER_CHECK_BRACKETS 4

//五项功能
#define FUNC_GETSTRING 0
#define FUNC_TRANSLATE 1
#define FUNC_UPDATE 2
#define FUNC_QUERY 3
#define FUNC_TRAVERSE 4

//update中的三种方式
#define MODE_FILE 2
#define MODE_STR 3
#define MODE_BAT 1
#define MODE_DIR 0 //路径

namespace Ui {
class JauMoon;
}

class JauMoon : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit JauMoon(QWidget *parent = 0);
    ~JauMoon();
    
private slots:
    void on_radioButton_1_clicked();

    void on_getstrRadioButton_clicked();

    void on_sdirLineEdit_textChanged(const QString &arg1);

    void on_sdirToolButton_clicked();

    void on_transRadioButton_clicked();

    void on_updateRadioButton_clicked();

    void on_querytRadioButton_clicked();

    void on_traveRadioButton_clicked();

    void on_tdirToolButton_clicked();

    void on_charRadioButton_clicked();

    void on_fileRadioButton_clicked();

    void on_batRadioButton_clicked();

    void on_exportRadioButton_clicked();

    void on_convertRadioButton_clicked();

    void on_checkvlistRadioButton_clicked();

    void on_checkbracketsRadioButton_clicked();

    void on_exePushButton_clicked();

    void on_delCheckBox_clicked();

    void on_tdirLineEdit_textChanged(const QString &arg1);

    void on_oldCheckBox_clicked();

private:
    Ui::JauMoon *ui;
    int m_iMode;
    int m_iOperate;
    int m_iFunc;
    bool m_iOld;
    bool m_iDel;
    FILE *m_logFile;
    QString m_sDir;
    QString m_tDir;
    QString getStringFromForm(int intMode);
    void funcGetString();
    std::string ws2s(const std::wstring& ws);

public:
    friend int infInit();
	friend int end_interface();
    friend int infGetString(char *szDirectory);
    friend int infTranslate(char *szDirectory, char *szDirectory2);
    friend int infUpdate(char *szFileName, char *szFileName2, char *szChinese, char *szTranslate, int nOldTrans, int	nClearTable, int nUpdateType);
    friend int infQuery(char *szChinese, int nDelete);
    friend int infTraverse(char *szChineseFileName, char *szTransFileName, char *szOperation);
};

#endif // JAUMOON_H
