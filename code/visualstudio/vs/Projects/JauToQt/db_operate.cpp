#include 	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<assert.h>

#ifdef WIN32
#include	<WinSock.h>
#endif

#include 	"db_operate.h"
#include 	"str_operate.h"

extern void write_log(int nLogType, char* pLogFormat, ...);

#define TABLE_NAME_PREFIX "dictionary_"

const char g_szTableType[][8] = {
	"tmp",
	"trad",
	"eng",
	"viet",
	"jpz",
};

#ifdef _USE_MYSQL_
#ifdef WIN32
#include	"build_windows/lib/mysql/mysql.h"
#else
#include 	"build_linux/lib/mysql/mysql.h"
#endif

#define DATABASE_CONF "db_mysql.conf"

MYSQL*  	g_myData = NULL;	 /* the global handle to mysql server */
DataBase      	g_dbInfo =		 /* the databse configure info */
	{"", "", "", "", "", 0}; 

int
get_db_cfg(DataBase* db_info){
	FILE*	pFile = NULL;
	char	szBuffer[MAX_BUF_2048];
	int	nStrLen;
	int	nLanguageType;

	pFile = fopen(DATABASE_CONF, "r");
	if ( pFile == NULL ) {
		write_log(LT_BOTH, "Error: Can't open file: %s\n",
			  DATABASE_CONF);
		return 0;
	}

	while (fgets( szBuffer, MAX_BUF_2048, pFile ) != NULL ){
		if ( szBuffer[0] == ';' || szBuffer[0] == '\n')
			continue;

		nStrLen = (int)strlen(szBuffer);
		if ( szBuffer[nStrLen-1] == '\n' )
			szBuffer[nStrLen-1] = '\0';
		
		if ( strncmp(szBuffer, "server=", 7) == 0 ){
			strcpy( db_info->szDatabaseSvr, szBuffer+7 );
		}
		else if ( strncmp(szBuffer, "database=", 9) == 0 ){
			strcpy( db_info->szDatabase, szBuffer+9 );
		}
		else if ( strncmp(szBuffer, "user=", 5) == 0 ){
			strcpy( db_info->szUser, szBuffer+5 );
		}
		else if ( strncmp(szBuffer, "passwd=", 7) == 0 ){
			strcpy( db_info->szPasswd, szBuffer+7 );
		}
		else if ( strncmp(szBuffer, "language=", 9) == 0 ){
			nLanguageType = atoi(szBuffer+9);
			if ( nLanguageType >= 0 && nLanguageType < LANGUAGE_NUM )
				db_info->nLanguageType = nLanguageType;
				sprintf(db_info->szTable, "%s%s", TABLE_NAME_PREFIX, 
					g_szTableType[nLanguageType]);
		}
		else if ( strncmp(szBuffer, "port=", 5) == 0 ){
			db_info->nPort = atoi( szBuffer+5 );
			break;
		}
		else{
			write_log(LT_BOTH, "Error: Unknown info %s in %s!\n",
				  szBuffer, DATABASE_CONF);
		}
	}

	SAFE_CLOSE_FILE(pFile);

	if ( db_info->nPort == 0 )
		db_info->nPort = MYSQL_PORT;
	
	if ( db_info->szUser[0] == '\0'  || db_info->szPasswd[0] == '\0'      ||
	     db_info->szTable[0] == '\0' || db_info->szDatabaseSvr[0] == '\0' ||
	     db_info->szDatabase[0] == '\0' ){
		write_log(LT_BOTH, "Error: Database configure file lack of "
		"enough info about database!\n");
		return 0;
	}

	return 1;
}

