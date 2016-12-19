/**********************************************************************
* 源程序名称: futu_data_types.h
* 软件著作权: 恒生电子股份有限公司
* 系统名称  : 06版本期货系统
* 模块名称  : 恒生期货周边接口
* 功能说明  : 周边接口常用数据类型定义
* 作    者  : xdx
* 开发日期  : 20110315
* 备    注  : 数据类型定义   
* 修改人员  ：
* 修改日期  ：
* 修改说明  ：20110315 创建
**********************************************************************/
#ifndef _FUTU_DATA_TYPES_H_
#define _FUTU_DATA_TYPES_H_

/**********************************************************************/
/*                                                                    */
/*                                 常量定义                           */
/*                                                                    */
/**********************************************************************/
//定义SDK的版本信息
//每次更新SDK,此值会更新,客户程序需检查此版本的需求,以使头文件定义和SDK库匹配
//相信请参考示例demo
#define HSFUSDK_VERSION     0x10000010
#define HSFUSDK_VERSTRING   "V1.0.0.10" 

//定义连接服务类型
const int SERVICE_TYPE_TRADE = 1;//交易服务
const int SERVICE_TYPE_QUOTE = 2;//行情回报服务

//行情常量定义(全市场行情的订阅代码)
const char* const ALLWWW = "ALLWWW";

//订阅类型
enum REGType
{
	NAType          = -1,     // 未知类型
	SingleCode      = 0,      // 单腿行情
	RspReport       = 1,      // 委托回报
	CombinCode      = 2,      // 组合行情
	OnlineMsg       = 3       // 在线消息
};

//请求方式(订阅或退订) 
enum REGAction
{
	NAAction        = -1,     // 未知请求方式
	Subscription    = 0,      // 订阅刷新(主要指行情)
	CxlAll          = 1,      // 取消全部的订阅
	CxlFlag         = 2,      // 根据制定类型订阅
	Snapshot        = 3       // 查询快照(主要指行情)
};

//连接的状态
enum CONState
{
	Uninitialized   = -1,     // 连接未初始化
	Disconnected	= 0x0000, // 未连接
	Connecting		= 0x0001, // socket正在连接
	Connected		= 0x0002, // socket已连接
	SafeConnecting	= 0x0004, // 正在建立安全连接
	SafeConnected	= 0x0008, // 已建立安全连接
	Registering		= 0x0010, // 正注册
	Registered		= 0x0020, // 已注册
	Rejected		= 0x0040  // 被拒绝,将被关闭
};

//消息模式定义(请求应答类型定义)
enum FUTU_MSG_MODE
{
	MSG_MODE_UNKNOWN = -1, //未知消息模式
	MSG_MODE_REQUEST = 0,  //表示是请求消息
	MSG_MODE_ANSWER  = 1   //表示是应答消息
};

//期货消息类型(注意：对客户发放此头文件的时候需要屏蔽部分消息类型)
enum FUTU_MSG_TYPE
{
	MSG_TYPE_UNKNOWN               = -1, //未知消息类型

	MSG_TYPE_USER_LOGIN            = 100,//用户登录
	MSG_TYPE_USER_LOGOUT           = 101,//UFT客户注销
	MSG_TYPE_CONFIRM_BILL          = 102,//客户确认账单
	MSG_TYPE_CHECK_CONTRACT_CODE   = 103,//检查合约代码(单腿)
	MSG_TYPE_CHECK_ENTRUST_PRICE   = 104,//检查委托价格
	MSG_TYPE_NEW_SINGLE_ORDER      = 105,//委托下单
	MSG_TYPE_CANCEL_ORDER          = 106,//委托撤单
	MSG_TYPE_BANK_TRANSFER         = 107,//银期转账
	MSG_TYPE_MODIFY_PASSWORD       = 108,//客户修改密码

	//20110822 luyj 增加组合委托 修改单号:20110819035
	MSG_TYPE_CHECK_COMBIN_CODE     = 109,//检查组合合约代码
	MSG_TYPE_NEW_COMBIN_ORDER      = 110,//组合委托确认
	//20110822 end

