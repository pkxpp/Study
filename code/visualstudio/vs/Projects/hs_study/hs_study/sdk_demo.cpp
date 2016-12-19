/**********************************************************************
源程序名称: sdk_demo.cpp
软件著作权: 恒生电子股份有限公司
系统名称  : 06版期货系统
模块名称  : 恒生期货消息订阅接口
功能说明  : HsFutuSDK示例程序
作    者  : xdx
开发日期  : 20110315
备    注  : HsFutuSDK示例程序
修改人员  ：
修改日期  ：
修改说明  : 20110315 创建
**********************************************************************/
//#include <vld.h>
#include <iostream>
#include <string>
#include <iomanip>
#include "futu_data_types.h"
#include "futu_message_interface.h"
#include "futu_sdk_interface.h"
#include "stdafx.h"

using namespace  std;

#ifdef _WIN32
#   include <conio.h>
#   include <windows.h>
#   ifdef _DEBUG
#      pragma comment(lib,"HsFutuSDK.lib")
#   else
#      pragma comment(lib,"HsFutuSDK.lib")
#   endif

///高精度计时器,单位毫秒
class CTimeCounter
{
public:
	//构造函数
	CTimeCounter(){ QueryPerformanceFrequency(&hz); }
	//开始计时
	inline void Start(){ QueryPerformanceCounter(&rs); }
	//从开始计时逝去的时间
	inline long double Elapse()
	{
		return ((now.QuadPart - rs.QuadPart) / (long double)hz.QuadPart) * 1000;
	}
	inline void Stop(){ QueryPerformanceCounter(&now); }
private:
	LARGE_INTEGER hz;
	LARGE_INTEGER rs;
	LARGE_INTEGER now;
};

//自动重连线程相关
HANDLE        g_hEvent = NULL;
volatile int  g_iType = 0;
volatile bool g_bStop = false;
//重连线程函数

DWORD WINAPI auto_connect(void* lpParam)
{
	IHsFutuComm* lpComm = (IHsFutuComm*)lpParam;
	while (!g_bStop)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_hEvent, INFINITE))
		{
			//退出
			if (g_bStop)
			{
				cout << "自动重连线程退出!" << endl;
				return 0;
			}

			cout << "检测连接端口,开始自动重连......" << endl;
			//重连
			for (int iRet = 0;;)
			{
				if (0 == (iRet = lpComm->Start(g_iType)))
				{
					ResetEvent(g_hEvent);
					break;
				}
				cout << "线程自动重连失败:" << lpComm->GetErrorMsg(iRet) << endl;
				//重连失败,暂停一会儿再试
				Sleep(1000);
			}
		}
	}
	cout << "自动重连线程退出!" << endl;
	return 0;
}
#define pause _getch
#else //end _WIN32
#include <sys/time.h>
class CTimeCounter
{
public:
	inline void Start(){ gettimeofday(&tv1, NULL); }
	inline void Stop(){ gettimeofday(&tv2, NULL); }
	inline long double Elapse()
	{
		return (tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec - tv1.tv_usec) / 1000.0f;
	}
private:
	timeval tv1, tv2;
};
#define pause getchar
#endif//end #else


//全局变量
CTimeCounter g_Counter;

#define USER_NAME "100001"
#define USER_PWD  "111111"

//测试入口函数
void test_case(IFuMessage* lpReqMsg);

//显示整条消息
void ShowFuMessage(IFuMessage* lpMessage)
{
	cout << "======================<<IFuMessage BEGIN>>=====================" << endl;
	cout << "nRecords:" << lpMessage->GetCount()
		<< " MsgType:" << lpMessage->GetMsgType()
		<< ((lpMessage->GetCount() <= 0) ? " (应答结果集为空)" : "") << endl;
	for (int i = 0; i<lpMessage->GetCount(); ++i)
	{
		IFuRecord* lpRecord = lpMessage->GetRecord(i);
		cout << "Record [" << i + 1 << "]:" << endl;
		for (lpRecord->MoveFirst(); !lpRecord->IsEOF(); lpRecord->MoveNext())
		{
			const char* strField = lpRecord->GetFieldName();
			cout.width(20);
			cout << strField << " : " << lpRecord->GetString(strField) << endl;
		}
		cout << endl;
	}
	cout << "=======================<<IFuMessage END>>======================" << endl;
}