int
db_init(int nClearTable){
	int 	nResult;
	char	szQureyStatement[512];

	//get database configure from configure file
	nResult = get_db_cfg( &g_dbInfo );
	if ( nResult == 0 )
		return 0;

	if ( g_myData != NULL ){
		write_log(LT_BOTH, "Error: the handle to mysql isnot empty!\n");
		return 0;
	}

	//init mysql
	if ( (g_myData = mysql_init(NULL)) == NULL ){
		write_log(LT_BOTH, "Error: Can't init mysql!\n" ) ;
		goto exit;
	}

	// connect mysql database according to the configure
	g_myData = mysql_real_connect( g_myData, g_dbInfo.szDatabaseSvr,
				       g_dbInfo.szUser, g_dbInfo.szPasswd,
				       g_dbInfo.szDatabase, g_dbInfo.nPort,
				       NULL, 0);

	if ( g_myData == NULL ){
		write_log(LT_BOTH, "Error: Can't connect to the mysql server "
		"at %s on port %d !\n",
			  g_dbInfo.szDatabaseSvr, g_dbInfo.nPort);
		goto exit;
	}

	// select which database you will use
	nResult = mysql_select_db( g_myData, g_dbInfo.szDatabase );
	if ( nResult != 0 ) {
		write_log(LT_BOTH, "Error: Can't select the %s database !\n",
			  g_dbInfo.szDatabase ) ;
		goto exit;
	}

	if ( mysql_set_character_set(g_myData, "gbk") ){
		write_log(LT_BOTH, "Error: Can't Set character set gbk!\n");
		goto exit;
	}
	sprintf( szQureyStatement, SQL_TABLE_CREATE, g_dbInfo.szTable);
	if ( mysql_query( g_myData, szQureyStatement) ){
		write_log(LT_BOTH, "Error: Can't create table %s!\n",
			  g_dbInfo.szTable);
		goto exit;
	}

	// if need clear table
	if ( nClearTable == 1 ){
		sprintf( szQureyStatement, SQL_DELETE_TABLE, g_dbInfo.szTable);
		if ( mysql_query( g_myData, szQureyStatement) ){
			write_log(LT_BOTH, "Error: Can't clear table %s!\n",
				  g_dbInfo.szTable);
			goto exit;
		}
	}

	return 1;
 exit:
	if ( g_myData != NULL ){
		mysql_close( g_myData );
		g_myData = NULL;
	}
	return 0;
}

void
db_uninit(){
	if ( g_myData != NULL ){
		mysql_close( g_myData );
		g_myData = NULL;
	}
}

int
update_trans_to_db(const char* pcszChinese, const char* pcszTrans){
	char 		      	szQueryStatement[MAX_SQL_LEN2];
	char			szChinese[MAX_CHINESE_LEN];
	char			szTranslate[MAX_TRANS_LEN];
	char			szTransConverted[MAX_TRANS_LEN];
	int			nRow_num;
	MYSQL_RES*		res = NULL;
	MYSQL_ROW		row;
	int			nRet = 0;
	
	special_sign_convert(pcszTrans, szTransConverted, g_dbInfo.nLanguageType);
	
	mysql_real_escape_string(g_myData, szChinese, pcszChinese, 
				 (int)strlen(pcszChinese));
	mysql_real_escape_string(g_myData, szTranslate, szTransConverted,
				 (int)strlen(szTransConverted));

	//下面的部分用于查询将要更新的记录是否在数据库中存在，
	//若不存在就插入，若存在就先判断翻译部分是否一样，
	//若一样则直接返回，否则直接更新
	sprintf(szQueryStatement, SQL_QUERY_CHINESE,
		g_dbInfo.szTable, szChinese);

	if ( mysql_query( g_myData, szQueryStatement ) ){
		write_log(LT_FILE, "Error: %s, ErrNo: %d\n", 
			mysql_error(g_myData), mysql_errno(g_myData));
		write_log(LT_FILE, "Failed: when query \"%s\"!\n", 
			szQueryStatement);
		goto exit;
	}

	res = mysql_store_result( g_myData );
	nRow_num = ( int )mysql_num_rows( res );
	row = mysql_fetch_row( res );

	if ( nRow_num == 0 )
	{
		nRet = insert_to_db(pcszChinese, pcszTrans);
		goto exit;
	}
	else if ( strcmp(row[1],pcszTrans) == 0 )
	{
		nRet = 1;
		goto exit;
	}

	//下面的部分用于更新指定的记录
	sprintf(szQueryStatement, SQL_UPDATE_STR,
		g_dbInfo.szTable, szTranslate, szChinese);

	if ( mysql_query(g_myData, szQueryStatement) ){
		write_log(LT_FILE, "Error: %s, ErrNo: %d\n", 
			  mysql_error(g_myData), mysql_errno(g_myData));
		write_log(LT_FILE, "Failed: when query \"%s\"!\n", 
			  szQueryStatement);
		nRet = 0;
		goto exit;
	}
	nRet = 1;
exit:
	if ( res != NULL )
		mysql_free_result( res );
	
	return nRet;
}

