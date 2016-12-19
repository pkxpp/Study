/**********************************************************************
* 源程序名称: futu_message_interface.h
* 软件著作权: 恒生电子股份有限公司
* 系统名称  : 06版本期货系统
* 模块名称  : 恒生期货周边接口
* 功能说明  : 周边接口数据操作定义
* 作    者  : xdx
* 开发日期  : 20110315
* 备    注  : 数据类型定义   
* 修改人员  ：
* 修改日期  ：
* 修改说明  ：20110315 创建
**********************************************************************/
#ifndef _FUTU_MESSAGE_INTERFACE_H_
#define _FUTU_MESSAGE_INTERFACE_H_

#include "futu_data_types.h"

#ifdef _WIN32
#  ifdef HSAPI
#  undef HSAPI  
#  endif
#  define HSAPI __stdcall
#else
#    define HSAPI 
#endif

//各类接口统一的查询与引用接口 (参照COM标准)
struct IHSKnown
{
	/**
	 * 查询与当前接口相关的其他接口(本版本的接口暂不使用)
	 *@param HS_SID  iid  接口全局唯一标识
	 *@param IKnown **ppv 返回iid对应的接口指针
	 *@return ERR_OK 成功,ERR_FAIL 未查到iid 相应接口
	*/
	virtual unsigned long  HSAPI QueryInterface( const char * iid, IHSKnown **ppv ) = 0;

	/**
	 * 引用接口，引用计数加一
	 *@return 当前引用计数
	*/
	virtual unsigned long  HSAPI AddRef() = 0;

	/** 
	 * 释放接口，引用计数减一，计数为0时释放接口的实现对象
	 *@param ERR_OK表示成功,其他表示失败
	*/
	virtual unsigned long  HSAPI Release() =  0;
};

/*
  一条消息数据记录,包含了很多的Field=Value,其中Field不重复
  若重复插入相同的Field,则覆盖上次的数据
  为了使接口跨语言,仅支持以下数据类型:
    单字符型(char),如'A','1',
	整型(int),如123,-2343
	浮点型(float),如23.50,-34.34
	字符串型(C语言风格'\0'结尾的串)(char*),如"Hello World"
	注意其接口个别方法非线程安全
*/
struct IFuRecord:public IHSKnown
{
	///添加数据的方法,若存在field则覆盖其值
	/**
	 * 添加一个字符型值
	 *@param sField   字段名称
	 *@param cValue   一个字符值
	 *@return ERR_OK添加成功,其他表示失败
	 [非线程安全]
    */
	virtual int HSAPI SetChar(const char* sField,char cValue) = 0;

	/**
	 * 添加一个整型值
	 *@param sField  字段名称
	 *@param iValue  一个整型值
	 *@return ERR_OK添加成功,其他表示失败
	 [非线程安全]
	*/
	virtual int HSAPI SetInt(const char* sField,int iValue) = 0;

	/**
	 * 添加一个浮点值
	 *@param sField 字段名称
	 *@param cValue 一个浮点值
	 *@return ERR_OK添加成功,其他表示失败
	 [非线程安全]
	*/
	virtual int HSAPI SetDouble(const char* sField,double dValue) = 0;

	/**
	 * 添加一个字符串(C语言格式'\0'结尾的字符串)
	 *@param sField 字段名称
	 *@param cValue 一个字符串值
	 *@return ERR_OK添加成功,其他表示失败
	 [非线程安全]
	*/
	virtual int HSAPI SetString(const char* sField,const char* strValue) = 0;


    ///根据字段访问数据的方法
	/**
	 * 获取一个字符型值
	 *@param sField 字段名称
	 *@return 有效字符值;字段不存在则默认'\0'
	 [线程安全]
	*/
	virtual char HSAPI GetChar(const char* sField) = 0;

	/**
	 * 获取一个整型值
	 *@param sField 字段名称
	 *@return 返回一个整型值;字段不存在则默认返回0
	 [线程安全]
	*/
	virtual int  HSAPI GetInt(const char* sField) = 0;

	/**
	 * 获得一个浮点值
	 *@param sField 字段名称
	 *@return 返回一个浮点值;字段不存在则默认返回0.0(注意浮点型的精度问题)
	 [线程安全]
	*/
	virtual double HSAPI GetDouble(const char* sField)  = 0;

	/**
	 * 获得一个字符串
	 *@param sField 字段名称
	 *@return 返回一个字符串,如不存在该字段则返回空字符串""(注意不是NULL)
	 [线程安全]
	*/
	virtual const char* HSAPI GetString(const char* sField) = 0;

