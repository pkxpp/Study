#include        <stdio.h>
#include	<string.h>
#include	<stdlib.h>

#ifndef WIN32			  /* linux header files */
#include        <sys/types.h>	  /* these header files */
#include        <sys/stat.h>	  /* is used to visit directory */
#include        <dirent.h>	  /* recursively */
#else				  /* windows header files */
#include	<io.h>
#include	<Windows.h>
#include	<direct.h>
#endif //WIN32		

#include	"str_operate.h"
#include 	"db_operate.h"

#define	 IGNORE_DIR_CFG		"ignore_dir.conf"
#define  IGNORE_FILE_CFG	"ignore_file.conf"
#define	 SPECIAL_SIGN_CFG	"sign_table.conf"

#ifndef WIN32
#define   str_nocase_cmp(a, b) (strcasecmp(a, b))
#define   get_path_name(pPath) (strrchr(pPath, '/')) 
#else
#define	  str_nocase_cmp(a, b)  (stricmp(a, b))
#define   get_path_name(pPath) (strrchr(pPath, '\\')) 
#endif


FILE* g_strFile 	= NULL;   /* record all normal chinese string which is not in database*/
FILE* g_pathFile 	= NULL;	  /* record all path string which include chinese string */
FILE* g_fileNameFile 	= NULL;	  /* record all file name which include chinese string */
FILE* g_errFile	 	= NULL;	  /* record all error string which is recognized by this system */

char g_szOrginalRoot[MAX_PATH];	  /* the root path you want to get the chinese string */
char g_szTranslateRoot[MAX_PATH]; /* the root path you want to save the translated files */

char g_szCurFilterFile[MAX_PATH]; /* record the current filtrating file name */
char g_szNotFilterRoot[MAX_PATH]; /* record the current not filtrating root path */
int  g_nFinishRemark 	= 1;	  /* record current line of c file which is filtering is a remark */
int  g_nCurLine 	= 0;	  /* record the current line number of the filter buffer */
int  g_nTranslate	= 0;	  /* sign whether there is need to translate the chinese draw out from files to files */
int  g_nCurPos 		= 0;	  /* record the current position(the column number of the buffer starts )
				     of the  filter buffer */

int  g_nCurFilterType   = 0;	  /* the current filtrating file's type */
int  g_nNotFiltrateDir	= 0;	  /* whether use filtrate function to filter the current dir */
int  g_nProcessFileNum  = 0;	  /* record total processed file number */
int  g_nRightCount	= 0;      /* record the count of database operation's result is right */
int  g_nWrongCount	= 0;      /* record the count of database operation's result is wrong */
int  g_nRepeatCount	= 0;      /* record the count of database operation's result is repeat */

#define MAX_SPECIAL_SIGN_NUM 64
char	g_szSpecialSignTable[MAX_SPECIAL_SIGN_NUM][8];

#define MAX_IGNORE_NUM		128
#define MAX_IGNORE_COL_NUM 	8
char g_ignoreDir[MAX_IGNORE_NUM][MAX_PATH];		/* specify directory which no need to filtrate */
int  g_ignoreDirProp[MAX_IGNORE_NUM];			/* specify directory which no need to filtrate but is need to ignore */
char g_ignoreFile[MAX_IGNORE_NUM][MAX_PATH];		/* specify file which no need to filtrate */
int  g_ignoreCol[MAX_IGNORE_NUM][MAX_IGNORE_COL_NUM]; 	/* every ignore file can specify 8 colum not filtrate */
int  g_ignoreIdx = -1;					/* if current processing file need to ignore some columns
							   record its index in g_ignoreFile*/

const filtrate_buf g_FilterFunc[] = { /* filter function for each type of buffer */
	NULL,
	filter_tab_buffer,	  /* filtrate chinese from *.txt (format: tab) files */
	filter_ini_buffer,	  /* filtrate chinese from *.ini (format: ini) files */
	filter_lua_buffer,	  /* filtrate chinese from *.lua (fromat: lua) files */
	filter_cpp_buffer,	  /* filtrate chinese from *.c *.cpp *.h files  */
};

extern void write_log(int nLogType, char* pLogFormat, ...);

int
str_operate_init(const char* pcszOrginalRoot, const char* pcszTransRoot)
{
	if ( load_ignore_dir() == 0 || load_ignore_file() == 0 || load_special_sign_table() == 0 )
		return 0;

	if ( pcszOrginalRoot == NULL || pcszOrginalRoot[0] == '\0' )
		return 0;

	strcpy(g_szOrginalRoot, pcszOrginalRoot);

	/* if the translate root path is not null,then the current work is to translate */
	if ( pcszTransRoot != NULL && pcszTransRoot[0] != '\0' ) {
		strcpy(g_szTranslateRoot, pcszTransRoot);
		g_nTranslate = 1;
	}

	/* prepare opening all log file for writing */
	g_strFile 	= fopen("str_chinese.txt", "w");
	
	if ( g_nTranslate == 1 )
		return (g_strFile != NULL);

	g_pathFile 	= fopen("str_path.txt", "w");
	g_fileNameFile 	= fopen("str_filename.txt", "w");
	g_errFile 	= fopen("str_error.txt", "w");
	
	return (g_strFile && g_pathFile &&
		g_errFile && g_fileNameFile);
}

void
str_operate_uninit(){
	/* close all log file when exit */
	SAFE_CLOSE_FILE(g_strFile);
	SAFE_CLOSE_FILE(g_pathFile);
	SAFE_CLOSE_FILE(g_errFile);
	SAFE_CLOSE_FILE(g_fileNameFile);
}

void
record_string(int nStrType, char* pBuffer, int nStrLen){
	FILE* 		pFileTmp = NULL;
	char		szBufInfo[MAX_BUF_2048];
	int		nLen;
	
	switch( nStrType ){
	case STR_NORMAL:
		pFileTmp = g_strFile;
		break;
	case STR_PATH:
		pFileTmp = g_pathFile;
		break;
	case STR_ERROR:
		pFileTmp = g_errFile;
		break;
	case STR_FILE_NAME:
		pFileTmp = g_fileNameFile;
		break;
	}

	if ( pFileTmp == NULL )
		return;

	fwrite(pBuffer, 1, nStrLen, pFileTmp);
	fwrite("\n", 1, 1, pFileTmp);

	/* record the string position info
	   when it is recognized as a non-normal string */
	if ( nStrType != STR_NORMAL ){
		nLen = get_buffer_info(szBufInfo);
		fwrite(szBufInfo, 1, nLen, pFileTmp);
		fwrite("\n", 1, 1, pFileTmp);
	}
}

/* please ensure pBufferInfo has alloced */
int
get_buffer_info(char* pBufferInfo){
	int  	nStrLen;
	
	nStrLen = sprintf(pBufferInfo, "at file: %s, line: %d, column: %d",
			  g_szCurFilterFile, g_nCurLine, g_nCurPos);

	return nStrLen;
}

int
is_ini_section(char* pBuf){
	if ( is_left_square_bracket(pBuf[0]) ){
		if ( strchr(pBuf, ']') != NULL )
			return 1;
		else if ( strchr(pBuf, '=') == NULL )
			return 2;/*error*/
		else
			return 0; /*it is possible a key*/
	}
	return 0;
}

int
is_gbk_chinese(unsigned char cFirst, unsigned char cSecond){
	if (cFirst >= 0xb0 && cFirst <= 0xf7 &&		
	    cSecond >= 0xa1 && cSecond <= 0xfe)
		return 1;
	return 0;
}