int
insert_to_db(const char* pChinese, const char* pTranslate){
	char 		      	szQueryStatement[MAX_SQL_LEN2];
	char			szChinese[MAX_CHINESE_LEN];
	char			szTranslate[MAX_TRANS_LEN];
	char			szTransConverted[MAX_TRANS_LEN];
	int			nRet;

	if (strlen(pChinese) >= 1000 || strlen(pTranslate) >= 4000 ){
		write_log(LT_FILE, "Too long: when insert translate!\n"
			"Chinese string: %s\nIts translate: %s\n", 
			pChinese, pTranslate);
		return 0;
	}

	special_sign_convert(pTranslate, szTransConverted, g_dbInfo.nLanguageType);
	
	mysql_real_escape_string(g_myData, szChinese,
				 pChinese, (int)strlen(pChinese));
	mysql_real_escape_string(g_myData, szTranslate,
				 szTransConverted, (int)strlen(szTransConverted));
	sprintf(szQueryStatement, SQL_INSERT_STR,
		g_dbInfo.szTable, szChinese, szTranslate);

	nRet = mysql_query(g_myData, szQueryStatement);
	if ( nRet == 0 )
		return 1;

	nRet = mysql_errno(g_myData);
	if ( nRet == 1062 ){
		nRet = get_trans_from_db((char*)pChinese, szTranslate,
					 MAX_TRANS_LEN);
		if ( nRet == 0 ){
			write_log(LT_FILE, "Can't get the translate of [%s],"
				  "when insert it to db!", pChinese);
			return 0;
		}
		else
			return 2;
	}
	else{
		write_log(LT_FILE, "Error: %s, ErrNo: %d\n",
			  mysql_error(g_myData), nRet);
		write_log(LT_FILE, "Failed: when query \"%s\"!\n",
			  szQueryStatement);
	}

	return 0;
}

int
insert_notrans_to_db(const char* pcszChinese){
	char 		      	szQueryStatement[MAX_SQL_LEN1];
	char			szChinese[MAX_CHINESE_LEN];
	int			nRet;

	if ( strlen(pcszChinese) >= 1000 ){
		write_log(LT_FILE, "Too long when insert no translate \n"
			  "Chinese string: %s\n",	pcszChinese);
		return 0;
	}

	mysql_real_escape_string(g_myData, szChinese,
				 pcszChinese, (int)strlen(pcszChinese));
	sprintf(szQueryStatement, SQL_INSERT_STR,
		g_dbInfo.szTable, szChinese, "");

	nRet = mysql_query(g_myData, szQueryStatement);
	if ( nRet != 0 ){
		nRet = mysql_errno(g_myData);
		if ( nRet != 1062  ){
			write_log(LT_FILE, "Error: %s, ErrNo: %d\n", 
				  mysql_error(g_myData), nRet);
			write_log(LT_FILE, "Failed: when query \"%s\"!\n",
				  szQueryStatement);
			return 0;
		}

		return 2;
	}
	//here the string is first inserted to database,
	//judge there is gbk,if not, translate it automatic
	nRet = is_there_gbk_code(pcszChinese);
	if ( nRet == 0 ){
		update_trans_to_db(pcszChinese, pcszChinese);
		return 3;
	}

	return 1;
}

int
get_trans_from_db(char* pChinese, char* pTranslate, int nTransSize){
	MYSQL_RES*	      	res;
	MYSQL_ROW		row;
	char 		      	szQueryStatement[MAX_SQL_LEN1];
	char			szChinese[MAX_CHINESE_LEN];
	int			nStrLen;

	if (strlen(pChinese) >= 1000 ){
		write_log(LT_FILE, "Too long when get translate\n"
			"Chinese string: %s\n", pChinese);
		return 0;
	}

	mysql_real_escape_string(g_myData, szChinese, 
				 pChinese, (int)strlen(pChinese));
	sprintf(szQueryStatement, SQL_QUERY_CHINESE,
		g_dbInfo.szTable, szChinese);

	if ( mysql_query( g_myData, szQueryStatement ) ){
		write_log(LT_FILE, "Error: %s, ErrNo: %d\n", 
			  mysql_error(g_myData), mysql_errno(g_myData));
		write_log(LT_FILE, "Failed: when query \"%s\"!\n",
			  szQueryStatement);
		return 0;
	}

	res = mysql_store_result( g_myData );
	row = mysql_fetch_row(res);
	if ( row == NULL ){
		write_log(LT_FILE, "Error: Can't fetch result row,"
				   "when query \"%s\"!\n",
			  szQueryStatement);
		return 0;
	}
	nStrLen = (int)strlen(row[1]);

	if ( nStrLen < 1 )
	{
		write_log(LT_FILE, "Error: the string of [%s] havn't translated!\n",
			pChinese);
		return 0;
	}

	if ( nStrLen >= nTransSize ){
		write_log(LT_FILE, "Error: the length of translate [%s] "
				   "exceed the max length:%d\n", pChinese,
			  nTransSize);
		nStrLen = nTransSize - 1;
	}

	strncpy(pTranslate, row[1], nStrLen);
	pTranslate[nStrLen] = '\0';

	return 1;
}

