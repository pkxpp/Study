#include 	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>
#include 	<stdarg.h>
#include    <QMessageBox>   /* test for */
#include    <QString>       /* test for */
#include    <QObject>

#include	"db_operate.h"
#include	"str_operate.h"

FILE* g_logFile	= NULL;	 	 /* 操作日志记录文件 */

void
show_version(){
	printf("JX2 abroad util (C)2007 KINGSOFT\n");
	printf("Developed by jxonline2 abroad team\n");
	printf("Build at "__DATE__" "__TIME__"\n");
}

void
write_log(int nLogType, char* pLogFormat, ...){
	char	szLogBuffer[MAX_LINE_LEN];
	va_list list;
	int	nStrLen;
	
	va_start(list, pLogFormat);
	nStrLen = vsprintf(szLogBuffer, pLogFormat, list);
	va_end(list);

	if ( nLogType & LT_SCREEN )
		printf(szLogBuffer);
	if ( nLogType & LT_FILE )
		fwrite(szLogBuffer, 1, strlen(szLogBuffer),
		       g_logFile);
}

void
show_help(){
	printf("------------------------------------------------------------------------------\n");
	printf("NAME:\n");
	printf("-----\n");
	printf("\tjau - jxonline2 abroad util\n");

	printf("\nDESCRIPTION:\n");
	printf("\tthe purpose of jau is to make jxonline2 abroad version conveniently\n");
	printf("\tand effectively(also can be used in other game).\n");
	printf("\tjau can do six main kinds of things to help you improve\n");
	printf("\tyour work, reduce your work time\n");

	printf("\n\t -  draw out all chinese string from files under a path\n");
	printf("\t     and put them to translate database\n");
	printf("\t -  draw out all chinese string from files under a path\n");
	printf("\t     and query database to get their translate, then use\n");
	printf("\t     translate instead of them\n");
	printf("\t -  update chinese string's translate from file to database\n");
	printf("\t -  query database to get all chinese string according to\n");
	printf("\t     its translate type or query a chinese string's translate\n");
	printf("\t -  traverse translate database to do something\n");

	printf("\nUSAGE: \n");
	printf("------\n");
	printf("\t  @  jau getstring {-s=dir}\n");
	printf("\t\t-s specify the directory you want to draw out chinese string\n");

	printf("\t  @  jau transalte {-s=dir -t=dir}\n");
	printf("\t\t-s specify the directory you want to draw out chinese string\n");
	printf("\t\t-t specify the target directory you want to save translated files\n");
	
	printf("\t  @  jau update  [-o]  {-f=file | -c=cfile -t=tfile | -C=cstr -T=tstr}\n");
	printf("\t\t-o the update content comes from old dictionary!\n");
	printf("\t\t-f specify a file from which you can update the database\n");
	printf("\t\t   file format: odd line, chinese string. even line, translate\n");
	printf("\t\t-c -t specify the chinese string file and the corresponding\n");
	printf("\t\t   translate string file\n");
	printf("\t\t-C -T specify a chinese string and its translate\n");

	printf("\t  @  jau query [-d] {-C=ctr}\n");
	printf("\t\t-C specify a chinese string you want to query\n");
	printf("\t\t   the query results include its translate(if exist) and status\n");
	printf("\t\t-d delete the chinese string and its translate from database\n");

	printf("\t  @  jau traverse {-d=operate -c=cfile -t=tfile}\n");
	printf("\t\t-d specify the operation you want to do\n");
	printf("\t\t-c specify the chinese file which you want to export database to it\n");
	printf("\t\t-t specify the translate file which you want to export database to it\n");

	printf("\nNOTE: \n");
	printf("-----\n");
	printf("\tWhen a syntax element consists of a number of alternatives, the\n");
	printf("\talternatives are separated by vertical bars (\"|\"). When one member\n");
	printf("\tfrom a set of choices _may_ be chosen, the alternatives are listed\n");
	printf("\twithin square brackets (\"[\" and \"]\")\n");
	printf("\tWhen one member from a set of choices _must_ be chosen, the\n");
	printf("\talternatives are listed within braces (\"{\" and \"}\"):\n");	
	printf("------------------------------------------------------------------------------\n");	
}