/* note that 'pBuffer[nStrLen]' must be a '\0', or there will run error! */
int
get_string_type(char* pBuffer, int nStrLen){
	char* pStr1, *pStr2, *pStr3, c;

	/*when the string length exceed the max path length,
	  it's not a path and a filename*/
	if ( nStrLen >= MAX_PATH )
		return STR_NORMAL;

	pStr1 = strrchr(pBuffer, '.');
	pStr2 = strchr(pBuffer, '\\');

	if ( pStr1 == NULL && pStr2 == NULL )
		return STR_NORMAL;

	/* list follws is not a valid part of path */
	if ( strchr( pBuffer, ':' )  ||
	     strchr( pBuffer, '?' )  ||
	     strchr( pBuffer, '"' )  ||
	     strchr( pBuffer, '/' )  ||
	     strchr( pBuffer, '<' )  ||
	     strchr( pBuffer, '>' )  ||
	     strchr( pBuffer, '*' ))
		return STR_NORMAL;
	
	if ( pStr1 != NULL ){
		pStr1++;
		/* '.' is at the end of the string
		   that it is a normal string */
		if ( *pStr1 == '\0' )
			return STR_NORMAL;
		
		/* extent name must be compose of digit or letter*/
		do{
			if ( (is_digit(*pStr1)) ||
			     (is_letter(*pStr1)) )
				pStr1++;
			else
				return STR_NORMAL;
		}while ( *pStr1 != '\0' );
		
		return STR_FILE_NAME;
	}
	else{	/* note: here pStr2 != NULL */
		/*if after '\' there is special letter,
		  then it maynot be path*/
		pStr2 ++;
		if ( *pStr2 == '\0' )
			return STR_NORMAL;
		
		do{
			c = *(pStr2);			
			if( c == 'n' || c == 'r' || c == 't')
				return STR_NORMAL;

			pStr3 = pStr2 + 1;
			pStr2 = strchr(pStr3, '\\');
		}while ( pStr2 != NULL );
		
		return STR_PATH;
	}
}

/* notice: nStrLen will be changed at this function
   这个函数用来对旧的工具抽取的字符串和对应的翻译进行处理,
   使其可以拿来翻译现在的工具抽取出来的中文字符串,它首先
   去掉中文和对应的翻译在开头和结尾相同并且不重要的字符
   然后对中文字符串进行重新抽取处理,使其可以直接使用 */
int
process_old_chinese_and_trans(char* pChinese, char* pTrans,
			     int* nChienseHeadLen, int* nTransHeadLen){
	char	*pChineseHead, *pChineseTail;
	char	*pTransHead, *pTransTail;
	int	nStrLen;
	int	nRet = 1;
	char*	pTemp = NULL;

	nStrLen = (int)strlen(pChinese);
	pChineseTail = &pChinese[nStrLen-1];
	nStrLen = (int)strlen(pTrans);
	pTransTail = &pTrans[nStrLen-1];

	pChineseHead = pChinese;
	pTransHead = pTrans;

	while( *pChineseHead != '\0' && *pTransHead != '\0' ){
		if ( is_ignore_sign2((unsigned char)(*pChineseHead)) ){
			nRet = (*pChineseHead == *pTransHead);
		
			if ( nRet == 0 )
				break;
			
			pChineseHead++;
			pTransHead++;
		}
		else
			break;
	}

	while( pChineseTail > pChinese && pTransTail > pTrans ){
		if ( is_ignore_sign2((unsigned char)(*pChineseTail)) ){
			nRet = (*pChineseTail == *pTransTail);

			if ( nRet == 0 )
				break;

			pChineseTail--;
			pTransTail--;
		}
		else
			break;
	}

	*(pChineseTail+1) = '\0';
	*(pTransTail+1) = '\0';

	*nChienseHeadLen = (int)(pChineseHead-pChinese);
	*nTransHeadLen = (int)(pTransHead-pTrans);

	nStrLen = (int)strlen(pChineseHead);
	pTemp = strip_ignore_sign(pChineseHead, &nStrLen);
	*nChienseHeadLen += (int)(pTemp - pChineseHead);
	*(pTemp + nStrLen) = '\0';

	return nRet;
}

// notice: nStrLen will be changed at this function
char*
strip_ignore_sign(char* pBuffer, int* nStrLen){
	char	*pHead, *pTail;

	pTail = &pBuffer[*nStrLen-1];
	
	// strip non-chinese string at tail
	while ( pTail > pBuffer ) {
		if ( is_gbk_chinese_space((unsigned char)(*pTail), (unsigned char)(*(pTail-1)))){
			pTail -= 2;
			continue;
		}
		
		if ( !is_ignore_sign((unsigned char)(*pTail)) )
			break;
		
		pTail --;
	}
	
	// strip non-chinese string at head 
	pHead = pBuffer;
	while( *pHead != '\0' ){
		if ( is_gbk_chinese_space((unsigned char)(*pHead), (unsigned char)(*(pHead+1)))){
			pHead += 2;
			continue;
		}

		if ( !is_ignore_sign((unsigned char)(*pHead)) )
			break;

		pHead ++;
	}
	*nStrLen = (int)(pTail-pHead+1);
	
	return pHead;
}

/* 针对繁体big5编码的特殊处理,因为有些字的big5编码中会含有'\'编码,
   而这个字符在c中是转义字符,从而会导致程序或脚本编译错误,需要在后面
   自动补一个'\'将自己转义为'\',不过这个功能现在已经不需要了,因为lua
   加了对中文的支持,另外程序里面的字符串也都抽取到了文件里面,所以不再
   存在这个问题*/
void
process_for_tradional_chinese( char* pTransBuff ){
	char	szTemp[MAX_TRANS_LEN];
	char*	pTemp = NULL;
	int	nCurPos = 0;

	if ( strchr(pTransBuff, '\\') == NULL || strlen(pTransBuff) < 2 )
		return;

	memset(szTemp, 0, sizeof(szTemp));
	pTemp = pTransBuff;
	while ( *pTemp != '\0' && *(pTemp+1) != '\0' )
	{
		if ( *(pTemp+1) == '\\' && 
			(unsigned char)(*pTemp) >= 0xA1 &&
			(unsigned char)(*pTemp) <= 0xF9 ){
				szTemp[nCurPos++] = *pTemp;
				szTemp[nCurPos++] = *(pTemp+1);
				szTemp[nCurPos++] = '\\';
				pTemp += 2;
			}
		else{
			szTemp[nCurPos++] = *pTemp;
			pTemp ++;
		}
	}

	if ( *pTemp != '\0' )
		szTemp[nCurPos++] = *pTemp;

	strcpy(pTransBuff, szTemp);
}