int
query_record_from_db(const char* pcszChinese, int nDelete){
	MYSQL_RES*	res;
	MYSQL_ROW	row;
	char 		szQueryStatement[MAX_SQL_LEN2];
	char		szChinese[MAX_CHINESE_LEN];

	if (strlen(pcszChinese) >= 1000 ){
		write_log(LT_FILE, "Too long when get translate\n"
			"Chinese string: %s\n", pcszChinese);
		return 0;
	}

	mysql_real_escape_string(g_myData, szChinese, 
				 pcszChinese, (int)strlen(pcszChinese));
	sprintf(szQueryStatement, SQL_QUERY_CHINESE,
		g_dbInfo.szTable, szChinese);

	if ( mysql_query( g_myData, szQueryStatement )){
		write_log(LT_BOTH, "Error: %s, ErrNo: %d\n", 
			  mysql_error(g_myData), mysql_errno(g_myData));
		write_log(LT_BOTH, "Failed: when query \"%s\"!\n", 
			  szQueryStatement);
		return 0;
	}

	res = mysql_store_result( g_myData );
	row = mysql_fetch_row( res );
	if ( row == NULL ){
		write_log(LT_BOTH, "Error: Can't fetch result row,"
				   "when query \"%s\"!\n",
			  szQueryStatement);
		return 0;
	}

	if ( row[1][0] != '\0' )
		write_log(LT_BOTH, "Its translate: %s", row[1]);

	mysql_free_result( res );

	if ( nDelete == 1 ){
		sprintf(szQueryStatement, SQL_DELETE_STR,
			g_dbInfo.szTable, szChinese);

	       	if ( mysql_query( g_myData, szQueryStatement )){
			write_log(LT_BOTH, "Error: %s, ErrNo: %d\n", 
				  mysql_error(g_myData), mysql_errno(g_myData));
			write_log(LT_BOTH, "Failed: when delete \"%s\"!\n", 
				  szQueryStatement);
			return 0;
		}
	}		

	return 1;
}
/* here are end of #ifdef _USE_MYSQL_ */
#else  /* follow code use berkeley db as database*/

#ifdef WIN32
#include 	"build_windows/lib/bdb/db.h"
#else
#include	"build_linux/lib/bdb/db.h"
#endif

DB* 		g_myBDB 	= NULL; /* the global handle to DB handle */
DataBase 	g_dbInfo	= {0};

#define 	DATABASE_CONF		"db_bdb.conf"

int
get_db_cfg(DataBase* db_info){
	FILE*	pFile = NULL;
	char	szBuffer[MAX_BUF_2048];
	int	nStrLen;
	int	nLanguageType;

	memset(db_info, 0, sizeof(DataBase));

	pFile = fopen(DATABASE_CONF, "r");
	if ( pFile == NULL ) {
		write_log(LT_BOTH, "Error: Can't open file: %s\n", 
			  DATABASE_CONF);
		return 0;
	}

	while (fgets( szBuffer, MAX_BUF_2048, pFile ) != NULL ){
		if ( szBuffer[0] == ';' || szBuffer[0] == '\n')
			continue;

		nStrLen = (int)strlen(szBuffer);
		if ( szBuffer[nStrLen-1] == '\n' )
			szBuffer[nStrLen-1] = '\0';
		
		if ( strncmp(szBuffer, "language=", 9) == 0 ){
			nLanguageType = atoi(szBuffer+9);
			if ( nLanguageType >= 0 && nLanguageType < LANGUAGE_NUM )
				db_info->nLanguageType = nLanguageType;
				sprintf(db_info->szDBName, "%s%s.db", TABLE_NAME_PREFIX, 
					g_szTableType[nLanguageType]);
		}
		else{
			write_log(LT_BOTH, "Error: Unknown info %s in %s!\n", 
				  szBuffer, DATABASE_CONF);
		}
	}

	SAFE_CLOSE_FILE(pFile);

	if ( db_info->szDBName[0] == '\0' ){
		write_log(LT_BOTH, "Error: Database configure file lack of "
				   "enough info about database!\n");
		return 0;
	}

	return 1;
}

void
db_err_handler(const DB_ENV* env, const char* err_prefix, const char* msg){
	write_log(LT_FILE, "%s: %s!\n", err_prefix,  msg);
}