int
get_string(char* argv[]){
/*	char	szDirectory[MAX_PATH];
	int	nIdx = 2;

	memset(szDirectory, 0, sizeof(szDirectory));
	while ( argv[nIdx] != NULL ){
		if ( strncmp(argv[nIdx], "-s=", 3) == 0 ){
			strcpy(szDirectory, &argv[nIdx][3]);
			szDirectory[MAX_PATH-1] = '\0';
		}
		else{
			printf("Error: invalid option '%s'\n", argv[nIdx]);
			return 0;
		}
		nIdx ++;
	}

	if ( szDirectory[0] == '\0' ){
		write_log(LT_BOTH,"Error: Please specify the source root path after '-s'\n");
		return 0;
	}
	
	get_path_or_filename(szDirectory);
	if ( is_a_valid_directory(szDirectory) == 0 )
		return 0;
	
	// init log file system which is used to record some error for get string
	if ( str_operate_init(szDirectory, NULL) == 0){
		write_log(LT_BOTH,"Error: Can't init 'get string' module!\n");
		return 0;
	}

	if ( db_init(0) == 0 ){
		write_log(LT_BOTH,"Error: Can't init 'database operation' module!\n");
		return 0;
	}

	write_log(LT_BOTH,"Current task:\n");
	write_log(LT_BOTH,"--------------------------------------------------------\n");
	write_log(LT_BOTH,"Get all chinese string from directory:\n\t'%s'\n", szDirectory);
	write_log(LT_BOTH,"The string will be inserted into translate database!\n");
	write_log(LT_BOTH,"--------------------------------------------------------\n\n");
*/
	return start_work();
}

int infGetString(char *szDirectory)
{
    if ( szDirectory[0] == '\0' ){
        write_log(LT_BOTH,"Error: Please specify the source root path after '-s'\n");
        return 0;
    }

	memcpy(szDirectory, "E:\\我的文档\\work\\TransWork\\chinese_setting", sizeof("E:\\我的文档\\work\\TransWork\\chinese_setting"));
//    QMessageBox::information(NULL, QObject::tr("Directory"), QObject::tr("Directory: ")+szDirectory);
    get_path_or_filename(szDirectory);
    if ( is_a_valid_directory(szDirectory) == 0 )
        return 0;

    // init log file system which is used to record some error for get string
    if ( str_operate_init(szDirectory, NULL) == 0){
        write_log(LT_BOTH,"Error: Can't init 'get string' module!\n");
        return 0;
    }

    if ( db_init(0) == 0 ){
        write_log(LT_BOTH,"Error: Can't init 'database operation' module!\n");
        return 0;
    }

    write_log(LT_BOTH,"Current task:\n");
    write_log(LT_BOTH,"--------------------------------------------------------\n");
    write_log(LT_BOTH,"Get all chinese string from directory:\n\t'%s'\n", szDirectory);
    write_log(LT_BOTH,"The string will be inserted into translate database!\n");
    write_log(LT_BOTH,"--------------------------------------------------------\n\n");

    return start_work();
}

int init_interface()
{
    time_t	nStartTm, nEndTm;

    g_logFile = fopen("log.txt", "a");
    if ( g_logFile == NULL ){
        write_log(LT_SCREEN,"Error: Can't open log file!\n");
        goto exit;
    }
	nStartTm = time(NULL);

exit:
   write_log(LT_SCREEN,"Please add '-h' option to get the detail help!\n");

exit2:
   nEndTm = time(NULL);
   write_log(LT_BOTH,"\n--------------------------------------------------------\n");
   write_log(LT_BOTH,"Task end, total use time: %d min, %d sec\n",
          (int)(nEndTm - nStartTm)/60, (int)(nEndTm - nStartTm)%60);

   db_uninit();
   str_operate_uninit();

//   SAFE_CLOSE_FILE(g_logFile);
   return 0;
}

int end_interface()
{
	SAFE_CLOSE_FILE(g_logFile);
	return 0;
}

int infInit()
{
    init_interface();
	return 0;
}