/* 对各种类型的文件buffer抽取出来的中文字符串进行集中处理 */
int
string_process(char* pBuffer, int nStrLen, int bError){
	int 		nStrType, nRet, nRet2;
	char		szTranslate[MAX_TRANS_LEN];
	char		cOld1, cOld2, *pChineseStr, *pTemp;
	int         	nHaveChinese = 0;
	int		nStrLen2;

	if ( pBuffer == NULL || *pBuffer == '\0' )
		return 1;

	memset(szTranslate, 0, MAX_TRANS_LEN);
	cOld1 = pBuffer[nStrLen];
	cOld2 = '\0';
 	pBuffer[nStrLen] = '\0';
	nRet = nStrLen+1;

	pChineseStr = pBuffer;
	nStrLen2 = nStrLen;
	if ( bError == 1 ){
		/* if this string is a error string, judge
		   there is chinese string it, if have then
		   write record to file or not;
		*/
		nStrType = STR_ERROR;
		pTemp = pBuffer;
		while ( *(pTemp+1) != '\0' ){
			if ( is_gbk_chinese(*pTemp, *(pTemp+1)) ){
				nHaveChinese = 1;
				break;
			}
			pTemp++;
		}
		if ( nHaveChinese == 0 )
			goto exit;
	}
	else{
		nStrType = get_string_type(pBuffer, nStrLen);
	}		

	if ( nStrType != STR_NORMAL ) {
		/*在抽取字符串时,非需要翻译的字符串直接记录在对应的文件里即可 */
		if ( g_nTranslate == 0 )
			record_string(nStrType, pChineseStr, nStrLen2);
		goto exit;
	}

	/* 对于需要翻译的中文字符串,先去除前后的非关键字符 */
	pChineseStr = strip_ignore_sign(pBuffer, &nStrLen2);
	cOld2= pChineseStr[nStrLen2];
	pChineseStr[nStrLen2] = '\0';

	if (pChineseStr[0] == '\0')
		goto exit;

	if ( g_nTranslate == 0 ){
		/* 如果不需要翻译,那么将这个字符串插入到字典数据库中 */
		nRet2 = insert_notrans_to_db(pChineseStr);
		if ( nRet2 == 1 ){
			/* 返回值为1,则这个字符串字典中尚不存在需要翻译,那么记录下来 */
			record_string(nStrType, pChineseStr, nStrLen2);
			g_nRightCount ++;
		}
		else if ( nRet2 == 0 ) /* 查找字典时出错 */
			g_nWrongCount ++;
		else if ( nRet2 == 2) /* 字典中已存在 */
			g_nRepeatCount ++;
	}
	else{
		nRet2 = get_trans_from_db(pChineseStr,
					  szTranslate, MAX_TRANS_LEN);
		if ( nRet2 == 0 ){
			g_nWrongCount ++;
			record_string(nStrType, pChineseStr, nStrLen2);
			goto exit;
		}
		else if ( nRet2 == 1 )
			g_nRightCount ++;

		nRet = (int)strlen(szTranslate) + 1;
		if ( (nStrLen != nStrLen2) && (cOld2 != '\0') ){
			szTranslate[nRet-1] = cOld2;
			if ( *(pChineseStr+nStrLen2+1) != '\0' )
				memcpy(szTranslate + nRet, pChineseStr + nStrLen2 + 1,
				       (int)strlen(pChineseStr + nStrLen2 + 1));
			nRet = (int)strlen(szTranslate) + 1;
		}
			
		szTranslate[nRet - 1] = cOld1;
		if ( cOld1 != '\0' )
			memcpy(szTranslate + nRet, pBuffer + nStrLen + 1,
			       (int)strlen(pBuffer + nStrLen + 1));

		strcpy(pChineseStr, szTranslate);
		nRet += (int)(pChineseStr-pBuffer);	
		goto exit2;
	}
exit:	
	pBuffer[nRet-1] = cOld1;
	if ( cOld2 != '\0' )
		pChineseStr[nStrLen2] = cOld2;
 exit2:	
	return nRet;
}

int
get_file_type(const char* pcszFileName){
	char	*pStr = NULL;

	pStr = strrchr((char*)pcszFileName, '.');
	if (pStr == NULL)
		return FILE_UN_KNOWN;

	pStr ++;
	if (*pStr == '\0')
		return FILE_UN_KNOWN;
	
	if ( str_nocase_cmp(pStr, "txt") == 0 )
		return FILE_TAB;
	else if ( str_nocase_cmp(pStr, "ini") == 0 ||
			  str_nocase_cmp(pStr, "cfg") == 0 )
		return FILE_INI;
	else if ( str_nocase_cmp(pStr, "lua") == 0 )
		return FILE_LUA;
	else if ( str_nocase_cmp(pStr, "cpp") == 0 ||
		  str_nocase_cmp(pStr, "c")   == 0 ||
		  str_nocase_cmp(pStr, "h")   == 0 )
		return FILE_CPP;
	else
		return FILE_UN_KNOWN;
}

int
filter_chinese_in_file(const char* pcszFileName){
	char		szBuffer[MAX_LINE_LEN];
	char		szFileName[MAX_PATH];
	int		nStrLen, nPos, nType;
	FILE*		pOriginalFile  = NULL;
	FILE*		pTranslateFile = NULL;
	int		nIsANewLineEnd = 0;
	int		nFiltrate = 1;
	int		nRet, i;
	char		szTemp[64];
	char		szTemp2[8];

	g_ignoreIdx = -1;
	nRet = is_ignored_file(pcszFileName);
	if ( nRet >= 0 ){
		if ( g_ignoreCol[nRet][0] > 0 ){
			g_ignoreIdx = nRet;
			memset(szTemp, 0, sizeof(szTemp));
			for (i = 0; i < MAX_IGNORE_COL_NUM; i ++)
			{
				if ( g_ignoreCol[nRet][i] == 0 )
					break;
				sprintf(szTemp2, "%d ", g_ignoreCol[nRet][i]);
				strcat(szTemp, szTemp2);
			}
			write_log(LT_FILE, "Info: File %s's %scolumn is ignored!\n",
				pcszFileName, szTemp);
		}
		else if ( g_ignoreCol[nRet][0] == -1 ){
			write_log(LT_FILE, "Info: File %s is ignored!\n",
				pcszFileName);
			return 1;
		}
		else{
			write_log(LT_FILE, "Info: File %s will not filtrate!\n",
				pcszFileName);
			nFiltrate = 0;
		}
	}

	if ( g_nNotFiltrateDir == 1 )
		nFiltrate = 0;

	nType = get_file_type( pcszFileName );
	if ( nType == FILE_UN_KNOWN ){
		write_log(LT_FILE, "Error: Unknown file type at file %s\n", 
			pcszFileName);
		nFiltrate = 0;
	}

	if ( g_nTranslate == 0 && nFiltrate == 0 )
		return 1;

	pOriginalFile = fopen( pcszFileName, "r" );
	if ( pOriginalFile == NULL ){
		write_log(LT_BOTH, "Error: Can't open file: %s\n", 
			pcszFileName);
		return 0;
	}

	/* if need translate, then open the translate file waiting for write */
	if ( g_nTranslate == 1 ){
		sprintf(szFileName, "%s%s", g_szTranslateRoot,
		        pcszFileName+strlen(g_szOrginalRoot));

		pTranslateFile = fopen(szFileName, "w");
		if ( pTranslateFile == NULL ){
			SAFE_CLOSE_FILE(pOriginalFile);
			write_log(LT_BOTH, "Error: Can't open file: %s\n",
				szFileName);
			return 0;
		}
	}
	
	g_nCurLine = 0;
	szBuffer[0] = '\0';
	/* read and process the file line by line */
	do{
		/* if it is translating file now, 
		   then write the translated line to the corresponding file*/
		if ( g_nTranslate == 1 && g_nCurLine > 0 ){
			if ( szBuffer[0] != '\0' )
				fwrite(szBuffer, 1, strlen(szBuffer), 
				pTranslateFile);
			if ( nIsANewLineEnd == 1 )
				fwrite("\n", 1, 1, pTranslateFile);			
		}

		nIsANewLineEnd = 0;
		if ( fgets(szBuffer, MAX_LINE_LEN, pOriginalFile) == NULL )
			break;
		
		g_nCurLine ++;
		szBuffer[MAX_LINE_LEN - 1] = '\0';

		nStrLen = (int)strlen( szBuffer );
		/* clear the newline at the tail of the line */
		if ( szBuffer[nStrLen-1] == '\n' ||
		     szBuffer[nStrLen-1] == '\r' ){
			szBuffer[nStrLen-1] = '\0';
			nIsANewLineEnd = 1;
		}

		if ( nFiltrate == 0 )
		     continue;

		/* it is impossible a string which its length is 1 contain chinese string */
		if ( nStrLen <= 1 )
			continue;

		/* search the first character which is not space and tab */
		for( nPos = 0; nPos < nStrLen; nPos++){
			if ( is_tab(szBuffer[nPos]) || is_space(szBuffer[nPos]) )
				continue;
			if ( is_gbk_chinese_space((unsigned char)szBuffer[nPos],
				(unsigned char)szBuffer[nPos+1])){
				nPos++;
				continue;
			}
			break;
		}

		/* ignore empty line and single character line */
		if ( szBuffer[nPos] == '\0' || szBuffer[nPos+1] == '\0')
			continue;

		/* record the current filtrating file and its position */
		g_nCurPos = 0;
		strcpy( g_szCurFilterFile, pcszFileName );
		
		g_nCurFilterType = nType;
		/* filtrate the current line' chinese string and process it */
		g_FilterFunc[nType]( &szBuffer[nPos], 
			(int)strlen(&szBuffer[nPos]) );
	}while(1);

	SAFE_CLOSE_FILE(pOriginalFile);
	SAFE_CLOSE_FILE(pTranslateFile);
	
	g_nProcessFileNum ++;
	
	if ( (g_nProcessFileNum % VIEW_A_NUM) == 0 ){
		printf("+ ");
		fflush(stdout);
	}
	if ( (g_nProcessFileNum % VIEW_LINE_NUM) == 0  )
		printf("\n");

	return 1;
}

