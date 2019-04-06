-----------------------------------------------------------------------------
--脚本功能：一些工具类的东西
--功能设计：page
--功能开发：page
--开发时间：2016/06/02 19:18
--修改记录：
--
-----------------------------------------------------------------------------
--@function: 计算某日是星期几 
--@return:
function CalcWeekday( year, month, day)
	if type(year) ~= "number" or 
		type(month) ~= "number" or
		type(day) ~= "number" then
		return;
	end
	local nWeekday = (day + 2 * month + 3 * (month + 1)/5 + year + year/4 - year/100 + year/400) % 7
	return nWeekday;
end

-- print("nWeekday = ", CalcWeekday(2016, 5, 25))

--@function: 快速阅读计算公式 
--@return:
function CalcFastReading( nWords, nTimes, nSeconds )
	local nRate = (nWords * nTimes)/nSeconds * 60;
	print("阅读速率为：", nRate)
end
-- CalcFastReading(981, 2, 131.881);


--@function: 一个整型分高低16位存放两个整型数值 
--@return:
function MergeTwo16Ints( nHight, nLow )
	local nValue = nHight * 2 ^ 16 + nLow;
	print("result: ", nValue);
end
MergeTwo16Ints(1026, 10);
MergeTwo16Ints(1027, 10);