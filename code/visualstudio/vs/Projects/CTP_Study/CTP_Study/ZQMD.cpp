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

	m_pMdApi->RegisterFront("tcp://172.16.126.104:29413");
	m_pMdApi->Init();

}

void CZQMD::OnFrontConnected()
{
	CThostFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin,0,sizeof(reqUserLogin));
	strcpy(reqUserLogin.BrokerID,"2011");
	strcpy(reqUserLogin.UserID,"698048");
	strcpy(reqUserLogin.Password,"1");
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
	Instruments[0] = "au1209";

//	m_pMdApi->SubscribeMarketData (Instruments,10);
}
void CZQMD::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) 
{
	printf("OnRtnDepthMarketData:%s!%d!updateTime:%s!\n",pDepthMarketData->InstrumentID,
		pDepthMarketData->Volume,pDepthMarketData->UpdateTime);
}
