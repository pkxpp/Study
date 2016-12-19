/**********************************************************************
* 源程序名称: futu_sdk_interface.h
* 软件著作权: 恒生电子股份有限公司
* 系统名称  : 06版本期货系统
* 模块名称  : 恒生期货周边接口
* 功能说明  : 周边通信接口定义
* 作    者  : xdx
* 开发日期  : 20110315
* 备    注  : 周边通信接口定义  
* 修改人员  ：
* 修改日期  ：
* 修改说明  ：20110315 创建
**********************************************************************/
#ifndef _FUTU_SDK_INTERFACE_H_
#define _FUTU_SDK_INTERFACE_H_

#include "futu_data_types.h"
#include "futu_message_interface.h"

#ifdef _WIN32
#  ifdef HSAPI
#  undef HSAPI  
#  endif
#  define HSAPI __stdcall
#else
#    define HSAPI 
#endif

struct IFuCallBack;

////////////////////////////////期货通信对象接口//////////////////////////////////////////
struct IHsFutuComm:public IHSKnown
{
	/** 
	 * 设置配置参数
	 *@param szSection 配置小节
	 *@param szName    配置项名称
	 *@param szVal     配置项的值
	 *@return          ERR_OK表示成功,其他表示失败
	 [非线程安全]
	*/
	virtual int HSAPI SetConfig(const char* szSection,const char* szName,const char* szVal) = 0;

	/** 
	 * 初始化接口,始化各种数据,并从登陆服务器上取得相应的信息
	 *@param lpCallback 异步回调对象,由用户继承对应的接口实现之,为NULL则表示不关心回调信息
	 *@param iTimeOut   超时时间,表示初始化超时时间,单位毫秒,-1表示无限期等待.
	 *@return           ERR_OK表示成功,其他表示失败
	 [线程安全]
	*/
	virtual int HSAPI Init(IFuCallBack* lpCallback = NULL,int iTimeOut = 5000) = 0;
	
	/**
	 * 启动服务,建立各种应用的通信连接.当通信连接断开时,也可以调用此函数进行重新连接
	 *@param iType    要启动的服务类型.允许取值1=SERVICE_TYPE_TRADE,
	                                           3=SERVICE_TYPE_TRADE+SERVICE_TYPE_QUOTE
	                  建议起始连接类型为3.
	 *@param iTimeout 启动服务超时
	 *@return         ERR_OK表示成功,其他表示失败 
	 [线程安全]
	*/
	virtual int HSAPI Start(int iType,int iTimeOut = 5000)  = 0;

	/** 20110729 luyj 增加reportFlag参数  修改单号20110729007
	 * 用户登录,在做具体的业务之前,需要先登陆以取得相关身份认证信息
	 *@param szUserID   用户名,一般指资金账号
	 *@param szUserPass 用户密码
	 *@param reportFlag 用户是否需要主推回报，1需要，0不需要
	 *@param lpReserved 保留参数,本版本接口必须设置为NULL
	 *@return           ERR_OK表示请求成功(但此时并不表示已经登陆成功,需从异步回调中取应答消息)
	 [线程安全]
	*/
	virtual int HSAPI DoLogin(const char *szUserID,const char* szUserPass,const int reportFlag,const void* lpReserved = NULL) = 0;

	/** 20110729 luyj 增加report_flag参数  修改单号20110729007 
	    20110701 luyj 增加这个函数 修改单号20110623004
	* 用户登录,在做具体的业务之前,需要先登陆以取得相关身份认证信息
	*@param szUserID   用户名,一般指资金账号
	*@param szUserPass 用户密码
	*@param lpReceivedMsg 用户的返回信息
	*@param report_flag 用户是否需要主推回报，1需要，0不需要
	*@param lpReserved 保留参数,本版本接口必须设置为NULL
	*@return           ERR_OK表示请求成功(但此时并不表示已经登陆成功,要从lpReceivedMsg取得应答消息)
	[线程安全]
	*/
	virtual int HSAPI DoLoginEx(const char *szUserID,const char* szUserPass,IFuMessage* lpReceivedMsg,const int reportFlag,const void* lpReserved = NULL) = 0;