	MSG_TYPE_GET_TRADING_CODE      = 200,//交易编码查询
	MSG_TYPE_GET_PROFIT            = 201,//期货客户资金权益查询
	MSG_TYPE_GET_HOLDSINFO         = 202,//持仓查询
	MSG_TYPE_GET_ENTRUST_ORDERS    = 203,//委托查询
	MSG_TYPE_GET_TRANS_DETAIL      = 204,//成交明细查询
	MSG_TYPE_GET_FUNDJOUR          = 205,//历史资金流水查询
	MSG_TYPE_GET_FUND_HISTRANSJOUR = 206,//查历史转账流水
	MSG_TYPE_GET_BANK_ACCOUNT      = 207,//银行账号查询   
	MSG_TYPE_GET_BANK_TRANSJOUR    = 208,//银期转账流水查询
	MSG_TYPE_GET_BANKBALA          = 209,//银行账户余额查询
	MSG_TYPE_GET_MARKET_DATA       = 210,//期货行情查询
	MSG_TYPE_GET_MARGIN            = 211,//合约保证金查询
	MSG_TYPE_GET_FUTU_BANKINFO     = 212,//期货登记银行信息查询
	MSG_TYPE_GET_EXCH_TIME         = 213,//交易所时间差查询 
	MSG_TYPE_GET_FMMC_PWD          = 214,//监控中心结算单系统密码查询
	MSG_TYPE_GET_BILL              = 215,//结算单查询
	MSG_TYPE_GET_HIS_ENTRUST       = 216,//历史委托查询
	MSG_TYPE_GET_HIS_BUSINESS      = 217,//历史成交查询
	MSG_TYPE_GET_HIS_FUND          = 218,//历史资金查询
	MSG_TYPE_UFT_ORDERHSACK        = 219,//委托反馈
	MSG_TYPE_UFT_ORDEREXACK        = 220, //成交反馈

	//20110822 luyj 增加组合委托相关查询(06) 修改单号:20110819035
	MSG_TYPE_GET_COMBIN_CODE       = 221, //查询组合代码
	MSG_TYPE_GET_COMBIN_QUOTE      = 222,  //查询组合行情
	MSG_TYPE_CHECK_PWD             = 223   //校验密码
	//20110822 end


};

/**********************************************************************/
/*                                                                    */
/*                              数据结构定义                          */
/*                                                                    */
/**********************************************************************/
//4字节对齐
#pragma pack(push,4)

//定义单腿行情的结构
struct CMarketInfo
{
	char            contract_code[13];  //0 合约代码,
	double          pre_square_price;   //1 昨日交割结算价
	double          futu_open_price;    //2 开盘价
	double          futu_last_price;    //3 最新价格
	double          buy_high_price;     //4 最高买入价
	unsigned int    buy_high_amount;    //5 最高买价买入量
	unsigned int    buy_total_amount;   //6 全部买量
	double          sale_low_price;     //7 最低卖价价格
	unsigned int    sale_low_amount;    //8 最低卖价卖量
	unsigned int    sale_total_amount;  //9 全部卖量
	double          futu_high_price;    //10 最高价
	double          futu_low_price;     //11 最低价
	double          average_price;      //12 均价
	double          change_direction;   //13 趋势
	int             business_amount;    //14 成交量
	int             bear_amount;        //15 总持量
	double          business_balance;   //16 成交额
	double          uplimited_price;    //17 涨停板
	double          downlimited_price;  //18 跌停板
	char            futu_exch_type[3];  //19 交易类别
	double          form_buy_price;     //20 组合买入价格
	double          form_sale_price;    //21 组合卖出价格
	int             form_buy_amount;    //22 组合买入数量
	int             form_sale_amount;   //23 组合卖出数量
	double          pre_close_price;    //24 昨日收盘价
	double          pre_open_interest;  //25 昨日空盘量
	double          futu_close_price;   //26 今日收盘价
	double          square_price;       //27 结算价
	double          pre_delta;          //28 昨日虚实度
	double          curr_delta;         //29 今日虚实度
	double          bid_price2;         //30 买二价
	unsigned int    bid_volume2;        //31 买二量
	double          bid_price3;         //32 买三价
	unsigned int    bid_volume3;        //33 买三量
	double          bid_price4;         //34 买四价
	unsigned int    bid_volume4;        //35 买四量
	double          bid_price5;         //36 买五价
	unsigned int    bid_volume5;        //37 买五量
	double          ask_price2;         //38 卖二价
	unsigned int    ask_volume2;        //39 卖二量
	double          ask_price3;         //40 卖三价
	unsigned int    ask_volume3;        //41 卖三量
	double          ask_price4;         //42 卖四价
	unsigned int    ask_volume4;        //43 卖四量
	double          ask_price5;         //44 卖五价
	unsigned int    ask_volume5;        //45 卖五量
};


