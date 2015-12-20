
-- desc: 时间函数
------------------------------------------------
-- 1. 指定时间对应的描述
-- year表示年,month表示月,day表示日期,hour表示小时,min表示分钟,sec表示秒,isdst表示是否夏令时 
local tab = {year=2015, month=12, day=8, hour=11,min=30,sec=0,isdst=false}
local time = os.time(tab);

print(1, os.time(), os.time(tab), os.time(tab) - os.time());

-- 2. 获取当前时间和某个时间点的描述
tab = os.date("*t", os.time())
-- for k, v in pairs(tab) do
	-- print (k, v)
-- end
function tf_CalcDiffTime(nHour, nMin, nSec)
	local nNow = os.time();
	local tbNow = os.date("*t", nNow);
	local tbTime = {
		year = tbNow.year, 
		month = tbNow.month, 
		day = tbNow.day, 
		hour = nHour or 0,
		min= nMin or 0,
		sec= nSec or 0,
		isdst = false}
	local nTime = os.time(tbTime);
	local nDiff = nTime - nNow;
	print("diff = ", nDiff);
	return nDiff;
end

-- tf_CalcDiffTime(11, 40, 0);

function tf_FormatSecond(nSecond)
	local nSecond = nSecond or 0;
	local nHour = math.floor(nSecond / 3600)
	local nTemp = nSecond % 3600
	local nMin = math.floor(nTemp / 60)
	local nSec = nTemp % 60
	
	local szFormat = string.format("%02d:%02d:%02d", nHour, nMin, nSec)
	return szFormat;
end

function tf_FormatWithTime(nHour, nMin, nSec)
	local nNow = os.time();
	local tbNow = os.date("*t", nNow);
	local tbTime = {
		year = tbNow.year, 
		month = tbNow.month, 
		day = tbNow.day, 
		hour = nHour or 0,
		min= nMin or 0,
		sec= nSec or 0,
		isdst = false
	}
	
	local nTime = os.time(tbTime);
	local nDiff = os.difftime(nTime, nNow);
	
	return tf_FormatSecond(nDiff);
end
print(tf_FormatWithTime(11,50, 0));