	///遍历访问方法,通过此访问可遍历整条记录
	/**
	 * 移动到记录头(第一个Field=Value)
	 *@return ERR_OK表示成功,其他表示失败
	 [非线程安全]
	*/
	virtual int HSAPI MoveFirst() = 0;

	/**
	 * 下一条记录
	 *@return ERR_OK表示成功,其他表示失败或已经到记录尾
	 [非线程安全]
	*/
	virtual int HSAPI MoveNext() = 0;

	/**
	 * 判断是否移到了记录尾
	 *@return 0表示未到记录尾,1表示已到记录尾
	 [非线程安全]
	*/
	virtual int HSAPI IsEOF() = 0;

	/**
	 * 获取当前字段名(通过该名可以访问到值)
	 *@return NULL表示已到结尾或无数据,非NULL表示字段名
	 [线程安全]
	*/
	virtual const char* HSAPI GetFieldName() = 0;

	/** 
	 * 删除一个字段
	 *@param sField 要删除的字段名
	 *@return ERR_OK表示删除成功,其他表示失败
	 [非线程安全]
    */
	virtual int HSAPI RemoveField(const char* sField) = 0;

	/**
	 * 是否存在某个字段
	 *@param sField 字段名
	 *@return 0表示不存在,1表示存在
	 [非线程安全]
	*/
	virtual int HSAPI IsExist(const char* sField) = 0;

	/**
	 * 删除所有的字段
	 *@return ERR_OK表示成功,其他表示失败
	 [非线程安全]
	*/
	virtual int HSAPI Clear() = 0;

	/** 
	 * 获取记录个数(Field=Value)
	 *@return 表示记录(Field=Value)的条数
	 [线程安全]
	*/
	virtual int HSAPI GetCount() = 0;
};

//期货消息接口,一条消息表明了其消息的类型,并由一个或多个IFuRecord组成
//注意大多数接口非线程安全，因此尽量不要跨线程同时操作消息
struct IFuMessage:public IHSKnown
{
	/**
	 * 设置消息类型
     *@param iType 参见FUTU_MSG_TYPE的定义
	 *@param iMode 表示此消息是请求还是应答,0-请求,1-应答
	 *@return ERR_OK表示成功,其他表示无效或不支持的消息类型
	 [非线程安全]
	*/
	virtual int HSAPI SetMsgType(int iType,int iMode) = 0;

	/** 
	 * 获取消息类型
	 *@param [out]lpMsgMode 消息模式.参考FUTU_MSG_MODE的定义
	 *@return  消息类型.参考FUTU_MSG_TYPE的定义
	 [线程安全]
    */
	virtual int HSAPI GetMsgType(int* lpMsgMode = NULL) = 0;

	/**
	 * 获取记录条数
	 *@param >=0表示记录条数,其他表示错误或失败
	 [线程安全]
	*/
	virtual int HSAPI GetCount() = 0;

	/** 
	 * 新增一条记录,操作返回的指针以操作其值
	 *@return 非NULL表示一条有效的记录,NULL表示分配内存失败
	 [非线程安全]
	*/
	virtual IFuRecord* HSAPI AddRecord() = 0;

	/**
	 * 获取一条记录
	 *@param iIndex 记录索引位置,从0开始计数
	 *@return 非NULL表示一条有效的记录,NULL表示索引越界
	 [非线程安全]
	*/
	virtual IFuRecord* HSAPI GetRecord(int iIndex = 0) = 0;

	/**
	 * 删除一条记录
	 *@param iIndex 记录索引位置,从0开始计数
	 *@return ERR_OK表示删除功能,否则失败(可能是索引越界)
	 [非线程安全]
	*/
	virtual int HSAPI DelRecord(int iIndex = 0) = 0;

	/**
	 * 删除所有的记录
	 *@return ERR_OK表示成功,其他表示失败
	 [非线程安全]
	*/
	virtual int HSAPI Clear() = 0;
};

//导出接口
#ifdef __cplusplus
extern "C"
{
#endif

   /** 
    * 产生一条新的消息
	*@param iMsgType 消息类型,请参考消息FUTU_MSG_TYPE类型定义
	*@param iMsgMode 消息模式,请参考消息FUTU_MSG_MODE类型定义
	*return          非NULL表示一条有效的消息,NULL表示分配内存失败
	[线程安全]
   */
   IFuMessage* HSAPI NewFuMessage(int iMsgType = MSG_TYPE_UNKNOWN,int iMsgMode = MSG_MODE_UNKNOWN);

#ifdef __cplusplus
}
#endif


#endif/*futu_msg_interface.h*/

