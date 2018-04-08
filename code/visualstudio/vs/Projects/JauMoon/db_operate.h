#ifndef _DB_OPERATE_H_
#define _DB_OPERATE_H_

#ifdef  _USE_MYSQL_
#define SQL_QUERY_CHINESE	"select * from %s where chinese='%s'"
#define SQL_QUERY_All		"select * from %s"
#define SQL_INSERT_STR		"insert into %s values('%s','%s')"
#define SQL_UPDATE_STR		"update %s set translate='%s' where chinese='%s'"
#define SQL_DELETE_TABLE	"delete from %s"
#define SQL_DELETE_STR		"delete from %s where chinese='%s'"
#define SQL_TABLE_CREATE	"create table if not exists %s (chinese text character\
				 set gbk, primary key (chinese(200)), translate text)"

typedef struct DATABASE_INFO{
	char	szUser[32];
	char	szPasswd[32];
	char	szDatabaseSvr[32];
	char	szDatabase[32];
	char	szTable[32];
	int	nPort;
	int	nLanguageType;
}DataBase;

#else
typedef struct DATABASE_INF{
	int	nLanguageType;
	char	szDBName[32];
}DataBase;
#endif //_USE_MY_SQL

#define MAX_BUF_2048		2048
#define MAX_CHINESE_LEN		MAX_BUF_2048
#define MAX_TRANS_LEN		MAX_BUF_2048*3
#define MAX_SQL_LEN1		MAX_CHINESE_LEN+256
#define MAX_SQL_LEN2		MAX_CHINESE_LEN+MAX_TRANS_LEN+256
#define MAX_LINE_LEN		MAX_BUF_2048*5

enum TRANS_STATE{		/* record type */
	TS_NOTRANSLATE = 1,  	/* not translated record*/
	TS_FILENAME,		/* file name record */
	TS_PATH,		/* path record */
	TS_ERROR,		/* error record which is not recognised*/
	TS_TRANSLATED,		/* translated record */
	TS_ALL,
	TS_STATUS_NUM,
};

/* 海外版的语言类型定义， */
enum LANGUAGE_TYPE{
	TEMP,			/* 临时字典 */
	TRANTIONAL,		/* 繁体 */
	ENGLISH,		/* 英文 */
	VIETNAM,		/* 越南 */
	CAMBODIA,		/* 柬埔寨 */
	LANGUAGE_NUM,
};
/* 以下函数mysql版本的berkeleydb版本的jau的实现不同 */
int  db_init(int nClearTable);
void db_uninit();

/* 更新一条具体的中文和翻译相对应的记录到数据库中 */
int  update_trans_to_db(const char* pcszChinese, const char* pcszTrans);
/* 插入一条没有翻译的中文记录到数据库中 */
int  insert_notrans_to_db(const char* pcszChinese);
/* 获取某条记录对应的翻译 */
int  get_trans_from_db(char* pChinese, char* pTranslate, int nTransSize);
/* 查询某条中文记录对应的翻译，如果ndelete为1,则查询后直接将该记录从数据库中删除掉 */
int  query_record_from_db(const char* pcszChinese, int nDelete = 0);
/* 插入一条具体的中文和翻译相对应的记录到数据库中 */
int  insert_to_db(const char* pcszChinese, const char* pcszTrans);

/* 下面这三个函数两个版本的实现是相同的 */
/* 从一个中文文件和它对应的翻译文件中读取对应的行将其更新到数据库中，
   nOldTrans指是否以兼容老的工具抽取的记录的方式对记录进行更新 */
int  update_trans_to_db_from_files(const char* pcszChineseFile,
				   const char* pcszTransFile,
				   int nOldTrans);

/* 从一个文件中每读两行，把第一行作为中文文件的文件名，把第二行作为对应的翻译的文件名，然后
   使用update_trans_to_db_from_files这个函数将这两个文件中的内容更新到数据库中 */
int  update_trans_to_db_from_a_file(const char* pcszFileName, int nOldTrans);

/* 遍历数据库中的每一条已翻译的记录对其执行某种指定的操作，并将需要输出的记录分别输出到中文和翻译两个文件中
   pcszChineseFile为执行操作之后需要记录的中文记录所写的文件
   pcszTransFile为执行操作之后需要记录的翻译记录所写的文件
   pcszoperation为需要执行的操作的字符串*/
int  traverse_to_do_something(const char* pcszChineseFile,
			      const char* pcszTransFile,
			      const char* pcszOperation);

#endif // _DB_OPERATE_H_