int
translate_chinese(char* argv[]){
	char	szDirectory[MAX_PATH];
	char	szDirectory2[MAX_PATH];	
	int	nIdx = 2;
	
	memset(szDirectory,  0, sizeof(szDirectory));
	memset(szDirectory2, 0, sizeof(szDirectory2));	
	while ( argv[nIdx] != NULL ){
		if ( strncmp(argv[nIdx], "-s=", 3) == 0 ){
			strcpy(szDirectory, &argv[nIdx][3]);
			szDirectory[MAX_PATH-1] = '\0';
		}
		else if ( strncmp(argv[nIdx], "-t=", 3) == 0 ){
			strcpy(szDirectory2, &argv[nIdx][3]);
			szDirectory2[MAX_PATH-1] = '\0';
		}
		else{
			write_log(LT_BOTH,"Error: invalid option '%s'", argv[nIdx]);
			return 0;
		}
		nIdx ++;
	}

	if ( szDirectory[0] == '\0' ){
		write_log(LT_BOTH,"Error: Please specify the source root path after '-s'\n");
		return 0;
	}
	
	if ( szDirectory2[0] == '\0' ){
		write_log(LT_BOTH,"Error: Please specify the target root path after '-t'\n");
		return 0;
	}

	get_path_or_filename(szDirectory);
	get_path_or_filename(szDirectory2);

	if ( is_a_valid_directory(szDirectory) == 0 )
		return 0;

	if ( is_a_valid_directory(szDirectory2) == 0 )
		return 0;
	
	// init log file system which is used to record some error for get string
	if ( str_operate_init(szDirectory, szDirectory2) == 0){
		write_log(LT_BOTH,"Error: Can't init 'get string' module!\n");
		return 0;
	}

	if ( db_init(0) == 0 ){
		write_log(LT_BOTH,"Error: Can't init 'database operation' module!\n");
		return 0;
	}

	write_log(LT_BOTH,"Current task:\n");
	write_log(LT_BOTH,"--------------------------------------------------------\n");
	write_log(LT_BOTH,"Translate all chinese string from directory:\n\t'%s'\n", szDirectory);
	write_log(LT_BOTH,"The results will write into directory:\n\t'%s'\n", szDirectory2);
	write_log(LT_BOTH,"--------------------------------------------------------\n\n");
	
	return start_work();
}

int
query_database(char* argv[]){
	char	szChinese[MAX_CHINESE_LEN];
	int	nIdx = 2;
	int	nRet;
	int	nDelete = 0;

	while ( argv[nIdx] != NULL ){
		if ( strncmp(argv[nIdx], "-C=", 3) == 0 ){
			strcpy(szChinese, &argv[nIdx][3]);
			szChinese[MAX_CHINESE_LEN-1] = '\0';
		}
		else if ( strcmp(argv[nIdx], "-d") == 0 )
			nDelete = 1;
		else{
			write_log(LT_BOTH,"Error: invalid option '%s'\n", argv[nIdx]);
			return 0;
		}
		nIdx ++;
	}
	
	if ( db_init(0) == 0 ){
		write_log(LT_BOTH,"Error: Can't init 'database operation' module!\n");
		return 0;
	}

	if ( szChinese[0] != '\0' ) {
		write_log(LT_BOTH,"Current task:\n");
		write_log(LT_BOTH,"--------------------------------------------------------\n");
		write_log(LT_BOTH,"Query records follow from database:\n\t%s\n", szChinese);
		write_log(LT_BOTH,"--------------------------------------------------------\n\n");

		nRet = query_record_from_db(szChinese, nDelete);
	}
	else{
		write_log(LT_BOTH,"Error: Please use '-C' option to specify a chinese!");
		return 0;
	}
	
	return nRet;
}