int
open_db(DB** pDb, const char* pcszDBName, int nClear, int nDuplicate){
	DB*	db;
	int	nRet;
	
	*pDb = NULL;
 opendb:
	/* Initialize BDB handle */
	if ( db_create(&db, NULL, 0) ){
		write_log(LT_BOTH, "Error: Can't init BDB,"
			  "when open database %s!\n", pcszDBName ) ;
		return 0;
	}

	/* if need clear table */
	if ( nClear == 1 ){
		db->remove(db, pcszDBName, NULL, 0);
		nClear = 0;
		goto opendb;
	}

	/* set up error handling for this database */
	db->set_errcall(db, db_err_handler);
	db->set_errpfx(db, "jau_db");
	if ( nDuplicate == 1 )
		db->set_flags(db, DB_DUPSORT);
	
	/* Now open the database */
	nRet = db->open(db, NULL, pcszDBName,
			     NULL, DB_BTREE, DB_CREATE, 0);
	if (nRet != 0) {
		write_log(LT_BOTH, "Error: Can't open database %s!\n",
			  pcszDBName);
		db->close(db, 0);
		return 0;
	}
	*pDb = db;
	
	return 1;
}

void
close_db(DB* db){
	if ( db != NULL ){
		db->close(db, 0);
		db = NULL;
	}
}

int
db_init(int nClearTable){
	int 	nRet;

	//get database configure from configure file
	nRet = get_db_cfg( &g_dbInfo );
	if ( nRet == 0 )
		return 0;
	
	if ( g_myBDB != NULL ){
		write_log(LT_BOTH, "Error: the handle to BDB isn't empty!\n");
		return 0;
	}
	
	return open_db(&g_myBDB, g_dbInfo.szDBName,
		       nClearTable, 0);
}

void
db_uninit(){
	close_db(g_myBDB);
}

int
update_trans_to_db(const char* pcszChinese, const char* pcszTrans){
	DBC 	*cursorp;
	DBT 	key, data;
	int	nRet = 0;
	char    szTransConverted[MAX_TRANS_LEN];
	
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));

	/* Set up our DBTs */
	key.data = (void*)pcszChinese;
	key.size = (unsigned int)strlen(pcszChinese) + 1;
	/* Get the cursor */
	g_myBDB->cursor(g_myBDB, NULL, &cursorp, 0);
	if ( cursorp == NULL ) {
		write_log(LT_FILE, "Error: Can't get the cursor, when update "
				   "the translate of %s!\n", pcszChinese);
		return 0;
	}

	special_sign_convert(pcszTrans, szTransConverted, g_dbInfo.nLanguageType);
	
	/* Position the cursor */
	nRet = cursorp->c_get(cursorp, &key, &data, DB_SET);
	if (nRet == 0) {
		data.data = (void*)szTransConverted;
		data.size = (unsigned int)strlen(szTransConverted) + 1;
		cursorp->c_put(cursorp, &key, &data, DB_CURRENT);
	}
	else{
		write_log(LT_FILE, "Error: Can't find record %s, when update "
				   "its translate, try to insert it!\n",
				   pcszChinese);
		nRet = insert_to_db(pcszChinese, pcszTrans);
		goto exit;
	}

	write_log(LT_FILE, "Info: %s update success!\n", pcszChinese);

	nRet = 1;
 exit:
	/* Cursors must be closed */
	if ( cursorp != NULL ) 
		cursorp->c_close(cursorp); 
	
	return nRet;
}

int
insert_to_db(const char* pChinese, const char* pTranslate){
	int	nRet, nRet2;
	DBT 	key, data;
	char	szTransConverted[MAX_TRANS_LEN];
	char	szTranslate[MAX_TRANS_LEN];
	
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));

	special_sign_convert(pTranslate, szTransConverted, g_dbInfo.nLanguageType);

	/* Set up our DBTs */
	key.data = (void*)pChinese;
	key.size = (unsigned int)strlen(pChinese) + 1;
	data.data = (void*)szTransConverted;
	data.size = (unsigned int)strlen(szTransConverted) + 1;

	nRet = g_myBDB->put(g_myBDB, NULL, &key, &data, DB_NOOVERWRITE);
	if ( nRet == DB_KEYEXIST ){
		nRet2 = get_trans_from_db((char*)pChinese, szTranslate,
					  MAX_TRANS_LEN);
		if ( nRet2 == 0 ){
			write_log(LT_FILE, "Error: Can't get translate of %s,"
					   "when insert it!\n", pChinese);
			return 0;
		}

		return 2;
	}
	else if ( nRet != 0 ) {
		write_log(LT_FILE, "Error: insert fail at string: %s,"
				   "return value: %d!\n", pChinese, nRet);
		return 0;
	}

	write_log(LT_FILE, "Info: %s update success!\n", pChinese);

	return 1;
}