	/** 
	 * 发送一个业务请求消息(应答结果在异步回调中接收)
	 *@param lpMessage 一条业务请求消息,由用户负责管理其生命期
	 *@param iKeyID    一个自定义标识,异步应答包中会带此标识.-1表示不关心此标志(则默认异步应答以资金账号作为keyid返回)
     *@return          ERR_OK表示发送请求成功,需要在异步回调中取相应的应答消息结果
	 [非线程安全]
	*/
	virtual int HSAPI AsyncSend(const IFuMessage* lpReqMsg,int iKeyID = -1) = 0;

	/** 同步发送一个业务请求消息
	 *@param lpReqMsg 请求的业务消息  [in]
	 *@param lpAnsMsg 同步应答业务消息[in,out]
	 *@param iTimeout 超时时间,单位毫秒
	 *@return         ERR_OK表示成功,其他表示失败
	 [非线程安全]
	*/
	virtual int HSAPI SyncSendRecv(const IFuMessage* lpReqMsg,IFuMessage* lpAnsMsg,int iTimeout = 3000) = 0;

	/**
	 * 发送订阅请求,业务接收结果
	 *@param rType   订阅类型(参见REGType的定义)
	 *@param rAction 订阅/退订等动作(参见REGAction的定义)
	 *@param szParam 根据不同的rType区分:
	                 若是行情:szParam表示订阅的合约列表,以逗号分割字符串,例如WS905,a0905,cu0905,IF0905  
	                          若订阅全市场的行情,订阅代码为ALLWWW
	                          合约串只能指定一种类型(单腿或者组合)
				     若是回报/个人消息:szParam则表示待订阅的用户账号,注意这里的账号都是登陆过的账号
	 *@return        ERR_OK表示请求成功(从异步应答中取订阅结果),其他表示请求失败
	 [非线程安全]
	*/
	virtual int HSAPI SubscribeRequest(REGType rType,REGAction rAction,const char* szParam) = 0;

	/** 
	 * 用户登出
	 *@param szUserID    待登出的用户账号
	 *@param lpReserved  保留参数,本版本必须设置为NULL
     *@return            ERR_OK表示请求成功,具体可以从登陆应答中取得信息
	 [线程安全]
	*/
	virtual int HSAPI DoLogout(const char* szUserID,const void* lpReserved = NULL) = 0;

	/**
	 * 关闭清理通信对象,停止相关通信连接
	 *@return ERR_OK成功,其他表示失败
	 [线程安全]
	*/
	virtual int HSAPI Stop() = 0;

	/**
	 * 获取连接状态
	 *@param iIndex连接编号.SERVICE_TYPE_TRADE - 表示交易连接 SERVICE_TYPE_QUOTE -行情(可能包含回报)连接
	 *@return 获取连接状态(参考CONState的定义)
	 [线程安全]
	*/
	virtual int HSAPI GetStatus(int iIndex) = 0;

	/**
	 * 获取错误信息
	 *@param iErrNo 错误号
	 *@return       获取错误信息的说明(本函数不会失败)
	 [线程安全]
	*/
	virtual const char* HSAPI GetErrorMsg(int iErrNo) = 0;

	/**
	 * 绑定与接口对象实例相关的数据(主要在多实例应用中)
	 *@param lpKey   要绑定的数据缓冲区
     *@param iKeyLen 要绑定的数据长度
	 *@return        ERR_OK绑定成功,其他表示绑定失败(可能是内存分配错误)
	 [线程安全]
	*/
	virtual int  HSAPI SetKeyData(const void* lpKeyData,int iLen) = 0;

	/**
	 * 获取与接口对象绑定的数据
	 *@param iLen 绑定的数据长度[in,out]
	 *@return 返回要绑定的数据指针
	 [线程安全]
	*/
	virtual const void* HSAPI GetKeyData(int* iLen) = 0;
};

////////////////////////通信异步回调接口(注意回调中的IFuMessage消息由SDK管理其生命周期)//////////////////////////////////////////////////
struct IFuCallBack:public IHSKnown
{
	/** 
	 * 连接状态的改变
	 *@param lpComm       通信接口对象,以区分多连接的应用
	 *@param iType        连接类型.0 - 交易 1 - 行情(可能包含回报)
	 *@param iRet         返回状态标识
	 *@param szNotifyTime 发生的时间
	 *@param szMessage    说明信息
	*/
	virtual void HSAPI OnNotifyConnState(IHsFutuComm* lpComm,int iType,int iStatus,const char* szNotifyTime,const char* szMessage) = 0;

	/**
	 * 登陆应答响应
	 *@param lpComm 通信接口对象,以区分多连接的应用
	 *@param lpMsg  登陆应答响应消息
	*/
	virtual void HSAPI OnRspLogin(IHsFutuComm* lpComm,IFuMessage* lpMsg) = 0;

