/********************************************************************************
** Form generated from reading UI file 'jaumoon.ui'
**
** Created: Thu Nov 1 11:03:52 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JAUMOON_H
#define UI_JAUMOON_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JauMoon
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QRadioButton *getstrRadioButton;
    QRadioButton *transRadioButton;
    QRadioButton *updateRadioButton;
    QRadioButton *querytRadioButton;
    QRadioButton *traveRadioButton;
    QGroupBox *otherGroupBox;
    QCheckBox *oldCheckBox;
    QCheckBox *delCheckBox;
    QCheckBox *otherCheckBox;
    QGroupBox *instructionGroupBox;
    QTextEdit *textEditFunction;
    QGroupBox *dirGroupBox;
    QLabel *sdirLabel;
    QLineEdit *sdirLineEdit;
    QLineEdit *tdirLineEdit;
    QLabel *tdirLabel;
    QToolButton *sdirToolButton;
    QToolButton *tdirToolButton;
    QPushButton *exePushButton;
    QPushButton *cancelPushButton;
    QTabWidget *operateTabWidget;
    QWidget *tab;
    QTextEdit *modeTextEdit;
    QRadioButton *charRadioButton;
    QRadioButton *fileRadioButton;
    QRadioButton *batRadioButton;
    QWidget *tab_2;
    QRadioButton *exportRadioButton;
    QRadioButton *checkvlistRadioButton;
    QRadioButton *convertRadioButton;
    QRadioButton *checkbracketsRadioButton;
    QTextEdit *operateTextEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *JauMoon)
    {
        if (JauMoon->objectName().isEmpty())
            JauMoon->setObjectName(QString::fromUtf8("JauMoon"));
        JauMoon->resize(639, 462);
        centralWidget = new QWidget(JauMoon);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setEnabled(true);
        groupBox->setGeometry(QRect(60, 10, 131, 151));
        getstrRadioButton = new QRadioButton(groupBox);
        getstrRadioButton->setObjectName(QString::fromUtf8("getstrRadioButton"));
        getstrRadioButton->setGeometry(QRect(20, 20, 89, 16));
        transRadioButton = new QRadioButton(groupBox);
        transRadioButton->setObjectName(QString::fromUtf8("transRadioButton"));
        transRadioButton->setGeometry(QRect(20, 40, 89, 16));
        updateRadioButton = new QRadioButton(groupBox);
        updateRadioButton->setObjectName(QString::fromUtf8("updateRadioButton"));
        updateRadioButton->setGeometry(QRect(20, 60, 89, 16));
        querytRadioButton = new QRadioButton(groupBox);
        querytRadioButton->setObjectName(QString::fromUtf8("querytRadioButton"));
        querytRadioButton->setGeometry(QRect(20, 80, 89, 16));
        traveRadioButton = new QRadioButton(groupBox);
        traveRadioButton->setObjectName(QString::fromUtf8("traveRadioButton"));
        traveRadioButton->setGeometry(QRect(20, 100, 89, 16));
        otherGroupBox = new QGroupBox(centralWidget);
        otherGroupBox->setObjectName(QString::fromUtf8("otherGroupBox"));
        otherGroupBox->setEnabled(false);
        otherGroupBox->setGeometry(QRect(210, 120, 341, 41));
        oldCheckBox = new QCheckBox(otherGroupBox);
        oldCheckBox->setObjectName(QString::fromUtf8("oldCheckBox"));
        oldCheckBox->setGeometry(QRect(10, 20, 71, 16));
        delCheckBox = new QCheckBox(otherGroupBox);
        delCheckBox->setObjectName(QString::fromUtf8("delCheckBox"));
        delCheckBox->setGeometry(QRect(90, 20, 71, 16));
        otherCheckBox = new QCheckBox(otherGroupBox);
        otherCheckBox->setObjectName(QString::fromUtf8("otherCheckBox"));
        otherCheckBox->setGeometry(QRect(170, 20, 71, 16));
        instructionGroupBox = new QGroupBox(centralWidget);
        instructionGroupBox->setObjectName(QString::fromUtf8("instructionGroupBox"));
        instructionGroupBox->setGeometry(QRect(60, 260, 491, 101));
        textEditFunction = new QTextEdit(instructionGroupBox);
        textEditFunction->setObjectName(QString::fromUtf8("textEditFunction"));
        textEditFunction->setEnabled(false);
        textEditFunction->setGeometry(QRect(10, 20, 471, 71));
        textEditFunction->setReadOnly(true);
        dirGroupBox = new QGroupBox(centralWidget);
        dirGroupBox->setObjectName(QString::fromUtf8("dirGroupBox"));
        dirGroupBox->setEnabled(false);
        dirGroupBox->setGeometry(QRect(60, 160, 491, 91));
        sdirLabel = new QLabel(dirGroupBox);
        sdirLabel->setObjectName(QString::fromUtf8("sdirLabel"));
        sdirLabel->setGeometry(QRect(20, 22, 54, 20));
        sdirLineEdit = new QLineEdit(dirGroupBox);
        sdirLineEdit->setObjectName(QString::fromUtf8("sdirLineEdit"));
        sdirLineEdit->setGeometry(QRect(70, 20, 341, 20));
        tdirLineEdit = new QLineEdit(dirGroupBox);
        tdirLineEdit->setObjectName(QString::fromUtf8("tdirLineEdit"));
        tdirLineEdit->setGeometry(QRect(70, 50, 341, 20));
        tdirLabel = new QLabel(dirGroupBox);
        tdirLabel->setObjectName(QString::fromUtf8("tdirLabel"));
        tdirLabel->setGeometry(QRect(20, 50, 54, 20));
        sdirToolButton = new QToolButton(dirGroupBox);
        sdirToolButton->setObjectName(QString::fromUtf8("sdirToolButton"));
        sdirToolButton->setGeometry(QRect(420, 20, 37, 18));
        tdirToolButton = new QToolButton(dirGroupBox);
        tdirToolButton->setObjectName(QString::fromUtf8("tdirToolButton"));
        tdirToolButton->setGeometry(QRect(420, 50, 37, 18));
        exePushButton = new QPushButton(centralWidget);
        exePushButton->setObjectName(QString::fromUtf8("exePushButton"));
        exePushButton->setEnabled(false);
        exePushButton->setGeometry(QRect(140, 370, 75, 23));
        cancelPushButton = new QPushButton(centralWidget);
        cancelPushButton->setObjectName(QString::fromUtf8("cancelPushButton"));
        cancelPushButton->setGeometry(QRect(390, 370, 75, 23));
        operateTabWidget = new QTabWidget(centralWidget);
        operateTabWidget->setObjectName(QString::fromUtf8("operateTabWidget"));
        operateTabWidget->setEnabled(false);
        operateTabWidget->setGeometry(QRect(210, 20, 341, 91));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        modeTextEdit = new QTextEdit(tab);
        modeTextEdit->setObjectName(QString::fromUtf8("modeTextEdit"));
        modeTextEdit->setEnabled(false);
        modeTextEdit->setGeometry(QRect(90, 10, 241, 51));
        modeTextEdit->setReadOnly(true);
        charRadioButton = new QRadioButton(tab);
        charRadioButton->setObjectName(QString::fromUtf8("charRadioButton"));
        charRadioButton->setGeometry(QRect(10, 10, 89, 16));
        fileRadioButton = new QRadioButton(tab);
        fileRadioButton->setObjectName(QString::fromUtf8("fileRadioButton"));
        fileRadioButton->setGeometry(QRect(10, 30, 89, 16));
        batRadioButton = new QRadioButton(tab);
        batRadioButton->setObjectName(QString::fromUtf8("batRadioButton"));
        batRadioButton->setGeometry(QRect(10, 50, 89, 16));
        operateTabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        exportRadioButton = new QRadioButton(tab_2);
        exportRadioButton->setObjectName(QString::fromUtf8("exportRadioButton"));
        exportRadioButton->setGeometry(QRect(10, 20, 61, 16));
        checkvlistRadioButton = new QRadioButton(tab_2);
        checkvlistRadioButton->setObjectName(QString::fromUtf8("checkvlistRadioButton"));
        checkvlistRadioButton->setGeometry(QRect(70, 20, 81, 16));
        convertRadioButton = new QRadioButton(tab_2);
        convertRadioButton->setObjectName(QString::fromUtf8("convertRadioButton"));
        convertRadioButton->setGeometry(QRect(10, 40, 61, 16));
        checkbracketsRadioButton = new QRadioButton(tab_2);
        checkbracketsRadioButton->setObjectName(QString::fromUtf8("checkbracketsRadioButton"));
        checkbracketsRadioButton->setGeometry(QRect(70, 40, 71, 16));
        operateTextEdit = new QTextEdit(tab_2);
        operateTextEdit->setObjectName(QString::fromUtf8("operateTextEdit"));
        operateTextEdit->setEnabled(false);
        operateTextEdit->setGeometry(QRect(160, 10, 171, 51));
        operateTextEdit->setReadOnly(true);
        operateTabWidget->addTab(tab_2, QString());
        JauMoon->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(JauMoon);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 639, 23));
        JauMoon->setMenuBar(menuBar);
        mainToolBar = new QToolBar(JauMoon);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        JauMoon->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(JauMoon);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        JauMoon->setStatusBar(statusBar);

        retranslateUi(JauMoon);

        operateTabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(JauMoon);
    } // setupUi

    void retranslateUi(QMainWindow *JauMoon)
    {
        JauMoon->setWindowTitle(QApplication::translate("JauMoon", "Jau", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("JauMoon", "\345\212\237\350\203\275", 0, QApplication::UnicodeUTF8));
        getstrRadioButton->setText(QApplication::translate("JauMoon", "1\343\200\201\346\212\275\345\217\226", 0, QApplication::UnicodeUTF8));
        transRadioButton->setText(QApplication::translate("JauMoon", "2\343\200\201\347\277\273\350\257\221", 0, QApplication::UnicodeUTF8));
        updateRadioButton->setText(QApplication::translate("JauMoon", "3\343\200\201\346\233\264\346\226\260", 0, QApplication::UnicodeUTF8));
        querytRadioButton->setText(QApplication::translate("JauMoon", "3\343\200\201\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        traveRadioButton->setText(QApplication::translate("JauMoon", "5\343\200\201\351\201\215\345\216\206", 0, QApplication::UnicodeUTF8));
        otherGroupBox->setTitle(QApplication::translate("JauMoon", "\345\205\266\344\273\226\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        oldCheckBox->setText(QApplication::translate("JauMoon", "\345\205\274\345\256\271", 0, QApplication::UnicodeUTF8));
        delCheckBox->setText(QApplication::translate("JauMoon", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        otherCheckBox->setText(QApplication::translate("JauMoon", "\345\205\266\344\273\226", 0, QApplication::UnicodeUTF8));
        instructionGroupBox->setTitle(QApplication::translate("JauMoon", "\350\257\264\346\230\216", 0, QApplication::UnicodeUTF8));
        dirGroupBox->setTitle(QApplication::translate("JauMoon", "\350\267\257\345\276\204", 0, QApplication::UnicodeUTF8));
        sdirLabel->setText(QApplication::translate("JauMoon", "\346\272\220\350\267\257\345\276\204\357\274\232", 0, QApplication::UnicodeUTF8));
        tdirLabel->setText(QApplication::translate("JauMoon", "\347\233\256\350\267\257\345\276\204\357\274\232", 0, QApplication::UnicodeUTF8));
        sdirToolButton->setText(QApplication::translate("JauMoon", "...", 0, QApplication::UnicodeUTF8));
        tdirToolButton->setText(QApplication::translate("JauMoon", "...", 0, QApplication::UnicodeUTF8));
        exePushButton->setText(QApplication::translate("JauMoon", "\346\211\247\350\241\214", 0, QApplication::UnicodeUTF8));
        cancelPushButton->setText(QApplication::translate("JauMoon", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        charRadioButton->setText(QApplication::translate("JauMoon", "\345\255\227\347\254\246\344\270\262", 0, QApplication::UnicodeUTF8));
        fileRadioButton->setText(QApplication::translate("JauMoon", "\345\215\225\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        batRadioButton->setText(QApplication::translate("JauMoon", "\346\211\271\345\244\204\347\220\206", 0, QApplication::UnicodeUTF8));
        operateTabWidget->setTabText(operateTabWidget->indexOf(tab), QApplication::translate("JauMoon", "\346\250\241\345\274\217", 0, QApplication::UnicodeUTF8));
        exportRadioButton->setText(QApplication::translate("JauMoon", "\345\257\274\345\207\272", 0, QApplication::UnicodeUTF8));
        checkvlistRadioButton->setText(QApplication::translate("JauMoon", "\346\243\200\346\237\245\346\240\274\345\274\217\344\270\262", 0, QApplication::UnicodeUTF8));
        convertRadioButton->setText(QApplication::translate("JauMoon", "\350\275\254\346\215\242", 0, QApplication::UnicodeUTF8));
        checkbracketsRadioButton->setText(QApplication::translate("JauMoon", "\346\243\200\346\237\245\346\213\254\345\217\267", 0, QApplication::UnicodeUTF8));
        operateTabWidget->setTabText(operateTabWidget->indexOf(tab_2), QApplication::translate("JauMoon", "\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class JauMoon: public Ui_JauMoon {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JAUMOON_H