int
filter_tab_buffer(char* pBuffer, int nStrLen){
	char* 		pStr 	= NULL;
	char* 		pStr2 	= NULL;
	int		nLength = 0;
	int		nColNum = 1;
	int		i, isIgnored;
		
	g_nCurPos = 0;
	pStr = pBuffer;
	while( (*(pStr) != '\0') && (*(pStr+1) != '\0') ){
		if ( !is_ascii((unsigned char)(*pStr)) ){
			pStr2 = strchr(pStr, '\t');

			isIgnored = 0;
			if ( g_ignoreIdx > -1 ){ /* judge this column is ignored */
				for ( i = 0; i < MAX_IGNORE_COL_NUM; i++ ){
					if ( g_ignoreCol[g_ignoreIdx][i] == 0 )
						break;
					if ( g_ignoreCol[g_ignoreIdx][i] == nColNum ) {
						isIgnored = 1;
						break;
					}
				}
			}

			if ( pStr2 == NULL ){
				nStrLen = (int)strlen(pBuffer);
				nLength = nStrLen - g_nCurPos;
				if ( isIgnored == 0 )
					string_process(&pBuffer[g_nCurPos],
						       nLength, 0);
				break;
			}
			else{	
				nLength = (int)(pStr2 - pBuffer) - g_nCurPos;
				if ( isIgnored == 0 )
					nLength = string_process(
					&pBuffer[g_nCurPos], nLength, 0);
				else
					nLength++;
				
				pStr = pBuffer + g_nCurPos + nLength;
				g_nCurPos = (int)(pStr - pBuffer);
				nColNum ++;
			}

			continue;
		}
		else if ( is_tab(*pStr) ){
			g_nCurPos = (int)(pStr - pBuffer) + 1;
			nColNum ++;
		}

		pStr++;
	}
	
	return 1;
}

int
filter_ini_buffer(char* pBuffer, int nStrLen){
	int 		nRet;
	char* 		pStr = NULL;
	char*		pStr2 = NULL;
	char		cTail = '\0';

	g_nCurPos = 0;
	/* if the line is a remark, not process */
	if( is_ini_remark((unsigned char)pBuffer[0], (unsigned char)pBuffer[1]) )
		return 1;

	/* if the line is a section, not process */
	nRet = is_ini_section(pBuffer); 
	if( nRet == 1 ){
		pStr2 = strrchr(pBuffer, ']');
		*pStr2 = '\0';
		pStr = pBuffer;
		cTail = ']';
		goto PROCESS;
	}
	else  if( nRet == 2){
		string_process(pBuffer, (int)strlen(pBuffer), 1);
		return 0;
	}

	/* if the line is not a key,then it's error! */
	pStr = strchr(pBuffer, '='); 
	if ( pStr == NULL ){
		string_process(pBuffer, (int)strlen(pBuffer), 1);
		return 0;
	}

	/* if the line is a key,judge whether there is chinese in key value */
PROCESS:
	pStr += 1;
	pStr2 = pStr;
	if ( *pStr == '\0' )
		return 1;

	nRet = (int)strlen(pStr2)+1;
	while( (*(pStr) != '\0') && (*(pStr+1) != '\0') ){
		if ( !is_ascii((unsigned char)(*pStr)) ){
			nRet = string_process(pStr2, (int)strlen(pStr2), 0);
			break;
		}
		pStr ++;
	}

	if ( cTail != '\0' ){
		pStr2[nRet-1] = cTail;
		pStr2[nRet] = '\0';
	}

	return 1;
}

/* 针对lua中出现的对话选项后面紧跟其执行的函数名这种情况进行特殊处理,
   因为抽取出来的字符串往往包含了对话其后的函数名,需要去掉,否则一旦翻
   译的时候他们把函数名写错了,问题将非常大 */
int
get_tail_func_name_len(char* pBuffer, int nStrLen){
	char	cOld, *pStr;
	int	nLen = 0;
	int	nIsFuncName = 1;

	cOld = pBuffer[nStrLen];
	pBuffer[nStrLen] = '\0';

	pStr = strrchr(pBuffer, '/');
	if (pStr != NULL){
		nLen = nStrLen - (int)(pStr - pBuffer);
		pStr++;
		if ( *pStr != '#'){
			while ( *pStr != '\0' ){
				if ( (!is_letter(*pStr)) &&
				     (!is_digit(*pStr)) &&
				     (!is_ungderline(*pStr))&&
				     (!is_bracket(*pStr)) ){
					nIsFuncName = 0;
					break;
				}
				pStr++;
			}
		}
		if ( nIsFuncName == 0 )
			nLen = 0;
	}

	pBuffer[nStrLen] = cOld;
	return nLen;
}

/*判断是否是被转义的cQuotation( cQuotation为 '"' or '\'' )*/
int
is_escape_quotation(char* pStr, char cQuotation)
{
	int nBiasNum = 0;
	if (*pStr != cQuotation)
		return -1;				/* 不是引号 */
	else {
		while ( *(--pStr) == '\\' )
			nBiasNum++;
	}
	if (nBiasNum % 2 == 0)
		return 0;				/* 真的引号 */
	else
		return nBiasNum;		/* 被转义的引号，返回'\\'的个数 */
}