	/** 
	 * 登出反馈
	 *@param lpComm 通信接口对象,以区分多连接的应用
	 *@param lpMsg  登出应答响应消息
	*/
	virtual void HSAPI OnRspLogout(IHsFutuComm* lpComm,IFuMessage* lpMsg) = 0;

	/**
	 *订阅\退订行情或者回报的结果
	 *@param lpComm    通信接口对象,以区分多连接的应用
	 *@param sType     订阅类型(单腿行情,组合行情,回报,在线消息)
	 *@param aAction   请求方式(订阅,取消,取消全部,快照)
	 *@param iResult   订阅结果,-1未知失败,0订阅成功,1重复订阅,2取消成功,3没有订阅可以取消
	 *@param lpParam   附加的参数,若是回报应答,则是账号;若是行情,则是合约代码;ALLWWW表示所有;NULL未知
	 *@param szMessage 文字说明  
	*/
	virtual void  HSAPI OnRspSubResult(IHsFutuComm* lpComm,REGType sType,REGAction aAction,int iResult,const char* lpParam,const char* szMessage) = 0;

	/** 
	 * 业务接收到业务应答消息
	 *@param lpComm     通信接口对象,以区分多连接的应用
	 *@param lpAnsData  业务应答消息
	 *@param iRet       处理结果ERR_OK表示成功,其他则根据其值取具体的错误信息
	 *@param iKeyID     异步发送请求中所带keyid,若请求不指定,则默认以资金账号返回
    */
	virtual void HSAPI OnReceivedBiz(IHsFutuComm* lpComm,IFuMessage* lpAnsData,int iRet,int iKeyID) = 0;

	/** 
	 * 接收到单腿市场行情
	 *@param lpComm     通信接口对象,以区分多连接的应用
	 *@param lpInfo     单腿行情数据
	 *@param rAction    表示是快照还是订阅主推
	*/
	virtual void HSAPI OnRecvMarketData(IHsFutuComm* lpComm,CMarketInfo* lpInfo,REGAction rAction) = 0;

	/** 
	 * 接收到组合市场行情
	 *@param lpComm     通信接口对象,以区分多连接的应用
	 *@param lpInfo     单腿行情数据
	 *@param rAction    表示是快照还是订阅主推
	*/
	virtual void HSAPI OnRecvArgMarketData(IHsFutuComm* lpComm,CArgMarketInfo* lpInfo,REGAction rAction) = 0;

	/** 
	 * 接收到委托反馈消息
	 *@param lpComm     通信接口对象,以区分多连接的应用
	 *@param lpInfo     委托反馈消息(参见COrderRspInfo结构的定义)
	*/
	virtual void HSAPI OnRecvOrderInfo(IHsFutuComm* lpComm,const COrderRspInfo* lpInfo) = 0;

	/** 
	 * 接收到订单成交反馈
	 *@param lpComm     通信接口对象,以区分多连接的应用
	 *@param lpInfo     订单成交反馈(参见CRealRspInfo结构的定义)
	*/
	virtual void HSAPI OnRecvOrderRealInfo(IHsFutuComm* lpComm,const CRealRspInfo* lpInfo) = 0;

	/**
	 * 在线个人消息
	 *@param  lpComm    通信接口对象,以区分多连接的应用
	 *@param  szUsrID   消息相关的账号
	 *@param  szMessage 相关的个人消息
	*/
	virtual void HSAPI OnRspOnlineMsg(IHsFutuComm* lpComm,const char* szUsrID,const char* szMessage) = 0;
};


//////////////////////////////////导出接口////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
    
	/** 
	 * 创建新的通信接口对象(可以创建不同的接口对象以连接多个服务商)
	 *@param lpReserved 保留参数,目前必须置为NULL
	 *@return           非NULL表示有效的接口对象(可以调用相应的接口函数)，否则表示分配内存失败
	 [线程安全]
	*/

    IHsFutuComm* HSAPI NewFuCommObj(void* lpReserved = NULL);

	/** 
	 * 获取SDK版本号
	 *@return 版本号,十六进制数据,例如0x10000000,表示版本为1.0.0.0
	 [线程安全]
    */
	int HSAPI GetSDKVersion();

#ifdef __cplusplus
}
#endif

#endif /*futu_sdk_interface.h*/

