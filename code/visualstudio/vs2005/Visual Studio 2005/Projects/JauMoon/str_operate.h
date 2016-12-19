#ifndef _GET_ALL_STRING_H_
#define _GET_ALL_STRING_H_

typedef int (*operate_callback)(const char* pcszFileName);
typedef int (*filtrate_buf)(char* pBuffer, int nStrLen);

#define		VIEW_A_NUM		50
#define		LINE_VIEW_NUM		28
#define		VIEW_LINE_NUM		(VIEW_A_NUM*LINE_VIEW_NUM)

#define is_tab(c) 			((c) == 0x09)
#define is_space(c)			((c) == 0x20)
#define is_gbk_chinese_space(a, b) 	((a) == 0xa1 && (b) == 0xa1)
#define is_point(c) 			((c) == 0x2e)
#define	is_colon(c)			((c) == 0x3a)
#define is_semicolon(c) 		((c) == 0x3b)
#define is_well(c) 			((c) == 0x23)
#define is_sigh(c) 			((c) == 0x21)
#define is_interrogation(c) 		((c) == 0x3f)
#define is_comma(c) 			((c) == 0x2c)
#define is_left_splash(c) 		((c) == 0x2f)
#define is_right_splash(c) 		((c) == 0x5c)
#define is_left_square_bracket(c) 	((c) == 0x5b)
#define is_right_square_bracket(c) 	((c) == 0x5d)
#define is_digit(c) 			(((c) >= 0x30) && ((c) <= 0x39))
#define is_letter(c) 			((((c) >= 0x41) && ((c) <= 0x5a) ) || (((c) >= 0x61) && ((c) <= 0x7a)))
#define is_minus(c) 			((c) == 0x2d)
#define is_star(c) 			((c) == 0x2a)
#define is_single_quotation(c) 		((c) == 0x27)
#define is_double_quotation(c) 		((c) == 0x22)
#define is_gbk_chinese_semicolon(a, b) 	(((a) == 0xa3) && ((b) == 0xbb))
#define is_ungderline(c) 		((c) == 0x5f)
#define is_bracket(c) 			(((c) == 0x28) || ((c) == 0x29))
#define is_ascii(c) 			(((c) < 128) && ((c) > 0))

#define is_ini_remark(a, b)	(is_gbk_chinese_semicolon(a, b) || is_semicolon(a) ||\
			     	is_well(a) || (is_left_splash(a) && is_left_splash(b)))
#define is_lua_remark(a, b)	(is_minus(a) && is_minus(b))
#define is_cpp_remark(a, b) 	(is_left_splash(a) && is_left_splash(b))
#define is_c_remark(a, b) 	(is_left_splash(a) && is_star(b))

#define is_ignore_sign(c) 	((is_tab(c)) || (is_semicolon(c)) || (is_point(c)) || \
			  	(is_space(c)) || (is_double_quotation(c)) || (is_star(c)) || \
			   	(is_comma(c)) || (is_interrogation(c)) || (is_sigh(c)) || \
			   	(is_colon(c))) 

#define is_ignore_sign2(c) 	((is_semicolon(c)) || (is_point(c)) || (is_colon(c)) || \
			   	(is_double_quotation(c)) || (is_star(c)) || (is_sigh(c)) ||\
			   	(is_comma(c)) || (is_interrogation(c))) 

#define  SAFE_CLOSE_FILE(pFile) if (pFile != NULL) {	\
			fclose(pFile); pFile = NULL;}

#ifndef MAX_PATH
#define MAX_PATH 	260
#endif//MAX_PATH

enum FILE_TYPE{
	FILE_UN_KNOWN = 0,
	FILE_TAB,
	FILE_INI,
	FILE_LUA,
	FILE_CPP,
};

enum STR_TYPE{
	STR_NORMAL = 1,
	STR_FILE_NAME,
	STR_PATH,
	STR_ERROR,
};

enum LOG_TYPE{
	LT_SCREEN = 0x01,
	LT_FILE = 0x02,
	LT_BOTH = 0x03,
};

int  load_ignore_dir();
int  load_ignore_file();
int  load_special_sign_table();
int  str_operate_init(const char* pcszSourceRoot, const char* pcszTransRoot);
void str_operate_uninit();

int  start_work();

void record_string(int nStrType, char* pBuffer, int nStrLen);

int  is_ini_section(char* pBuf);
int  is_gbk_chinese(unsigned char cFirst, unsigned char cSecond);

int  get_file_type(const char* pcszFileName);
int  filter_chinese_in_file(const char* pcszFileName);

int  filter_tab_buffer(char* pBuffer, int nStrLen);
int  filter_ini_buffer(char* pBuffer, int nStrLen);
int  filter_lua_buffer(char* pBuffer, int nStrLen);
int  filter_cpp_buffer(char* pBuffer, int nStrLen);
int  string_prcess(char* pBuffer, int nStrLen, int bError);
int  get_buffer_info(char* pBufferInfo);
int  get_string_type(char* pBuffer, int nStrLen);

int  create_dir_for_translate(const char* pcszPath);
int  filtrate_ignore_dir(const char* pcszPath);
int  clear_ignore_sign(const char* pcszPath);
int  is_a_valid_directory(char* pcszDirectory);
int  is_ignored_dir(const char* pcszDirectory);
int  is_ignored_file(const char* pcszFileName);
int  visit_path_recursively(const char* pcszPath, operate_callback file_func,
			    operate_callback dir_func, operate_callback dir_func2);

char*  strip_ignore_sign(char* pBuffer, int* nStrLen);
int  process_old_chinese_and_trans(char* pChinese, char* pTrans,
				    int* nChienseHeadLen, int* nTransHeadLen);

void get_path_or_filename(char* pPath);
int  is_there_gbk_code(const char* pBuffer);

int  special_sign_convert(const char* pStrSrc, char* pStrDest, int nLanguageType);
int  check_vlist(const char* pcszChinese, const char* pcszTrans);
int  check_brackets(const char* pcszChinese, const char* pcszTrans);

#endif // _GET_ALL_STRING_H_