int
insert_notrans_to_db(const char* pcszChinese){
	DBT 	key, data;
	int	nRet;
	char	szNULL[] = "";
	
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));

	/* Set up our DBTs */
	key.data = (void*)pcszChinese;
	key.size = (unsigned int)strlen(pcszChinese) + 1;

	data.data = (void*)szNULL;
	data.size = 1;

	nRet = g_myBDB->put(g_myBDB, NULL, &key, &data, DB_NOOVERWRITE);
	if ( nRet == DB_KEYEXIST )
		return 2;
	else if ( nRet != 0 ) {
		write_log(LT_FILE, "Error: insert fail at string: %s,"
				   "return value: %d\n", pcszChinese, nRet);
		return 0;
	}

	//here the string is first inserted to database,
	//judge there is gbk,if not, translate it automatic
	nRet = is_there_gbk_code(pcszChinese);
	if ( nRet == 0 ){
		update_trans_to_db(pcszChinese, pcszChinese);
		return 3;
	}
	
	return 1;
}

int
get_trans_from_db(char* pChinese, char* pTranslate, int nTransSize){
	DBT 	key, data;
	int	nRet, nStrLen;
	
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));

	/* Set up our DBTs */
	key.data = (void*)pChinese;
	key.size = (unsigned int)strlen(pChinese) + 1;

	nRet = g_myBDB->get(g_myBDB, NULL, &key, &data, 0);
	if ( nRet != 0 ){
		write_log(LT_FILE, "Error: Can't get the translate of [%s], ret: %d!\n",
			  pChinese, nRet);
		return 0;
	}

	if ( data.size <= 1 ){
		write_log(LT_FILE, "Error: the string of [%s] havn't translated!\n",
			  pChinese);
		return 0;
	}

	nStrLen = data.size;
	if ( nStrLen >= nTransSize ){
		write_log(LT_FILE, "Error: the length of translate [%s] exceed"
			  "the max length:%d!\n", pChinese, nTransSize);
		nStrLen = nTransSize - 1;
	}

	strncpy(pTranslate, (char*)data.data, nStrLen);
	pTranslate[nStrLen-1] = '\0';

	return 1;
}

int
query_record_from_db(const char* pcszChinese, int nDelete){
	DBT 	key, data;
	int	nRet;
	
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));

	/* Set up our DBTs */
	key.data = (void*)pcszChinese;
	key.size = (unsigned int)strlen(pcszChinese) + 1;

	nRet = g_myBDB->get(g_myBDB, NULL, &key, &data, 0);
	if ( nRet != 0 ){
		write_log(LT_BOTH, "Error: Can't get the information of %s, ret: %d\n",
			  pcszChinese, nRet);
		return 0;
	}

	if ( data.size > 1 )
		write_log(LT_BOTH, "Its translate: %s\n", (char*)data.data);

	if ( nDelete == 1){
		g_myBDB->del(g_myBDB, NULL, &key, 0);
		write_log(LT_BOTH, "this record has deleted from database!\n");
	}

	return 1;
}

#endif//_USE_MYSQL_

int
auto_special_sign_convert(const char* pcszChinese, const char* pcszTrans){
	char szTrans[MAX_TRANS_LEN];

	memset(szTrans, 0, sizeof(szTrans));
	special_sign_convert(pcszTrans, szTrans, g_dbInfo.nLanguageType);
	update_trans_to_db(pcszChinese, szTrans);
	
	return 1;
}

/* 只导出含有非中文字符的已翻译内容 */
int
export_db(const char* pcszChinese, const char* pcszTrans){
	if (is_there_gbk_code(pcszTrans))
		return 1;
	return 0;
}

/* 导出所有翻译内容 */
int
export_db_all(const char* pcszChinese, const char* pcszTrans){
	return 0;
}

int
check_gbk(const char* pcszChinese, const char* pcszTrans){
	if ( is_there_gbk_code(pcszTrans) )
		return 0;
	return 1;
}

typedef int (*TRAVERSE_OPERATION_FUNC)(const char* pcszChinese, const char* pcszTranslate);

TRAVERSE_OPERATION_FUNC
get_traverse_operation_func(const char* pcszOperation){
	if ( strcmp(pcszOperation, "convert") == 0 ){
		if ( load_special_sign_table() == 0 ){
			write_log(LT_BOTH, "Error: Can't load special sign table!\n");
			return NULL;
		}
		else
			return auto_special_sign_convert;
	}
	else if ( strcmp(pcszOperation, "check_brackets") == 0 )
		return check_brackets;
	else if ( strcmp(pcszOperation, "export") == 0 )
		return export_db;
	else if ( strcmp(pcszOperation, "export_all") == 0 )
		return export_db_all;
	else if (  strcmp(pcszOperation, "check_vlist") == 0 )
		return check_vlist;
	else if ( strcmp(pcszOperation, "check_gbk") == 0 )
		return check_gbk;
	else
		return NULL;
}