//定义组合行情的结构
struct CArgMarketInfo
{
	char         arbicontract_id[31];  //0 套利合约号
	char         futu_exch_type[3];    //1 交易类别
	char         first_code[13];       //2 第一腿
	char         second_code[13];      //3 第二腿
	char         arbi_type;            //4 组合类型1-SPD, 2 -IPS
	double       buy_price;            //5 最高买入价
	unsigned int buy_amount;           //6 最高买入价买量
	unsigned int buy_total_amount;     //7 全部买量
	double       sale_price;           //8 最低卖价价格
	unsigned int sale_amount;          //9 最低卖价卖量
	unsigned int sale_total_amount;    //10 全部卖量
	double       futu_high_price;      //11 最高价
	double       futu_low_price;       //12 最低价
	double       uplimited_price;      //13 涨停板价格
	double       downlimited_price;    //14 跌停板价格
};


//委托反馈信息
struct COrderRspInfo
{
	int          entrust_no;            //0 委托号
	char         futures_account[21];   //1 交易编码
	char         futu_exch_type[11];    //2 交易所类别
	char         contract_code[13];     //3 合约代码
	char         entrust_bs[9];       //4 买卖标识
	char         entrust_direction[9];     //6 开平标识
	char         hedge_type[9];         //7 套保标识
	int          fund_account;          //8 资金账户
	char         futu_report_no[21];    //9 本地单号
	char         firm_no[9];            //10 会员号
	char         operator_no[9];        //11 操作员号
	int          client_group;          //12 客户类别
	int          entrust_amount;        //13 委托数量
	int          business_total_amount; //14 成交总数量
	int          cacel_amount;          //15 撤单数量
	double       entrust_price;         //16 委托价格
	char         entrust_status;        //17 委托状态
	int          branch_no;             //18 营业部号
	int          batch_no;              //19 委托批号
	char         futu_entrust_type;     //20 委托类型
	int          amount_per_hand;       //21 合约称数
	char         forceclose_reason;     //22 强平原因
	int          init_date;             //23 交易日期
	int          curr_time;             //24 当前时间
	char         confirm_no [21];       //25 主场单号
	int          weave_type;            //26 组合委托类型
	char         arbitrage_code [21];   //27 套利合约代码
	int          time_condition;        //28 有效期类型
	int          volume_condition;      //29 成交量类型
	int          futu_entrust_prop;     //30 期货委托属性
	double       frozen_fare;           //31 冻结总费用
};

//成交反馈信息
struct CRealRspInfo
{
	int          entrust_no;            //0 委托号
	char         futures_account[21];   //1 交易编码
	char         futu_exch_type[11];    //2 交易所类别
	char         business_no[21];       //3 成交编号
	char         contract_code[13];     //4 合约代码
	char         entrust_bs[9];		//5 买卖标识
	char         entrust_direction[8];     //6 开平标识
	double       business_price;        //7 成交价格
	double       business_amount;       //8 成交数量  
	char         hedge_type[9];         //9 套保标识
	int          fund_account;          //10 资金账户
	char         futu_report_no[21];    //11 本地单号
	char         firm_no[9];            //12 会员号
	char         operator_no[9];        //13 操作员号
	int          client_group;          //14 客户类别
	int          entrust_amount;        //15 委托数量
	int          business_total_amount; //16 成交总数量
	int          cacel_amount;          //17 撤单数量
	double       entrust_price;         //18 委托价格
	char         entrust_status;        //19 委托状态
	int          branch_no;             //20 营业部号
	int          batch_no;              //21 委托批号
	char         futu_entrust_type;     //22 委托类型
	int          amount_per_hand;       //23 合约称数
	char         forceclose_reason;     //24 强平原因
	int          init_date;             //25 交易日期
	int          business_time;         //26 成交时间
	char         confirm_no[21];        //27 主场单号
	double       frozen_fare;           //28 冻结总费用
};

#pragma pack(pop)

#endif//end of futu_data_types.h


