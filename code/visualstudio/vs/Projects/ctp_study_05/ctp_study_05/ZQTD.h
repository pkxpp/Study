// ZQTD.h: interface for the CZQTD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZQTD_H__3CA7A2E1_A491_47BE_A447_669B265F00B9__INCLUDED_)
#define AFX_ZQTD_H__3CA7A2E1_A491_47BE_A447_669B265F00B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "api\ThostFtdcTraderApi.h"

class CZQTD : public CThostFtdcTraderSpi  
{
public:
	void Init();
	void OnFrontConnected();
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

	void OnRtnOrder(CThostFtdcOrderField *pOrder);
	void OnRtnTrade(CThostFtdcTradeField *pTrade);
	void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);


	CZQTD();
	virtual ~CZQTD();

private:
	//double getPrice(TZQThostFtdcStockPriceType price);
	CThostFtdcTraderApi * m_pTdApi;
};

#endif // !defined(AFX_ZQTD_H__3CA7A2E1_A491_47BE_A447_669B265F00B9__INCLUDED_)