int
traverse_to_do_something(const char* pcszChineseFile, const char* pcszTransFile,
			 const char* pcszOperation){
	int		nRet = 0;
	FILE*		pFile1;
	FILE*		pFile2;
	TRAVERSE_OPERATION_FUNC func;

	assert(pcszChineseFile);
	assert(pcszTransFile);
	assert(pcszOperation);
		
	pFile1 = fopen(pcszChineseFile, "w");

	if ( pFile1 == NULL ){
		write_log(LT_BOTH, "Error: Can't open file %s\n!", pcszChineseFile);
		goto exit;
	}
	
	pFile2 = fopen(pcszTransFile, "w");	
	if ( pFile2 == NULL ){
		write_log(LT_BOTH, "Error: Can't open file %s\n!", pcszTransFile);
		goto exit;
	}

	func = get_traverse_operation_func(pcszOperation);
	if ( func == NULL ){
		write_log(LT_BOTH, "Error: Can't get the corresponding"
		" func of operation '%s'\n!", pcszOperation);
		goto exit;
	}

#ifdef _USE_MYSQL_	
	MYSQL_RES*	res;
	MYSQL_ROW	row;
	char 		szQueryStatement[MAX_SQL_LEN2];
	sprintf(szQueryStatement, SQL_QUERY_All, g_dbInfo.szTable);

	if ( mysql_query( g_myData, szQueryStatement )){
		write_log(LT_BOTH, "Error: %s, ErrNo: %d\n", 
			mysql_error(g_myData), mysql_errno(g_myData));
		write_log(LT_BOTH, "Failed: when query \"%s\"!\n", 
			szQueryStatement);
		goto exit;
	}

	res = mysql_use_result( g_myData );

	if ( res == NULL ){
		write_log(LT_BOTH, "Error: Can't use result,"
			"when query \"%s\"!\n",
			szQueryStatement);
		goto exit;
	}

	while (row = mysql_fetch_row( res ))
	{
		if ( strlen(row[1]) > 0 ){
			nRet = func(row[0], row[1]);
			if (nRet == 0)
			{
				fwrite(row[0], 1, strlen(row[0]), pFile1);
				fwrite("\n", 1, 1, pFile1);
				fwrite(row[1], 1, strlen(row[1]), pFile2);
				fwrite("\n", 1, 1, pFile2);
			}
		}
	}
	mysql_free_result( res );
#else
	DBC 		*cursorp;
	DBT 		key, data;
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));

	g_myBDB->cursor(g_myBDB, NULL, &cursorp, 0);
	if ( cursorp == NULL ) {
		write_log(LT_BOTH, "Error: Can't get the cursor!\n");
		goto exit;
	}
	/* Iterate over the database, retrieving each record in turn. */
	while ( (nRet = cursorp->c_get(cursorp, &key,
		&data, DB_NEXT)) == 0 ) {
		if ( data.size > 1 ){
			nRet = func((char*)key.data, (char*)data.data);
			if (nRet == 0)
			{
				fwrite(key.data, 1, key.size-1, pFile1);
				fwrite("\n", 1, 1, pFile1);
				fwrite(data.data, 1, data.size-1, pFile2);
				fwrite("\n", 1, 1, pFile2);
			}
		}
	}
#endif
	nRet = 1;

exit:
	SAFE_CLOSE_FILE(pFile1);
	SAFE_CLOSE_FILE(pFile2);

	return nRet;
}

int
update_trans_to_db_from_a_file(const char* pcszFileName, int nOldTrans){
	FILE*	pFile;
	char	szChineseFileName[MAX_PATH];
	char	szTransFileName[MAX_PATH];
	int	nStrLen;
	int	nRet = 0;

	assert(pcszFileName);
	pFile = fopen(pcszFileName, "r");
	if ( pFile == NULL ){
		write_log(LT_BOTH, "Error: Can't open file %s!\n", pcszFileName);
		return 0;
	}

	while ( fgets(szChineseFileName, MAX_PATH, pFile) != NULL ){
		if ( szChineseFileName[0] == '\n' ||
		     szChineseFileName[0] == '\r' )
			break;
		
		if ( fgets(szTransFileName, MAX_PATH, pFile) == NULL ){
			write_log(LT_BOTH, "Error: Cant't get the corresponding "
				  "translate file of %s !\n", szChineseFileName);
			nRet = 0;
			goto exit;
		}

		nStrLen = (int)strlen(szChineseFileName);
		if ( szChineseFileName[nStrLen-1] == '\n' )
			szChineseFileName[nStrLen-1] = '\0';
		nStrLen = (int)strlen(szTransFileName);
		if ( szTransFileName[nStrLen-1] == '\n' )
			szTransFileName[nStrLen-1] = '\0';

		write_log(LT_BOTH, "-----------------------updating---"
			  "----------------------\n"
			  "chinese file: %s \ntranslate file: %s!\n",
			  szChineseFileName, szTransFileName);

		nRet = update_trans_to_db_from_files(szChineseFileName, 
						     szTransFileName, nOldTrans);
		if ( nRet == 0 ){
			write_log(LT_BOTH, "Error: when update chinese "
				  "file: %s and translate file: %s!\n",
				  szChineseFileName, szTransFileName);
			goto exit;
		}
	}

	nRet = 1;
 exit:
	SAFE_CLOSE_FILE(pFile);

	return nRet;
}