int
update_translate(char* argv[]){
	char	szFileName[MAX_PATH];
	char	szFileName2[MAX_PATH];
	char	szChinese[MAX_CHINESE_LEN];
	char	szTranslate[MAX_TRANS_LEN];
	int	nIdx = 2;
	int	nUpdateType = 0;
	int	nRet, nOldTrans = 0;
	int	nClearTable = 0;

	memset(szFileName,  0, sizeof(szFileName));
	memset(szFileName2, 0, sizeof(szFileName2));
	memset(szChinese,   0, sizeof(szChinese));
	memset(szTranslate, 0, sizeof(szTranslate));
	
	while ( argv[nIdx] != NULL ){
		if ( strncmp(argv[nIdx], "-f=", 3) == 0 ){
			nUpdateType = 1;
			strcpy(szFileName, &argv[nIdx][3]);
			szFileName[MAX_PATH-1] = '\0';
		}
		else if ( strncmp(argv[nIdx], "-c=", 3) == 0 ){
			nUpdateType = 2;
			strcpy(szFileName, &argv[nIdx][3]);
			szFileName[MAX_PATH-1] = '\0';			
		}
		else if ( strncmp(argv[nIdx], "-C=", 3) == 0 ){
			nUpdateType = 3;
			strcpy(szChinese, &argv[nIdx][3]);
			szChinese[MAX_CHINESE_LEN-1] = '\0';			
		}
		else if ( strncmp(argv[nIdx], "-t=", 3) == 0 ){
			strcpy(szFileName2, &argv[nIdx][3]);
			szFileName2[MAX_PATH-1] = '\0';
		}
		else if ( strncmp(argv[nIdx], "-T=", 3) == 0 ){
			strcpy(szTranslate, &argv[nIdx][3]);
			szTranslate[MAX_TRANS_LEN-1] = '\0';
		}
		else if ( strcmp(argv[nIdx], "-o") == 0 ){
			nOldTrans = 1;
		}
		else if ( strcmp(argv[nIdx], "-d") == 0 ){
			nClearTable = 1;			
		}
		else{
			write_log(LT_BOTH,"Error: invalid option '%s'\n", argv[nIdx]);
			return 0;
		}
		nIdx ++;
	}

	if ( db_init(nClearTable) == 0 ){
		write_log(LT_BOTH,"Error: Can't init 'database operation' module!\n");
		return 0;
	}

	write_log(LT_BOTH,"Current task:\n");
	write_log(LT_BOTH,"--------------------------------------------------------\n");
	if ( nUpdateType == 1 ){
		if ( szFileName[0] == '\0' ){
			write_log(LT_BOTH,"Error: Please specify the filename after '-f'!\n");
			return 0;
		}
		
		write_log(LT_BOTH,"\t'%s'\n", szFileName);
		write_log(LT_BOTH,"--------------------------------------------------------\n\n");
		get_path_or_filename(szFileName);

		nRet = update_trans_to_db_from_a_file(szFileName, nOldTrans);
	}
	else if ( nUpdateType == 2 ){
		if ( szFileName[0] == '\0' || szFileName2[0] == '\0' ){
			write_log(LT_BOTH,"Error: Please specify the chinese filename after");
			write_log(LT_BOTH,"'-c' or translate filename after '-t'!\n");
			return 0;
		}
		
		write_log(LT_BOTH,"Chinese string file:\n\t'%s'\n", szFileName);
		write_log(LT_BOTH,"Translate string file:\n\t'%s'\n", szFileName2);
		if ( nOldTrans == 1 )
			write_log(LT_BOTH,"Update type: old\n");
		else
			write_log(LT_BOTH,"Update type: new\n");
		write_log(LT_BOTH,"--------------------------------------------------------\n\n");
		get_path_or_filename(szFileName);
		get_path_or_filename(szFileName2);

		nRet = update_trans_to_db_from_files(szFileName, szFileName2, nOldTrans);
	}
	else if ( nUpdateType == 3 ){
		if ( szChinese[0] == '\0' || szTranslate[0] == '\0' ){
			write_log(LT_BOTH,"Error: Please specify the chinese string after");
			write_log(LT_BOTH,"'-C' or translate string after '-T'!\n");
			return 0;
		}
		
		write_log(LT_BOTH,"Chinese string:\n\t'%s'\n", szChinese);
		write_log(LT_BOTH,"Translate string:\n\t'%s'\n", szTranslate);
		write_log(LT_BOTH,"--------------------------------------------------------\n\n");
		get_path_or_filename(szChinese);
		get_path_or_filename(szTranslate);
		
		nRet = update_trans_to_db(szChinese, szTranslate);
	}
	else {
		write_log(LT_BOTH,"Error: Please use '-f' or '-c' or '-C' specify the update type!\n");
		return 0;
	}

	return nRet;
}