//异步回调接口的实现
class CMyCallBack :public IFuCallBack
{
public:
	//接口查询
	unsigned long  HSAPI QueryInterface(const char *, IHSKnown **){ return 0; }
	//引用接口，引用计数加一
	unsigned long  HSAPI AddRef(){ return 0; }
	//释放接口，引用计数减一，计数为0时释放接口的实现对象
	unsigned long  HSAPI Release(){ return 0; }
	//连接状态的改变
	void HSAPI OnNotifyConnState(IHsFutuComm* lpComm, int iType, int iStatus, const char* szNotifyTime, const char* szMessage)
	{
		cout << "通信状态改变:" << iType << " : " << setw(2) << iStatus << " : " << szNotifyTime << " : " << szMessage << endl;
#ifdef _WIN32
		if ((iStatus == 0) && !g_bStop)//连接断开了
		{
			g_iType = iType;
			SetEvent(g_hEvent);
		}
#else

#endif
	}

	//登陆反馈
	void HSAPI OnRspLogin(IHsFutuComm* lpComm, IFuMessage* lpMsg)
	{
		cout << "登陆反馈:" << lpMsg->GetRecord()->GetCount() << endl;
		lpMsg->AddRef();
		ShowFuMessage(lpMsg);
	}

	//登出反馈
	void HSAPI OnRspLogout(IHsFutuComm* lpComm, IFuMessage* lpMsg)
	{
		cout << "登出反馈:" << endl;
		ShowFuMessage(lpMsg);
	}

	//定义反馈
	void  HSAPI OnRspSubResult(IHsFutuComm* lpComm, REGType sType, REGAction aType, int iResult, const char* lpParam, const char* szMessage)
	{
		cout << "订阅结果反馈:type=" << sType << " action=" << aType << " result=" << iResult << " param=" << (lpParam ? lpParam : "(null)") << " message=" << szMessage << endl;
	}

	//接收到业务应答消息
	void HSAPI OnReceivedBiz(IHsFutuComm* lpComm, IFuMessage* lpAnsData, int iRet, int iKeyID)
	{
		g_Counter.Stop();
		cout << "收到业务消息，异步处理耗时:" << g_Counter.Elapse() << " iRet=" << iRet << "KeyID=" << iKeyID << endl;
		ShowFuMessage(lpAnsData);
	}

	//接收到单腿市场行情
	void HSAPI OnRecvMarketData(IHsFutuComm* lpComm, CMarketInfo* lpInfo, REGAction rAction)
	{
		cout << "单腿行情:(" << lpInfo->futu_exch_type << ":" << lpInfo->contract_code << ")" << endl;
	}

	//接收到组合市场行情
	void HSAPI OnRecvArgMarketData(IHsFutuComm* lpComm, CArgMarketInfo* lpInfo, REGAction rAction)
	{
		cout << "组合行情:(" << lpInfo->futu_exch_type << ":" << lpInfo->arbicontract_id << ")" << endl;
	}

	//接收到委托反馈消息
	void HSAPI OnRecvOrderInfo(IHsFutuComm* lpComm, const COrderRspInfo* lpInfo)
	{
		cout << "委托反馈:(fund_account=" << lpInfo->fund_account << " : entrust_no=" << lpInfo->entrust_no << ")" << endl;
	}

	//接收到成交反馈
	void HSAPI OnRecvOrderRealInfo(IHsFutuComm* lpComm, const CRealRspInfo* lpInfo)
	{
		cout << "成交反馈:(";
		cout << "fund_account=" << lpInfo->fund_account << ";";
		cout << " entrust_no=" << lpInfo->entrust_no << ";";
		cout << "entrust_bs=" << lpInfo->entrust_bs << ";";
		cout << "entrust_direction=" << lpInfo->entrust_direction << ";";
		cout << "hedge_type=" << lpInfo->hedge_type << ";";
		cout << "futu_exch_type=" << lpInfo->futu_exch_type;
		cout << ")" << endl;

	}

	//个人在线消息
	void HSAPI OnRspOnlineMsg(IHsFutuComm* lpComm, const char* szUsrID, const char* szMessage)
	{
		cout << "在线消息:(user=" << (szUsrID ? szUsrID : "(null)") << " message=" << szMessage << ")" << endl;
	}
};