int
filter_lua_buffer(char* pBuffer, int nStrLen){
	char* 	pStr 	= NULL;
	char*	pStr2 	= NULL;
	char*	pOld 	= NULL;
	int		nLength = 0;
	int		nDoubleCount = 0;
	int		nSingleCount = 0;
	int		nTailLen = 0;
	int		bError = 0;
	int		nBiasNum = 0;
	char	cQuotation, cOld;

	g_nCurPos = 0;
	
	/* if the line is a remark, not process */
	if( is_lua_remark(pBuffer[0], pBuffer[1]) )
		return 1;

	pStr = pBuffer;
	while( (*(pStr) != '\0') && (*(pStr+1) != '\0') ){
		/* stop when encounter lua remark */
		if ( is_lua_remark(*pStr, *(pStr+1)) )
			break;

		/* when encounter a chinese,
		   draw out the entire string from its start after '"'
		   to its end before the next '"' */
		if ( !is_ascii((unsigned char)(*pStr)) ){
			if ( (nSingleCount % 2) == (nDoubleCount % 2) )
				bError = 1;
			else if ( nDoubleCount % 2 )
				cQuotation = '"';
			else if ( nSingleCount % 2 )
				cQuotation = '\'';

			if ( bError != 1 ){
				pStr2 = strchr(++pStr, cQuotation);
				while ( pStr2 != NULL && is_escape_quotation( pStr2, cQuotation ) > 0 ){
					pStr2 = strchr(++pStr2, cQuotation);
				}
			}

			if ( pStr2 == NULL )
				bError = 1;

			if ( bError == 1 ){
				nStrLen = (int)strlen(pBuffer);
				nLength = nStrLen - g_nCurPos;
				if ( nLength > 0 )
					string_process(&pBuffer[g_nCurPos],
						     nLength, 1);
				break;
			}
			else{
				nLength = (int)(pStr2 - pBuffer) - g_nCurPos;

				if ( cQuotation == '"' )
					nDoubleCount ++;
				else if ( cQuotation == '\'' )
					nSingleCount ++;

				nTailLen = get_tail_func_name_len(
					&pBuffer[g_nCurPos], nLength);
				nLength = string_process(&pBuffer[g_nCurPos],
						      nLength-nTailLen, 0);

				if ( nTailLen >= 2 ){
					pStr = pBuffer + g_nCurPos + nLength;
					pOld = pStr2;

					while ( ++pStr < pOld ){
						if ( ( is_escape_quotation( pStr, cQuotation ) == 1 ) || 
							( ( is_single_quotation(*pStr) || is_double_quotation(*pStr) ) && *pStr != cQuotation ) ){

							pStr2 = strchr( (pStr + 1), *pStr );
							while ( pStr2 != NULL ){
								if ( *pStr2 != cQuotation )
									break;
								else if ( is_escape_quotation( pStr2, cQuotation ) == 1 )
									break;

								pStr2 = strchr( ++pStr2, *pStr );
							}

							if ( pStr2 == NULL ){
								nStrLen = (int)strlen(pBuffer);
								nLength = nStrLen - g_nCurPos;
								if ( nLength > 0 )
									string_process(&pBuffer[g_nCurPos],
											nLength, 1);
								return 1;
							}

							*pStr2 = '\0';
							if ( is_there_gbk_code(pStr) ){
								*pStr2 = *pStr;
								string_process(&pBuffer[(int)(pStr - pBuffer) + 1],
									((int)(pStr2 - pStr) - 1), 0);
							}

							pStr = pStr2;
						}
					}
				}

				nLength += nTailLen;

				pStr = pBuffer + g_nCurPos + nLength;
				g_nCurPos = (int)(pStr - pBuffer);
			}
			
			continue;
		}
		// only a string starts at '"' maybe a string contain chinese
		// record the position after the '"'
		// as the start of possible chinese string
		else if( (nSingleCount % 2) == 0 && is_escape_quotation( pStr, '"' ) == 0 ){
			if( (++nDoubleCount) % 2 )
				g_nCurPos = (int)(pStr - pBuffer) + 1;
		}
		else if( (nDoubleCount % 2) == 0 && is_escape_quotation( pStr, '\'' ) == 0 ){
			if( (++nSingleCount) % 2 )
				g_nCurPos = (int)(pStr - pBuffer) + 1;
		}
		
		pStr++;
	}
	
	return 1;
}

int
filter_cpp_buffer(char* pBuffer, int nStrLen){
	char* 		pStr	 = NULL;
	char*		pStr2 	 = NULL;
	int		nLength  = 0;
	int		nCount	 = 0;
	int		bError	 = 0;

	//if the line is a cpp remark, not process
	if( is_cpp_remark(pBuffer[0], pBuffer[1]) )
		return 1;

	if( is_c_remark(pBuffer[0], pBuffer[1]) ){
		if( strstr(pBuffer, "*/") == NULL )
			g_nFinishRemark = 0;
		return 1;
	}

	if( g_nFinishRemark == 0 ){
		if( strstr(pBuffer, "*/") )
			g_nFinishRemark = 1;
		return 1;
	}

	pStr = pBuffer;
	while( (*(pStr) != '\0') && (*(pStr+1) != '\0') ){
		if( is_cpp_remark(*pStr, *(pStr+1)) ||
		    is_c_remark(*pStr, *(pStr+1)))
			break;
		
		if ( !is_ascii((unsigned char)(*pStr)) ){
			
			pStr2 = strchr(pStr, '"');
		
			if ( pStr2 == NULL )
				bError = 1;
			else if ((nCount%2) == 0){
				if ( *(pStr2-1) != '\\' )
					bError = 1;
			}

			if ( bError == 1 ){
				nStrLen = (int)strlen(pBuffer);
				nLength = nStrLen - g_nCurPos;
				if ( nLength > 0 )
					string_process(&pBuffer[g_nCurPos],
						       nLength, 1);
				break;
			}
			else{	
				nLength = (int)(pStr2 - pBuffer) - g_nCurPos;
				nLength = string_process(&pBuffer[g_nCurPos],
						      nLength, 0);
				nCount ++;
				pStr = pBuffer + g_nCurPos + nLength;
				g_nCurPos = (int)(pStr - pBuffer);
			}
			
			continue;
		}
		else if( is_double_quotation(*pStr) ){
			nCount ++;
			if( nCount % 2 )
				g_nCurPos = (int)(pStr - pBuffer) + 1;
		}
		
		pStr++;
	}
	
	return 1;
}