int
traverse_database(char* argv[]){
	char	szChineseFileName[MAX_PATH];
	char	szTransFileName[MAX_PATH];
	char	szOperation[32];
	int	nIdx = 2;
		
	szChineseFileName[0] = '\0';
	szTransFileName[0] = '\0';

	while ( argv[nIdx] != NULL ){
		if ( strncmp(argv[nIdx], "-c=", 3) == 0 ){
			strcpy(szChineseFileName, &argv[nIdx][3]);
			szChineseFileName[MAX_PATH-1] = '\0';
		}
		else if ( strncmp(argv[nIdx], "-t=", 3) == 0 ){
			strcpy(szTransFileName, &argv[nIdx][3]);
			szTransFileName[MAX_PATH-1] = '\0';
		}
		else if ( strncmp(argv[nIdx], "-d=", 3) == 0 ){
			strcpy(szOperation, &argv[nIdx][3]);
			szOperation[sizeof(szOperation) - 1] = '\0';
		}
		else{
			write_log(LT_BOTH,"Error: invalid option '%s'\n", argv[nIdx]);
			return 0;
		}
		nIdx ++;
	}

	if ( szChineseFileName[0] == '\0' ){
		write_log(LT_BOTH,"Error: please specify the chinese file after '-c'\n");
		write_log(LT_BOTH,"from which you want to import to database!\n");
		return 0;
	}
	
	if ( szTransFileName[0] == '\0' ){
		write_log(LT_BOTH,"Error: please specify the translate file after '-t'\n");
		write_log(LT_BOTH,"from which you want to import to database!\n");
		return 0;
	}

	if ( szOperation[0] == '\0' ){
		write_log(LT_BOTH, "Error: please specify the Operation after '-d'\n");
		return 0;
	}

	write_log(LT_BOTH,"Current task:\n");
	write_log(LT_BOTH,"--------------------------------------------------------\n");
	write_log(LT_BOTH,"traverse the data in database to do '%s'\nresult will write to:\n"
		  "chinese: %s\ntranslate: %s\n", szOperation, szChineseFileName, szTransFileName);
	write_log(LT_BOTH,"--------------------------------------------------------\n");

	if ( db_init(0) == 0 ){
		write_log(LT_BOTH,"Error: Can't init 'database operation' module!\n");
		return 0;
	}

	traverse_to_do_something(szChineseFileName, szTransFileName, szOperation);

	return 1;
}

typedef	int 	(*CMD_EXEC_FUNC)(char* argv[]);
typedef struct{
	char		szCommand[32];
	CMD_EXEC_FUNC	pFunc;
}CMMD_STRUCT;

const CMMD_STRUCT g_CmdFunc[] = {
	{"getstring",	get_string},		/* 遍历一个目录下的所有文件抽取出所有需要翻译的中文字符串并把每一条记录插入到字典数据库中 */
	{"translate",	translate_chinese},	/* 遍历一个目录下的所有文件抽取出所有需要翻译的中文字符串并从字典数据库中获取其对应的翻译对其翻译, */
	{"update"   ,	update_translate},	/* 更新文件中的翻译到字典数据库中 */
	{"query"    ,	query_database},	/* 查询字典获取某一条中文的翻译 */
	{"traverse" ,	traverse_database},	/* 遍历数据库里面的每一条记录,对其执行一些字典的检查或转换操作 */
};

int
Exec_Command(char* Argv[]){
	int	i;
	int	nSize;

	nSize = sizeof(g_CmdFunc)/sizeof(CMMD_STRUCT);

	for ( i = 0; i < nSize; i++ )
		if ( strcmp(Argv[1], g_CmdFunc[i].szCommand) == 0 )
			return g_CmdFunc[i].pFunc(Argv);

	write_log(LT_SCREEN,"Error: invalid command '%s'\n", Argv[1]);
	return 0;
}

/*
int
main(int nArgc, char* Argv[]){
	time_t	nStartTm, nEndTm;
	int	nRet;

	g_logFile = fopen("log.txt", "a");
	if ( g_logFile == NULL ){
		write_log(LT_SCREEN,"Error: Can't open log file!\n");
		goto exit;
	}

	nStartTm = time(NULL);
	if ( nArgc == 1 ){
		write_log(LT_SCREEN,"Error: the number of argument is error\n");
		goto exit;
	}
	else if ( nArgc == 2 ){
		if ( strcmp(Argv[1], "-h") == 0 ){
			show_help();
			return 0;
		}
		else if ( strcmp(Argv[1], "-v") == 0 ){
			show_version();
			return 0;
		}
		else {
			write_log(LT_SCREEN,"Error: invalid option '%s'\n", Argv[1]);			
			goto exit;
		}
	}
	
	if (Exec_Command(Argv) == 0)
		goto exit;
	
	nRet = 0;
	goto exit2;
 exit:
	nRet = 1;
	write_log(LT_SCREEN,"Please add '-h' option to get the detail help!\n");

 exit2:
	nEndTm = time(NULL);
	write_log(LT_BOTH,"\n--------------------------------------------------------\n");
	write_log(LT_BOTH,"Task end, total use time: %d min, %d sec\n",
	       (int)(nEndTm - nStartTm)/60, (int)(nEndTm - nStartTm)%60);

	db_uninit();
	str_operate_uninit();

	SAFE_CLOSE_FILE(g_logFile);

	return nRet;
}*/
