// ZQMD.cpp: implementation of the CZQMD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZQMD.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZQMD::CZQMD()
{

}

CZQMD::~CZQMD()
{

}

void CZQMD::Init()
{
	m_pMdApi=CThostFtdcMdApi::CreateFtdcMdApi(".\\flow\\md");// 产生一个CThostFtdcMdApi实例
	m_pMdApi->RegisterSpi(this);

	m_pMdApi->RegisterFront("tcp://180.169.124.100:41213");
	m_pMdApi->Init();

}

void CZQMD::OnFrontConnected()
{
	CThostFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin,0,sizeof(reqUserLogin));
	strcpy(reqUserLogin.BrokerID,"1032");
	strcpy(reqUserLogin.UserID,"00000030");
	strcpy(reqUserLogin.Password,"123456");
	// 发出登陆请求
	m_pMdApi->ReqUserLogin(&reqUserLogin,1);
}
void CZQMD::OnFrontDisconnected(int nReason)
{
	printf("I am in OnFrontDisconnected!\n");
}
void CZQMD::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printf("I am in OnRspUserLogin!\n");
	//订阅行情
	char * Instruments[10];
	memset(Instruments,0,sizeof(Instruments));
	//Instruments[0] = "IF1406";
	Instruments[0] = "IF1406";

	m_pMdApi->SubscribeMarketData (Instruments,10);
	//m_pMdApi->UnSubscribeMarketData(Instruments, 10);
}
void CZQMD::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) 
{
	printf("OnRtnDepthMarketData:%s!%d!updateTime:%s!\n",pDepthMarketData->InstrumentID,
		pDepthMarketData->Volume,pDepthMarketData->UpdateTime);
}