//主函数入口
int main(void)
{
	cout << "************************************************************" << endl;
	cout << "***                                                      ***" << endl;
	cout << "***    HsFutuSDK DEMO by HUNDSUN (" << __DATE__ << "-" << __TIME__ << ")  ***" << endl;
	cout << "***              SDK Version:0x" << hex << GetSDKVersion() << dec << "                  ***" << endl;
	cout << "***                                                      ***" << endl;
	cout << "************************************************************" << endl;
	//cout<<"ProcessID:"<<GetCurrentProcessId()<<" ThreadID:"<<GetCurrentThreadId()<<endl;

	//检查dll的版本是接口声明的版本相符合否
	if (HSFUSDK_VERSION != GetSDKVersion())
	{
		cout << "注意：接口的头文件版本[0x" << hex << HSFUSDK_VERSION << "]与库版本[0x" << GetSDKVersion() << "]不符合,可能导致兼容性错误!" << endl;
		return -1;
	}
	//新建通信对象
	IHsFutuComm* lpComm = NewFuCommObj(NULL);//FUTU_SYSTEM_V06,FUTU_SYSTEM_UFT

#ifdef _WIN32
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//创建自动重连线程
	HANDLE hThread = CreateThread(NULL, 0, auto_connect, lpComm, 0, NULL);
#else

#endif
	//设置参数
	lpComm->SetConfig("futu", "server", "127.0.0.1:2800");
	lpComm->SetConfig("futu", "biz_license_str", "31332263F984F25D4A1ADEFEC3");
	lpComm->SetConfig("futu", "comm_license_file", "C:\\yfq_license.dat");
	lpComm->SetConfig("futu", "entrust_type", "L");//委托方式

	//初始化接口对象
	CMyCallBack callback;
	int iRet = 0;
	if (0 != (iRet = lpComm->Init(&callback)))
	{
		cout << "初始化失败.iRet=" << iRet << " msg:" << lpComm->GetErrorMsg(iRet) << endl;
		lpComm->Release();
		return -1;
	}
	//启动(消息中心)
	if (0 != (iRet = lpComm->Start(SERVICE_TYPE_QUOTE)))
	{
		cout << "启动失败.iRet=" << iRet << " msg:" << lpComm->GetErrorMsg(iRet) << endl;
		lpComm->Release();
		return -1;
	}

	pause();


	//启动(交易) 
	if (0 != (iRet = lpComm->Start(SERVICE_TYPE_TRADE)))
	{
		cout << "启动失败.iRet=" << iRet << " msg:" << lpComm->GetErrorMsg(iRet) << endl;
		lpComm->Release();
		return -1;
	}


	pause();

	//登陆
	if (0 != (iRet = lpComm->DoLogin(USER_NAME, USER_PWD, 1)))
	{
		cout << "登陆失败.iRet:" << iRet << " msg:" << lpComm->GetErrorMsg(iRet) << endl;
		lpComm->Release();
		return -1;
	}


	//用DoLoginEx登录
	/*
	IFuMessage* lpLoginMsg=NewFuMessage(MSG_TYPE_USER_LOGIN,MSG_MODE_ANSWER);
	if(0 != (iRet = lpComm->DoLoginEx(USER_NAME,USER_PWD,lpLoginMsg,0)))
	{
	cout<<"登陆失败.iRet:"<<iRet<<" msg:"<<lpComm->GetErrorMsg(iRet)<<endl;
	lpComm->Release();
	return -1;
	}
	cout<<"登陆反馈:"<<lpLoginMsg->GetRecord()->GetCount()<<endl;
	lpLoginMsg->AddRef();
	ShowFuMessage(lpLoginMsg);
	lpLoginMsg->Release();
	*/
	pause();
	//lpComm->DoLogin("20033",USER_PWD);
	pause();
	//lpComm->DoLogin("20035",USER_PWD);
	pause();

	//订阅一下行情或者回报
	lpComm->SubscribeRequest(SingleCode, Subscription, "ALLWWW");
	//lpComm->SubscribeRequest(SingleCode,Subscription,"CF205");
	lpComm->SubscribeRequest(RspReport, Subscription, USER_NAME);
	//lpComm->SubscribeRequest(CombinCode,Subscription,"ALLWWW");
	//lpComm->SubscribeRequest(OnlineMsg,Subscription,USER_NAME);

	pause();
	/*
	//业务操作委托下单
	IFuMessage* lpReqMsg = NewFuMessage(MSG_TYPE_NEW_SINGLE_ORDER,MSG_MODE_REQUEST); //委托下单
	IFuMessage* lpAnsMsg = NewFuMessage();     //接收消息(无关消息类型和模式,SDK会置相关类型)

	//打包请求参数(字段顺序无关,重复设置字段则覆盖其值)
	IFuRecord* lpRecord = lpReqMsg->AddRecord();
	lpRecord->SetString("fund_account",USER_NAME);
	lpRecord->SetString("password",USER_PWD);
	lpRecord->SetString("futu_exch_type","F1");
	lpRecord->SetString("futures_account","");
	lpRecord->SetString("contract_code","TA110");
	lpRecord->SetString("entrust_bs","2");
	lpRecord->SetString("futures_direction","1");
	lpRecord->SetString("hedge_type","0");
	lpRecord->SetString("futu_entrust_prop","0");
	lpRecord->SetString("futu_entrust_price","9160.0");
	lpRecord->SetString("entrust_amount","1");
	lpRecord->SetString("entrust_kind","0");
	//lpRecord->SetString("volume_condition","0");
	CTimeCounter t1;
	t1.Start();
	//同步接收消息
	iRet = lpComm->SyncSendRecv(lpReqMsg,lpAnsMsg);
	t1.Stop();
	cout<<"同步委托耗时:"<<":"<<t1.Elapse()<<endl;
	if(iRet != 0)
	{
	cout<<"同步委托失败:iRet="<<iRet<<" msg:"<<lpComm->GetErrorMsg(iRet)<<endl;
	}
	else
	{
	cout<<"同步委托成功!"<<endl;
	}
	ShowFuMessage(lpAnsMsg);

	//组合委托代码检查
	IFuMessage* lpCheckCombinReqMsg = NewFuMessage(MSG_TYPE_CHECK_COMBIN_CODE,MSG_MODE_REQUEST); //委托下单
	IFuMessage* lpCheckCombinAnsMsg = NewFuMessage();     //接收消息(无关消息类型和模式,SDK会置相关类型)

	//打包请求参数(字段顺序无关,重复设置字段则覆盖其值)
	IFuRecord* lpCheckCombinRecord = lpCheckCombinReqMsg->AddRecord();
	lpCheckCombinRecord->SetString("fund_account",USER_NAME);
	lpCheckCombinRecord->SetString("futu_exch_type","F1");
	lpCheckCombinRecord->SetString("futures_account","");
	lpCheckCombinRecord->SetString("contract_code","CF109");
	lpCheckCombinRecord->SetString("entrust_bs","1");
	lpCheckCombinRecord->SetString("futures_direction","1");
	lpCheckCombinRecord->SetString("second_code","CF111");
	lpCheckCombinRecord->SetString("arbitrage_code","CF109&CF111");
	CTimeCounter t2;
	t2.Start();
	//同步接收消息
	iRet = lpComm->SyncSendRecv(lpCheckCombinReqMsg,lpCheckCombinAnsMsg);
	t2.Stop();
	cout<<"同步委托耗时:"<<":"<<t2.Elapse()<<endl;
	if(iRet != 0)
	{
	cout<<"同步委托失败:iRet="<<iRet<<" msg:"<<lpComm->GetErrorMsg(iRet)<<endl;
	}
	else
	{
	cout<<"同步委托成功!"<<endl;
	}
	ShowFuMessage(lpCheckCombinAnsMsg);
	lpCheckCombinReqMsg->Release();
	lpCheckCombinAnsMsg->Release();

	pause();
	*/

	//持仓查询
	IFuMessage* lpReqMsgHOLD = NewFuMessage(MSG_TYPE_GET_HOLDSINFO, MSG_MODE_REQUEST); //委托下单
	IFuMessage* lpAnsMsgHOLD = NewFuMessage();  //接收消息(无关消息类型和模式,SDK会置相关类型)
	IFuRecord*  lpRecordHOLD = lpReqMsgHOLD->AddRecord();
	lpRecordHOLD->SetString("fund_account", USER_NAME);
	lpRecordHOLD->SetString("begin_date", "20120202");
	lpRecordHOLD->SetString("end_date", "20120206");
	lpRecordHOLD->SetString("money_type", "0");
	lpRecordHOLD->SetString("total_type", "1");

	CTimeCounter tHOLD;
	tHOLD.Start();
	lpComm->SyncSendRecv(lpReqMsgHOLD, lpAnsMsgHOLD);
	tHOLD.Stop();
	cout << "同步委托耗时:" << ":" << tHOLD.Elapse() << endl;
	ShowFuMessage(lpAnsMsgHOLD);

	lpReqMsgHOLD->Release();
	lpAnsMsgHOLD->Release();

	pause();

	/*
	//组合委托下单
	IFuMessage* lpReqMsg = NewFuMessage(MSG_TYPE_NEW_COMBIN_ORDER,MSG_MODE_REQUEST); //委托下单
	IFuMessage* lpAnsMsg = NewFuMessage();     //接收消息(无关消息类型和模式,SDK会置相关类型)

	//打包请求参数(字段顺序无关,重复设置字段则覆盖其值)
	IFuRecord* lpRecord = lpReqMsg->AddRecord();
	lpRecord->SetString("fund_account",USER_NAME);
	lpRecord->SetString("futu_exch_type","F1");
	lpRecord->SetString("futures_account","");
	lpRecord->SetString("contract_code","CF109");
	lpRecord->SetString("entrust_bs","1");
	lpRecord->SetString("futures_direction","1");
	lpRecord->SetString("hedge_type","0");
	lpRecord->SetString("futu_entrust_prop","7");
	lpRecord->SetString("futu_entrust_price","0");
	lpRecord->SetString("entrust_amount","1");
	lpRecord->SetString("spring_price","0");
	lpRecord->SetString("time_condition","3");
	lpRecord->SetString("valid_date","0");
	lpRecord->SetString("second_code","CF111");
	lpRecord->SetString("volume_condition","0");
	lpRecord->SetString("weave_type","7");
	lpRecord->SetString("arbitrage_code","CF109&CF111");
	CTimeCounter t1;
	t1.Start();
	//同步接收消息
	iRet = lpComm->SyncSendRecv(lpReqMsg,lpAnsMsg);
	t1.Stop();
	cout<<"同步委托耗时:"<<":"<<t1.Elapse()<<endl;
	if(iRet != 0)
	{
	cout<<"同步委托失败:iRet="<<iRet<<" msg:"<<lpComm->GetErrorMsg(iRet)<<endl;
	}
	else
	{
	cout<<"同步委托成功!"<<endl;
	}
	ShowFuMessage(lpAnsMsg);

	//释放消息
	lpReqMsg->Release();
	lpAnsMsg->Release();

	pause();
	*/
	/*
	//查询组合代码
	IFuMessage* lpGetCombinCodeReqMsg = NewFuMessage(MSG_TYPE_GET_COMBIN_CODE,MSG_MODE_REQUEST); //委托下单
	IFuMessage* lpGetCombinCodeAnsMsg = NewFuMessage();     //接收消息(无关消息类型和模式,SDK会置相关类型)

	//打包请求参数(字段顺序无关,重复设置字段则覆盖其值)
	IFuRecord* lpGetCombinCodeRecord = lpGetCombinCodeReqMsg->AddRecord();
	lpGetCombinCodeRecord->SetString("fund_account",USER_NAME);
	lpGetCombinCodeRecord->SetString("futu_exch_type","F1");
	lpGetCombinCodeRecord->SetString("request_num","");
	CTimeCounter t3;
	t3.Start();
	//同步接收消息
	iRet = lpComm->SyncSendRecv(lpGetCombinCodeReqMsg,lpGetCombinCodeAnsMsg);
	t3.Stop();
	cout<<"同步委托耗时:"<<":"<<t3.Elapse()<<endl;
	if(iRet != 0)
	{
	cout<<"同步委托失败:iRet="<<iRet<<" msg:"<<lpComm->GetErrorMsg(iRet)<<endl;
	}
	else
	{
	cout<<"同步委托成功!"<<endl;
	}
	ShowFuMessage(lpGetCombinCodeAnsMsg);

	//释放消息
	lpGetCombinCodeReqMsg->Release();
	lpGetCombinCodeAnsMsg->Release();

	pause();
	*/

	/*
	//查询组合行情
	IFuMessage* lpGetCombinQuoteReqMsg = NewFuMessage(MSG_TYPE_GET_COMBIN_QUOTE,MSG_MODE_REQUEST); //委托下单
	IFuMessage* lpGetCombinQuoteAnsMsg = NewFuMessage();     //接收消息(无关消息类型和模式,SDK会置相关类型)

	//打包请求参数(字段顺序无关,重复设置字段则覆盖其值)
	IFuRecord* lpGetCombinQuoteRecord = lpGetCombinQuoteReqMsg->AddRecord();
	lpGetCombinQuoteRecord->SetString("fund_account",USER_NAME);
	lpGetCombinQuoteRecord->SetString("futu_exch_type","");
	lpGetCombinQuoteRecord->SetString("arbicontract_id","");
	CTimeCounter t4;
	t4.Start();
	//同步接收消息
	iRet = lpComm->SyncSendRecv(lpGetCombinQuoteReqMsg,lpGetCombinQuoteAnsMsg);
	t4.Stop();
	cout<<"同步委托耗时:"<<":"<<t4.Elapse()<<endl;
	if(iRet != 0)
	{
	cout<<"同步委托失败:iRet="<<iRet<<" msg:"<<lpComm->GetErrorMsg(iRet)<<endl;
	}
	else
	{
	cout<<"同步委托成功!"<<endl;
	}
	ShowFuMessage(lpGetCombinQuoteAnsMsg);

	//释放消息
	lpGetCombinQuoteReqMsg->Release();
	lpGetCombinQuoteAnsMsg->Release();

	pause();
	*/
	/*
	//校验密码
	IFuMessage* lpGetCheckPwdReqMsg = NewFuMessage(MSG_TYPE_CHECK_PWD,MSG_MODE_REQUEST); //委托下单
	IFuMessage* lpGetCheckPwdAnsMsg = NewFuMessage();     //接收消息(无关消息类型和模式,SDK会置相关类型)

	//打包请求参数(字段顺序无关,重复设置字段则覆盖其值)
	IFuRecord* lpGetCheckPwdRecord = lpGetCheckPwdReqMsg->AddRecord();
	lpGetCheckPwdRecord->SetString("fund_account",USER_NAME);
	lpGetCheckPwdRecord->SetString("password",USER_PWD);
	CTimeCounter t5;
	t5.Start();
	//同步接收消息
	iRet = lpComm->SyncSendRecv(lpGetCheckPwdReqMsg,lpGetCheckPwdAnsMsg);
	t5.Stop();
	cout<<"同步委托耗时:"<<":"<<t5.Elapse()<<endl;
	if(iRet != 0)
	{
	cout<<"同步委托失败:iRet="<<iRet<<" msg:"<<lpComm->GetErrorMsg(iRet)<<endl;
	}
	else
	{
	cout<<"同步委托成功!"<<endl;
	}
	ShowFuMessage(lpGetCheckPwdAnsMsg);

	//释放消息
	lpGetCheckPwdReqMsg->Release();
	lpGetCheckPwdAnsMsg->Release();
	*/
	/*if(0 == iRet)
	{
	//撤单请求
	cout<<"Press any key to cancel an order....."<<endl;
	pause();
	lpRecord->Clear();
	lpReqMsg->SetMsgType(MSG_TYPE_CANCEL_ORDER,MSG_MODE_REQUEST);

	lpRecord->SetString("entrust_no",lpAnsMsg->GetRecord()->GetString("entrust_no"));   //委托号
	lpRecord->SetString("fund_account",USER_NAME);
	lpRecord->SetString("password",USER_PWD);

	//异步发送消息
	lpAnsMsg->Clear();
	iRet = lpComm->SyncSendRecv(lpReqMsg,lpAnsMsg);
	if(0 != iRet)
	{
	cout<<"撤单失败:iRet="<<iRet<<" msg:"<<lpComm->GetErrorMsg(iRet)<<endl;
	}
	else
	{
	cout<<"撤成功,Show"<<endl;
	}
	ShowFuMessage(lpAnsMsg);
	}
	*/



	pause();

	//登出
	pause();
	lpComm->SubscribeRequest(NAType, CxlAll, ALLWWW);
	pause();
	lpComm->DoLogout(USER_NAME);
	//pause();
	//lpComm->DoLogout("");
	pause();
	//lpComm->DoLogout("20035");
	//暂停等待登出结果
	pause();

#ifdef _WIN32
	//关闭自动重连线程
	g_bStop = true;
	SetEvent(g_hEvent);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
#else

#endif

	//关闭通信连接
	lpComm->Stop();

	//释放通信对象
	lpComm->Release();
}


