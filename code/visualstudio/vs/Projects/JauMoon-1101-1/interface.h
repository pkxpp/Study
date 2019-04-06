#ifndef INTERFACE_H
#define INTERFACE_H

int infInit();
int infGetString(char *szDirectory);
int infTranslate(char *szDirectory, char *szDirectory2);
int infUpdate(char *szFileName, char *szFileName2, char *szChinese, char *szTranslate, int nOldTrans);
int infQuery(char *szChinese, int nDelete);
int infTraverse(char *szChineseFileName, char *szTransFileName, char *szOperation);
#endif // INTERFACE_H
