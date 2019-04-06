#include "../DataExport/IReader.h"
#include "mysql/mysql.h"

struct MYSQL_DB_CFG{
	char szDBAddr[64];
	char szDBName[64];
	char szDBUser[64];
	char szDBPasswd[64];
	MYSQL* pMySQL;
};
MYSQL_DB_CFG g_DB;

int ConnectDB(MYSQL_DB_CFG &cfg)
{
	int nResult			= 0;
	int nRetCode		= 0;
	MYSQL *pRetConnect	= NULL;
	my_bool bReconnectFlag = 0;

	cfg.pMySQL = mysql_init(NULL);
	KGLOG_PROCESS_ERROR(cfg.pMySQL);
	pRetConnect = mysql_real_connect( cfg.pMySQL,
					cfg.szDBAddr,	cfg.szDBUser,
					cfg.szDBPasswd,	cfg.szDBName, 
					3306, 0, 0);
	KGLOG_PROCESS_ERROR(pRetConnect != NULL);

	bReconnectFlag = 1;
	nRetCode = mysql_options(cfg.pMySQL, MYSQL_OPT_RECONNECT, &bReconnectFlag);
	KGLOG_PROCESS_ERROR(nRetCode == 0);

	KGLogPrintf(KGLOG_INFO, "connect database %s@%s ok!", cfg.szDBName, cfg.szDBAddr);

	nRetCode = mysql_set_character_set(cfg.pMySQL, "latin1");
	KGLOG_PROCESS_ERROR((nRetCode==0)&&"Can't set database's characterset to latin1!");

	nResult = 1;
Exit0:
	return nResult;
}

int ReadFamily()
{
	char szSQL[256] = {0};
	nLen = snprintf(szSQL, sizeof(szSQL)-1,
		"select FamilyName,Account, Data, LastModify from family where LastModify > '2012-10-29'");

	IReader *g_pDataReader = GetInterface();
}

int Init()
{
	strcpy(g_DB.szDBAddr, "10.20.102.64");
	strcpy(g_DB.szDBName, "moon_test_db");
	strcpy(g_DB.szDBPasswd, "x8z5j3");
	strcpy(g_DB.szDBUser, "gateway01173");
	ConnectDB(g_DB);
}

int main()
{
	ReadFamily();
}