int
visit_path_recursively(const char* pcszPath, operate_callback file_func,
		       operate_callback dir_func,
		       operate_callback dir_func2){
#ifndef WIN32
	struct stat		statbuf;
	struct dirent		*dirp;
	DIR			*dp;
	char			szPath[MAX_PATH];
	int			nRet = 1;
	
	lstat(pcszPath, &statbuf);

	/* if it is a file, use file operation
	   callback function to process it */
	if (S_ISDIR(statbuf.st_mode) == 0){
		if ( file_func != NULL ){
			nRet = file_func( pcszPath );
			return nRet;
		}
		else
			return 1;
	}

	/* here it is qure a path, then try to open the directory */
	if ( (dp = opendir(pcszPath)) == NULL){
		write_log(LT_BOTH, "Error: Can't open directory: %s!\n",
			  szPath );
		return 0;
	}

	/* use directory operation callback function to process it */
	if ( dir_func != NULL ){
		nRet = dir_func(pcszPath);
		if ( nRet == 0 || nRet == 2 ){
			if ( nRet == 0 )
				write_log(LT_BOTH, "Error: when process %s using dir_func!\n", pcszPath);
			goto exit;
		}
	}

	/* Read the directory and visit its all child recursively */

	while ( (dirp = readdir(dp)) != NULL) {
		/* ignore dot and dot-dot */
		if (strcmp(dirp->d_name, ".") == 0  ||
		    strcmp(dirp->d_name, "..") == 0)
			continue;

		/* append name after slash */
		sprintf(szPath, "%s/%s", pcszPath, dirp->d_name);

		nRet = visit_path_recursively(szPath, file_func, dir_func, dir_func2);
		if ( nRet == 0 ){
			write_log(LT_BOTH, "Error: when process %s!\n", szPath);
			goto exit;
		}
		if ( dir_func2 != NULL ){
			nRet = dir_func2(szPath);
			if ( nRet == 0 ){
				write_log(LT_BOTH, "Error: when process %s using dir_func2!\n", szPath);				
				goto exit;
			}
		}
	}

 exit:
	/* close the directory which is opened before */
	if (closedir(dp) < 0)
		write_log(LT_BOTH, "Error: Can't close directory %s!\n", pcszPath);
	
	return nRet;
#else		//WIN32
	intptr_t		hFind;
	_finddata_t		FindData;
	char			szPath[MAX_PATH];
	int			nRet = 1;

    if ( SetCurrentDirectoryA(pcszPath) == 0 ){
		write_log(LT_BOTH, "Error: Can't set the current directory: %s!\n",
			  pcszPath);
		return 0;
	}

	hFind = _findfirst("*.*", &FindData);
	if (hFind == -1)
		return 0;

	do{
		if ( strcmp(FindData.name, ".") == 0 ||
		     strcmp(FindData.name, "..") == 0)
			continue;

		sprintf(szPath, "%s\\%s", pcszPath, FindData.name);
		if ( FindData.attrib & _A_SUBDIR ){
			if ( dir_func != NULL ){
				nRet = dir_func(szPath);
				if ( nRet == 0 )
					goto exit;
				else if ( nRet == 2 ){
					continue;
				}
			}

			nRet = visit_path_recursively(szPath, file_func, 
						      dir_func, dir_func2);
			if ( nRet == 0 )
				goto exit;
			
			if ( dir_func2 != NULL ){
				nRet = dir_func2(szPath);
				if ( nRet == 0 )
					goto exit;
			}
		}
		else{
            SetFileAttributesA(FindData.name, FILE_ATTRIBUTE_NORMAL);
			if ( file_func != NULL ){
				nRet = file_func( szPath );
				if ( nRet == 0 )
					goto exit;
			}
		}
	}while ( _findnext(hFind, &FindData) == 0 );

 exit:
	if ( _findclose(hFind) )
		write_log(LT_BOTH, "Error: Can't close directory %s!\n", 
			  pcszPath);
	
	return nRet;
#endif //WIN32
}

int
create_dir_for_translate(const char* pcszPath){
	int	nRet;
	int	nRootPathLen;

	if ( g_nTranslate == 0 )
		return 1;

	nRet = filtrate_ignore_dir(pcszPath);
	if ( nRet == 2 )
		return 2;

	nRootPathLen = (int)strlen(g_szOrginalRoot);
#ifndef WIN32
	struct stat	statbuf;
	char		szPath[MAX_PATH];
	
	sprintf(szPath, "%s%s", g_szTranslateRoot,
		pcszPath+nRootPathLen);
	
	lstat(szPath, &statbuf);

	if (S_ISDIR(statbuf.st_mode) == 1){
		write_log(LT_FILE, "Info: Directory:%s existed!\n", szPath);
		return 1;
	}

	nRet = mkdir(szPath, S_IRWXU | S_IRWXG);
	write_log(LT_BOTH, "create path: %s\n", szPath);
	return (nRet == 0);
#else
	char		szPath[MAX_PATH];
	char*		pStr = NULL;

	sprintf(szPath, "%s%s", g_szTranslateRoot,
		pcszPath+nRootPathLen);

    if ( SetCurrentDirectoryA(szPath) == 1 ){
		write_log(LT_FILE, "Info: Directory:%s existed!\n", szPath);
		return 1;
	}

	pStr = strrchr(szPath, '\\');
	*pStr = '\0';
	pStr ++;
    if ( SetCurrentDirectoryA(szPath) == 0 ){
		write_log(LT_FILE, "Error: Can't set the current directory: %s!\n",
			szPath);
		return 0;
	}

    return CreateDirectoryA(pStr, NULL);
#endif	
}

int
clear_ignore_sign(const char* pcszPath){
	if ( g_szNotFilterRoot[0] != '\0' && 
		strcmp(pcszPath, g_szNotFilterRoot) == 0 ){
			g_nNotFiltrateDir = 0;
			g_szNotFilterRoot[0] = '\0';
			write_log(LT_FILE, "Info: Leave Directory %s!\n", 
				pcszPath);
	}

	return 1;
}

int
filtrate_ignore_dir(const char* pcszPath){
	int	nIdx;

	nIdx = is_ignored_dir(pcszPath);
	if ( nIdx > -1 ){
		if ( g_ignoreDirProp[nIdx] == 0 ){
			write_log(LT_FILE, "Info: Directory %s is ignored!\n", 
				pcszPath);
			return 2;
		}
		else {
			write_log(LT_FILE, "Info: Directory %s will not filtrate!\n", 
				pcszPath);
			
			if ( g_nTranslate == 0 )
				return 2;

			if ( g_szNotFilterRoot[0] == '\0' ){
				g_nNotFiltrateDir = 1;
				strcpy(g_szNotFilterRoot, pcszPath);
			}
			
			return 1;
		}
	}

	return 1;
}

int
start_work(){
	operate_callback 	pFileFunc;
	operate_callback	pDirFunc;
	operate_callback	pDirFunc2;
	int			nRet;

	pFileFunc = filter_chinese_in_file;
	pDirFunc2 = clear_ignore_sign;
	if ( g_nTranslate == 1 )
		pDirFunc  = create_dir_for_translate;
	else
		pDirFunc = filtrate_ignore_dir;

	nRet = visit_path_recursively(g_szOrginalRoot, pFileFunc, pDirFunc, pDirFunc2);

	write_log(LT_BOTH, "\n--------------------------------------------------------\n");
	if ( nRet == 0 )
		write_log(LT_BOTH, "There is error in the processing,"
			"see log.txt for detail!\n");
	write_log(LT_BOTH, "Total process file number: %d\n", g_nProcessFileNum);

	if ( g_nTranslate == 0 ){
		if ( g_nRightCount > 0 )
			write_log(LT_BOTH,
			"Total %d records are inserted to database!\n",
			g_nRightCount);
		if ( g_nWrongCount > 0 )
			write_log(LT_BOTH,
			"Total %d records are failing to insert to database!\n",
			g_nWrongCount);
		if ( g_nRepeatCount > 0 )
			write_log(LT_BOTH,
			"Total %d records are repeated when insert them to database!\n",
			g_nRepeatCount);
	}
	else{
		if ( g_nRightCount > 0 )
			write_log(LT_BOTH,
			"Total %d chinese strings are translated!\n",
			g_nRightCount);
		if ( g_nWrongCount > 0 )
			write_log(LT_BOTH,
			"Total %d chinese strings aren't translated!\n",
			g_nWrongCount);
	}

	return nRet;
}