int
update_trans_to_db_from_files(const char* pcszChineseFile,
			      const char* pcszTransFile, int nOldTrans){
	char 		szChinese[MAX_CHINESE_LEN];
	char		szTranslate[MAX_TRANS_LEN];
	int		nStrLen = 0;
	FILE*		pFile  	= NULL;
	FILE*		pFile2 	= NULL;
	int		nLine1  = 1;
	int		nLine2  = 1;
	int		nRet	= 0;
	int		nCount	= 0;
	int		nCount2	= 0;
	int		nCount3 = 0;
	int		nCount4 = 0;
	int		nChineseHeadPos, nTransHeadPos;

	assert(pcszChineseFile);
	assert(pcszTransFile);
	
	pFile  = fopen(pcszChineseFile, "r");
	if ( pFile == NULL ){
		write_log(LT_BOTH, "Error: Can't open file %s\n", pcszChineseFile);
		return 0;
	}

	pFile2 = fopen(pcszTransFile, "r");
	if ( pFile2 == NULL ){
		write_log(LT_BOTH, "Error: Can't open file %s\n",
			  pcszTransFile);
		SAFE_CLOSE_FILE(pFile);
		return 0;
	}

	while( fgets(szChinese, MAX_CHINESE_LEN, pFile) != NULL){
		nLine1 ++;
		if ( szChinese[0] == '\n' || szChinese[0] == '\r' )
			break;

		if ( pFile2 == NULL ){
			if ( fgets(szTranslate, MAX_TRANS_LEN, pFile) == NULL ){
				write_log(LT_BOTH, "Error: the file's format is "
					  "not right at line: %d, file: %s\n",
					  nLine1, pcszChineseFile);
				nRet = 0;
				goto exit;
			}
			nLine1++;
		}
		else{
			if ( fgets(szTranslate, MAX_TRANS_LEN, pFile2) == NULL ){
				write_log(LT_BOTH, "Error: the file's format "
					  "is not right at line1: %d, line2: %d, file: %s\n",
					  nLine1, nLine2, pcszTransFile);
				nRet = 1;
				goto exit;
			}
			nLine2++;
		}

		szChinese[MAX_CHINESE_LEN-1] = '\0';
		szTranslate[MAX_TRANS_LEN-1] = '\0';

		// drop '\n' at the tail of the line
		nStrLen = (int)strlen(szTranslate);
		if ( szTranslate[nStrLen-1] == '\n' )
			szTranslate[nStrLen-1] = '\0';

		nStrLen = (int)strlen(szChinese);
		if ( szChinese[nStrLen-1] == '\n' )
			szChinese[nStrLen-1] = '\0';

		if ( nOldTrans > 0 ){
			nRet = process_old_chinese_and_trans(szChinese, szTranslate,
							     &nChineseHeadPos, &nTransHeadPos);
			if (nRet == 0){
				nCount4 ++;
			}
			nRet = insert_to_db(&szChinese[nChineseHeadPos], 
					    &szTranslate[nTransHeadPos]);
		}
		else
			nRet = update_trans_to_db(szChinese, szTranslate);
		
		if ( nRet == 1 )
			nCount++;
		else if ( nRet == 0 )
			nCount2 ++;
		else
			nCount3 ++;
	}

	nRet = 1;
 exit:
	SAFE_CLOSE_FILE(pFile);
	SAFE_CLOSE_FILE(pFile2);
	
	write_log(LT_BOTH, "\nResult: \n");
	if ( nCount > 0 )
		write_log(LT_BOTH, "Total %d records are updated\n", nCount);
	if ( nCount3 > 0 )
		write_log(LT_BOTH, "Total %d records have repeated\n", nCount3);
	if ( nCount4 > 0 )
		write_log(LT_BOTH, "Total %d records is not corresponding!\n", nCount4);
	if ( nCount2 > 0 )
		write_log(LT_BOTH,"Error: there are %d errors when update translate,"
			  "see log.txt for detail!\n", nCount2);
	
	return nRet;
}
