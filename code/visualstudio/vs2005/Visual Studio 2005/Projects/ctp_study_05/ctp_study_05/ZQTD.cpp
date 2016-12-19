// ZQTD.cpp: implementation of the CZQTD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZQTD.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZQTD::CZQTD()
{

}

CZQTD::~CZQTD()
{

}

void CZQTD::Init()
{

	// 产生一个CThostFtdcTraderApi实例
	m_pTdApi = CThostFtdcTraderApi::CreateFtdcTraderApi(".\\flow\\td");

	// 注册一事件处理的实例
	m_pTdApi->RegisterSpi(this);
	
	// 订阅私有流
	//        TERT_RESTART:从本交易日开始重传
	//        TERT_RESUME:从上次收到的续传
	//        TERT_QUICK:只传送登录后私有流的内容
	m_pTdApi->SubscribePrivateTopic(THOST_TERT_RESTART);
	
	// 订阅公共流
	//        TERT_RESTART:从本交易日开始重传
	//        TERT_RESUME:从上次收到的续传
	//        TERT_QUICK:只传送登录后公共流的内容
	m_pTdApi->SubscribePublicTopic(THOST_TERT_RESTART);

	// 设置交易托管系统服务的地址，可以注册多个地址备用
	m_pTdApi->RegisterFront("tcp://172.16.126.104:29407");

	// 使客户端开始与后台服务建立连接
	m_pTdApi->Init();
}

void CZQTD::OnFrontConnected()
{
	CThostFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin,0,sizeof(reqUserLogin));
	strcpy(reqUserLogin.BrokerID,"2011");
	strcpy(reqUserLogin.UserID,"698048");
	strcpy(reqUserLogin.Password,"1");
	// 发出登陆请求
	m_pTdApi->ReqUserLogin(&reqUserLogin, 1);

}
void CZQTD::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
//查询合约手续费率-暂未提供//////////////////////////////////////////////////////////////////////////////////////////////
	CThostFtdcQryInstrumentCommissionRateField QryInstrumentCommissionRate;
	memset(&QryInstrumentCommissionRate,0,sizeof(QryInstrumentCommissionRate));
	strcpy(QryInstrumentCommissionRate.BrokerID,"2011");
	strcpy(QryInstrumentCommissionRate.InvestorID,"698048");
//	strcpy(QryInstrumentCommissionRate.ExchangeID,"SSE");
	//strcpy(QryInstrumentCommissionRate.InstrumentID,"601857");
	m_pTdApi->ReqQryInstrumentCommissionRate(&QryInstrumentCommissionRate,1);

//查询股东账号//////////////////////////////////////////////////////////////////////////////////////////////
	CThostFtdcQryTradingCodeField QryTradingCode;
	memset(&QryTradingCode,0,sizeof(QryTradingCode));
	strcpy(QryTradingCode.BrokerID,"2011");
	strcpy(QryTradingCode.InvestorID,"698048");

	m_pTdApi->ReqQryTradingCode(&QryTradingCode, 1);
//查询报单//////////////////////////////////////////////////////////////////////////////////////////////
	CThostFtdcQryOrderField QryOrder;
	memset(&QryOrder,0,sizeof(QryOrder));
	strcpy(QryOrder.BrokerID,"2011");
	strcpy(QryOrder.InvestorID,"698048");

	m_pTdApi->ReqQryOrder(&QryOrder, 1);

//////////////////////////////////////////
	CThostFtdcQryInstrumentField qryStruct;
	memset(&qryStruct,0,sizeof(qryStruct));
	//m_pTdApi->ReqQryInstrument(&qryStruct,1);

//报单//////////////////////////////////////////////////////////////////////////////////////////////
	CThostFtdcInputOrderField pInputOrder;
	
	memset(&pInputOrder,0,sizeof(pInputOrder));
	CThostFtdcInputOrderField * pIptOrdFld=&pInputOrder;

	strcpy(pIptOrdFld->BrokerID,"2011");
	strcpy(pIptOrdFld->InvestorID,"698048");
	strcpy(pIptOrdFld->InstrumentID,"601857");
	//strcpy(pIptOrdFld->ExchangeID,"SSE");
	pIptOrdFld->OrderPriceType=THOST_FTDC_OPT_AnyPrice;//TZQThostFtdcOrderPriceTypeType	
	pIptOrdFld->Direction=THOST_FTDC_D_Buy;
	pIptOrdFld->VolumeTotalOriginal=100;
	pIptOrdFld->TimeCondition = THOST_FTDC_TC_GFD;///当日有效
	pIptOrdFld->VolumeCondition=THOST_FTDC_VC_AV;
	pIptOrdFld->ContingentCondition = THOST_FTDC_CC_Immediately;
	pIptOrdFld->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	//strcpy(pIptOrdFld->LimitPrice,"9.12");
//////////////////////////////////////////////////////////////////////////////////////////////////////
	//strcpy(pIptOrdFld->UserID,"698048");// 用户代码
	//strcpy(pIptOrdFld->OrderRef,"1"); ///报单引用
	//pIptOrdFld->CombOffsetFlag[0]=THOST_FTDC_OF_Open;
	//pIptOrdFld->CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
	//strcpy(pIptOrdFld->GTDDate,"20120202");
	//pIptOrdFld->MinVolume=0;
	//pIptOrdFld->IsAutoSuspend = 0;
	//pIptOrdFld->UserForceClose = 0;
	//pIptOrdFld->RequestID=1;

	m_pTdApi->ReqOrderInsert(pIptOrdFld, 1);
}
void CZQTD::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pInstrument!=NULL)
		printf("Instrument:%s;Name:%s!\n",pInstrument->InstrumentID,pInstrument->InstrumentName);
}
void CZQTD::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	return;
}
void CZQTD::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
 {
	return;
}
void CZQTD::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;
}
void CZQTD::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;
}

//ErrRSP&Rtn/////////////////////////////////////////////////////////////////////
void CZQTD::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	return;
}
void CZQTD::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	return;
}
void CZQTD::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	return;
}
//Rtn/////////////////////////////////////////////////////////////////////

void CZQTD::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	return;
}
void CZQTD::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	//if(pTrade!=NULL)
		
	//	printf("Instrument:%s;Price:%s;dbPrice:%n!\n",pTrade->InstrumentID,pTrade->Price,getPrice(pTrade->Price));
	return;
}
void CZQTD::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	return;
}
/////////////////////////////////////////////////////////////////////////////////////
/*
double CZQTD::getPrice(TThostFtdcStockPriceType price)
{
	return sizeof(price) / sizeof(price[0]);
}
*/