int
is_a_valid_directory(char* pcszDirectory){
#ifndef WIN32
	struct stat	statbuf;

	if ( pcszDirectory[0] == '\0' )
		return 0;
	
	lstat(pcszDirectory, &statbuf);

	if (S_ISDIR(statbuf.st_mode) == 1){
		return 1;
	}

	write_log(LT_BOTH, "Error: %s is not a valid path!\n", pcszDirectory);
	return 0;
#else
	char		szPath[MAX_PATH];
	int			nRet = 0;

    GetCurrentDirectoryA(MAX_PATH, szPath);
    if ( SetCurrentDirectoryA(pcszDirectory) )
		nRet = 1;
	
    SetCurrentDirectoryA(szPath);
	if ( nRet == 0 )
		write_log(LT_BOTH, "Error: %s is not a valid path!\n",
		pcszDirectory);

	return nRet;
#endif //WIN32
}

int
load_ignore_dir()
{
	FILE*		pFile = NULL;
	char		szBuffer[MAX_PATH];
	int		nStrLen;
	char		*pStart, *pEnd;
	int		nDirCount;
	
	pFile = fopen(IGNORE_DIR_CFG, "r");
	if ( pFile == NULL ){
		write_log(LT_BOTH, "Error: can't open file %s\n", IGNORE_DIR_CFG);
		return 0;
	}

	nDirCount = 0;
	memset(g_ignoreDir,  0, sizeof(g_ignoreDir));
	memset(g_ignoreDirProp, 0, sizeof(g_ignoreDirProp));
	memset(g_szNotFilterRoot, 0, sizeof(g_szNotFilterRoot));

	while ( fgets(szBuffer, MAX_PATH, pFile) != NULL ){
		szBuffer[MAX_PATH - 1] = '\0';
		nStrLen = (int)strlen(szBuffer);
		
		if ( szBuffer[0] == ';' )
			continue;

		if ( szBuffer[nStrLen-1] == '\n' ||
		     szBuffer[nStrLen-1] == '\r'){
			szBuffer[nStrLen-1] = '\0';
			nStrLen --;
		}

		if ( nStrLen == 0 )
			continue;

		pStart = szBuffer;
		while ( *pStart != '\0' ){
			if ( !is_tab(*pStart) && !is_space(*pStart) )
				break;
			pStart ++;
		}
		
		pEnd = &szBuffer[nStrLen - 1];
		while ( pEnd != pStart ){
			if ( !is_tab(*pEnd) && !is_space(*pEnd) )
				break;
			pEnd--;
		}
		*(pEnd+1) = '\0';
		
		if ( is_ignored_dir(pStart) > -1 ){
				write_log(LT_BOTH, "Error: file %s have exist"
					"in before line!\n", pStart);
				continue;
		}

		if ( (pEnd = strchr(pStart, '\t')) != NULL ){
			*pEnd = '\0';
			g_ignoreDirProp[nDirCount] = 1;
		}
		strcpy(g_ignoreDir[nDirCount++], pStart);
		
		if ( nDirCount >= MAX_IGNORE_NUM ){
			write_log(LT_BOTH, "Error: exceed the max setting"
				"number when load %s", IGNORE_DIR_CFG);
			break;
		}
	}

	SAFE_CLOSE_FILE(pFile);

	return 1;	
}

int
load_ignore_file(){
	FILE*		pFile = NULL;
	char		szBuffer[MAX_PATH];
	int		nStrLen,nColCount;
	char		*pStart, *pEnd;
	char		*pPrevTab, *pNextTab;
	int		nFileCount;
	int		nColNum;
	
	pFile = fopen(IGNORE_FILE_CFG, "r");
	if ( pFile == NULL ){
		write_log(LT_BOTH, "Error: can't open file %s\n", IGNORE_FILE_CFG);
		return 0;
	}

	nFileCount = 0;
	memset(g_ignoreFile, 0, sizeof(g_ignoreFile));
	memset(g_ignoreCol,  0, sizeof(g_ignoreCol));

	while ( fgets(szBuffer, MAX_PATH, pFile) != NULL ){
		szBuffer[MAX_PATH - 1] = '\0';
		nStrLen = (int)strlen(szBuffer);
		
		if ( szBuffer[0] == ';' )
			continue;

		if ( szBuffer[nStrLen-1] == '\n' ||
		     szBuffer[nStrLen-1] == '\r'){
			szBuffer[nStrLen-1] = '\0';
			nStrLen --;
		}

		if ( nStrLen == 0 )
			continue;

		pStart = szBuffer;
		while ( *pStart != '\0' ){
			if ( !is_tab(*pStart) && !is_space(*pStart) )
				break;
			pStart ++;
		}
		
		pEnd = &szBuffer[nStrLen - 1];
		while ( pEnd != pStart ){
			if ( !is_tab(*pEnd) && !is_space(*pEnd) )
				break;
			pEnd--;
		}
		*(pEnd+1) = '\0';
		

		pPrevTab = strchr(szBuffer, '\t');
		if ( pPrevTab == NULL ){
			if ( is_ignored_file(pStart) > -1 ){
				write_log(LT_BOTH, "Error: file %s"
					"have existed in before line!\n",
				       pStart);
				continue;
			}
			strncpy(g_ignoreFile[nFileCount++], pStart,
				(int)(pEnd-pStart)+1);
		}
		else {
			*pPrevTab = '\0';
			if ( is_ignored_file(pStart) > -1 ){
				write_log(LT_BOTH, "Error: file %s have "
					"existed in before line!\n",
				       pStart);
				continue;
			}

			strncpy(g_ignoreFile[nFileCount++], pStart,
				(int)(pPrevTab-pStart));

			nColCount = 0;
			pNextTab = pPrevTab;
			
			do{
				pPrevTab = pNextTab+1;
				pNextTab = strchr(pPrevTab, '\t');

				if ( pNextTab == NULL )
					pNextTab = pEnd+1;
				*pNextTab = '\0';
				nColNum = atoi( pPrevTab );
				g_ignoreCol[nFileCount-1][nColCount++] = nColNum;
				
				if ( nColCount == MAX_IGNORE_NUM ){
					write_log(LT_BOTH, 
						"Error: too much ignore column number for %s!\n",
						g_ignoreFile[nFileCount-1]);
					break;
				}
			}while (pNextTab != pEnd+1);
		}

		if ( nFileCount >= MAX_IGNORE_NUM ){
			write_log(LT_BOTH, "Error: exceed the max setting"
				"number when load %s", IGNORE_FILE_CFG);
			break;
		}
	}

	SAFE_CLOSE_FILE(pFile);

	return 1;
}

int
is_ignored_dir(const char* pcszDirectory){
	int	i    = 0;
	int	nDirLen;
	int	nIgnoreDirLen;

	for ( i = 0; i < MAX_IGNORE_NUM; i++ ){
		if ( g_ignoreDir[i][0] == '\0' )
			break;
		
		nDirLen = (int)strlen(pcszDirectory);
		nIgnoreDirLen = (int)strlen(g_ignoreDir[i]);
		if ( nDirLen < nIgnoreDirLen ||
		     (pcszDirectory[nDirLen-nIgnoreDirLen-1] != '\\' &&
		      pcszDirectory[nDirLen-nIgnoreDirLen-1] != '/') )
			continue;
		
		if ( str_nocase_cmp(&pcszDirectory[nDirLen-nIgnoreDirLen], 
			g_ignoreDir[i]) == 0 )
			return i;
	}

	return -1;
}

