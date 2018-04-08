// ZQMD.h: interface for the CZQMD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZQMD_H__E1C8C421_0DB0_4129_AC7E_7DF1BB1C6DFC__INCLUDED_)
#define AFX_ZQMD_H__E1C8C421_0DB0_4129_AC7E_7DF1BB1C6DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "api\ThostFtdcMdApi.h"

class CZQMD : public CThostFtdcMdSpi  
{
public:
	void OnFrontConnected();
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	void OnFrontDisconnected(int nReason);
	void Init();
	void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void RegisterFront(char *pszFrontAddress);
	CZQMD();
	virtual ~CZQMD();

private:
	CThostFtdcMdApi * m_pMdApi;
};

#endif // !defined(AFX_ZQMD_H__E1C8C421_0DB0_4129_AC7E_7DF1BB1C6DFC__INCLUDED_)
