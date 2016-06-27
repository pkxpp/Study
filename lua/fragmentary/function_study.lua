--2014/01/14
--(1)closure:非全局函数中的递归
--[[
local fact = function(n)
	if n == 0 then
		return 1;
	else
		return n * fact(n-1)
	end
end

local fact1
fact1 = function(n)
	if n == 0 then
		return 1;
	else
		return n * fact1(n-1);
	end
end

print(fact(3))
]]

--2015/02/02/
-- local getRow = function(nPos)
-- 		local MAX_ROW = 3
-- 		nPos = nPos or 1;
-- 		local nRow = nPos%MAX_ROW
-- 		if nRow == 0 then 
-- 			nRow = MAX_ROW;
-- 		end
-- 		return nRow
-- 	end
-- 	local getCol = function(nPos)
-- 		local MAX_ROL = 3;
-- 		nPos = nPos or 1
-- 		local nCol = (math.floor((nPos-1)/3))%3 + 1
-- 		return nCol;
-- 	end

-- for i = 1, 18 do
-- 	print(i, getRow(i), getCol(i))
-- end

-- local getOrderByPos = function(nPos, tbTargets)
-- 	local MAX_MAGIC = 3
-- 	local MAX_NUM = MAX_MAGIC * MAX_MAGIC;
-- 	local tbOrder = {}
-- 	local bIsEnemy = false
-- 	local nTargetPos = nPos + MAX_NUM
-- 	if nPos > MAX_NUM then
-- 		bIsEnemy = true;
-- 		nTargetPos = nPos - MAX_NUM
-- 	end
	
-- 	local tbHashTargets = {}
-- 	for _, target in pairs(tbTargets) do
-- 		tbHashTargets[target.nPos] = true;
-- 	end
	
-- 	local tbRows = {}
-- 	local nRow = getRow(nTargetPos)
-- 	table.insert(tbRows, nRow)
-- 	for i=1, MAX_MAGIC do
-- 		if i ~= nRow then
-- 			table.insert(tbRows, i);
-- 		end
-- 	end
	
	
-- 	for _, row in ipairs(tbRows) do
-- 		for i=1, MAX_MAGIC do
-- 			local nNewPos = (i-1)*MAX_MAGIC + row
-- 			if nTargetPos > MAX_NUM then
-- 				nNewPos = (i+2)*MAX_MAGIC + row;
-- 			end
-- 			-- print("New Pos: ", nNewPos);
-- 			table.insert(tbOrder, nNewPos);
-- 		end
-- 	end
-- 	return tbOrder;
-- end
--[[
local targets = {}
--for i=1, 9 do
	local tbElem = {}
	tbElem.nPos = 5;
	table.insert(targets, tbElem)
--end

--for i=1, 9 do
	local tbOrder = getOrderByPos(10, targets)
	local tbHash = {}
	for _, v in pairs(tbOrder) do
		tbHash[v] = true;
	end
	for i=10, 18 do
		if tbHash[i] ~= true then
			print("Error!")
			return;
		end
	end
	print("All true");
--end
]]
--@function: 计算某日是星期几 
--@return:
-- function CalcWeekday( year, month, day)
-- 	if type(year) ~= "number" or 
-- 		type(month) ~= "number" or
-- 		type(day) ~= "number" then
-- 		return;
-- 	end
-- 	local nWeekday = (day + 2 * month + 3 * (month + 1)/5 + year + year/4 - year/100 + year/400) % 7
-- 	return nWeekday;
-- end

-- print("nWeekday = ", CalcWeekday(2016, 5, 25))

--@function: 快速阅读计算公式 
--@return:
function CalcFastReading( nWords, nTimes, nSeconds )
	local nRate = (nWords * nTimes)/nSeconds * 60;
	print("阅读速率为：", nRate)
end
CalcFastReading(824, 2, 115.606);