int
is_ignored_file(const char* pcszFileName){
	int	i    = 0;
	int	nFileNameLen;
	int	nIgnoreFileNameLen;
	
	for ( i = 0; i < MAX_IGNORE_NUM; i++ ){
		if ( g_ignoreFile[i][0] == '\0' )
			break;
		
		nFileNameLen = (int)strlen(pcszFileName);
		nIgnoreFileNameLen = (int)strlen(g_ignoreFile[i]);
		if ( nFileNameLen < nIgnoreFileNameLen ||
		     (pcszFileName[nFileNameLen-nIgnoreFileNameLen-1] != '\\' &&
		     pcszFileName[nFileNameLen-nIgnoreFileNameLen-1] != '/') )
			continue;

 		if ( str_nocase_cmp(&pcszFileName[nFileNameLen-nIgnoreFileNameLen], 
			g_ignoreFile[i]) == 0 )
			return i;
	}

	return -1;
}

void
get_path_or_filename(char* pPath){
	int	nStrLen;
	char	szPath[MAX_PATH];

	if ( pPath[0] == '\0' )
		return;
	
	if ( pPath[0] == '"' ){
		strcpy(szPath, pPath);
		nStrLen = (int)strlen(szPath);
		if ( szPath[nStrLen-1] == '"' )
			szPath[nStrLen-1] = '\0';
		strcpy(pPath, szPath+1);			
	}
	
	nStrLen = (int)strlen(pPath);
	if ( pPath[nStrLen-1] == '/' ||
	     pPath[nStrLen-1] == '\\')
		pPath[nStrLen-1] = '\0';
}	

int
is_there_gbk_code(const char* pBuffer){
	char* pTemp = (char*) pBuffer;
	
	while ( *pTemp != '\0' && *(pTemp+1) != '\0' ){
		if ( is_gbk_chinese(*pTemp, *(pTemp+1)) ){
			return 1;
		}
		pTemp++;
	}
	
	return 0;
}

int
is_special_sign(char cFirst, char cSecond){
	int    i = 0;

	for ( i = 0; i < MAX_SPECIAL_SIGN_NUM; i++ ){
		if ( g_szSpecialSignTable[i][0] == '\0' )
			break;

		if (cFirst == g_szSpecialSignTable[i][0] &&
			cSecond == g_szSpecialSignTable[i][1])
			return i;
	}

	return -1;
}

int
special_sign_convert(const char* pStrSrc, char* pStrDest, int nLanguageType){
	char*   pTemp = NULL;
	int     nIdx, nCount = 0;
	
	pTemp = (char*)pStrSrc;
	while ( *pTemp != '\0' && *(pTemp+1) != '\0' ){
		nIdx = is_special_sign(*pTemp, *(pTemp+1));
		if (nIdx != -1){
			if ( nLanguageType == TRANTIONAL ){
				pStrDest[nCount++] = g_szSpecialSignTable[nIdx][2];
				pStrDest[nCount++] = g_szSpecialSignTable[nIdx][3];
			}
			else{
				pStrDest[nCount++] = g_szSpecialSignTable[nIdx][4];
				if ( g_szSpecialSignTable[nIdx][5] != '\0' )
					pStrDest[nCount++] = g_szSpecialSignTable[nIdx][5];
			}
			pTemp += 2;
		}
		else{
			pStrDest[nCount++] = *pTemp;
			pTemp ++;
		}
	}

	if ( *pTemp != '\0' ) 
		pStrDest[nCount++] = *pTemp;
	pStrDest[nCount] = '\0';

	return 1;
}

int
load_special_sign_table(){
	FILE*	pFile = NULL;
	char	szBuffer[8];
	int     nCount = 0;

	pFile = fopen(SPECIAL_SIGN_CFG, "r");
	if ( pFile == NULL ){
		write_log(LT_BOTH, "Error: Can't open file %s!\n", SPECIAL_SIGN_CFG);
		return 0;
	}

	memset(g_szSpecialSignTable, 0, sizeof(g_szSpecialSignTable));
	while ( fgets(szBuffer, 32, pFile) != NULL ){
		if ( szBuffer[0] == ';' || szBuffer[0] == ' ' )
			continue;
		szBuffer[strlen(szBuffer)-1] = '\0';
		strcpy(g_szSpecialSignTable[nCount++], szBuffer);
		if ( nCount == MAX_SPECIAL_SIGN_NUM){
			write_log(LT_BOTH, "Error: the number of special sign is exceeded max number %d!\n",
				MAX_SPECIAL_SIGN_NUM);
			break;
		}
	}

	SAFE_CLOSE_FILE(pFile);
	return 1;
}

int
get_prescribed_sign_num(const char* pcszString, char cChar){
	char* pTemp = (char*)pcszString;
	int nNum = 0;
	while (*pTemp != '\0')
	{
		if (*pTemp == cChar)
			nNum ++;
		pTemp ++;
	}
	return nNum;
}

int
check_vlist(const char* pcszChinese, const char* pcszTrans){
	int   bvlist, bNotdate;
	char* pTemp1 = (char*)pcszChinese;
	char* pTemp2 = (char*)pcszTrans;

	pTemp1 = strchr((char*)pcszChinese, '%');
	while ( pTemp1 != NULL ){
		bvlist = ((*(pTemp1+1) == 'd') || (*(pTemp1+1) == 's'));
		if ( bvlist == 1 ){
			do{
				pTemp2 = strchr(pTemp2, '%');

				if ( pTemp2 == NULL )
					return 0;
				
				bvlist = ((*(pTemp2+1) == 'd') || (*(pTemp2+1) == 's'));
				if ( bvlist == 1 ){
					if ( *(pTemp1+1) != *(pTemp2+1) )
						return 0;
					else
						break;
				}
				else
					pTemp2++;
			}while (1);
		}
			
		pTemp1 ++;
		pTemp2 ++;
		pTemp1 = strchr(pTemp1, '%');
	}

	pTemp2 =(char*) pcszTrans;
	pTemp1 = strchr((char*)pcszChinese, '/');
	while ( pTemp1 != NULL ){
		bvlist = is_digit(*(pTemp1+1));
		if ( *(pTemp1+2) != '\0' )
			bNotdate =  !(is_digit(*(pTemp1+2)));
		else
			bNotdate = 1;
		
		if ( bvlist && bNotdate ){
			do{
				pTemp2 = strchr(pTemp2, '/');

				if ( pTemp2 == NULL )
					return 0;
				
				bvlist = is_digit(*(pTemp2+1));
				if ( bvlist == 1 ){
					if ( *(pTemp1+1) != *(pTemp2+1) )
						return 0;
					else
						break;
				}
				else
					pTemp2++;
			}while (1);
		}

		pTemp1 ++;
		pTemp2 ++;
		pTemp1 = strchr(pTemp1, '/');
	}

	return 1;
}

int
check_brackets(const char* pcszChinese, const char* pcszTrans){
	int nNum1,nNum2;
	
	nNum1 = get_prescribed_sign_num(pcszChinese, '{');
	nNum2 = get_prescribed_sign_num(pcszTrans, '{');
	if (nNum1 != nNum2 && nNum1 != 0)
		return 0;

	nNum1 = get_prescribed_sign_num(pcszChinese, '}');
	nNum2 = get_prescribed_sign_num(pcszTrans, '}');
	if (nNum1 != nNum2 && nNum1 != 0)
		return 0;

	nNum1 = get_prescribed_sign_num(pcszChinese, '[');
	nNum2 = get_prescribed_sign_num(pcszTrans, '[');
	if (nNum1 != nNum2 && nNum1 != 0)
		return 0;

	nNum1 = get_prescribed_sign_num(pcszChinese, ']');
	nNum2 = get_prescribed_sign_num(pcszTrans, ']');
	if (nNum1 != nNum2 && nNum1 != 0)
		return 0;

	return 1;
}
