function GetNewPosition(nPos, nMax, nChange)
	if not nPos or not nMax or not nChange then
		return 0;
	end
	local nNew = (nPos + nChange) % nMax;
	if nNew == 0 then
		nNew = nMax;
	end
	return nNew;
end

-- print(GetNewPosition(1, 3, -1))
-- local nMax = 5
-- local nChange = -1;
-- for i=1, nMax do
	-- local simbol = (nChange > 0) and "+" or ""
	-- print(i .. " " .. simbol .. nChange .. "->" .. GetNewPosition(i, nMax, nChange))
-- end

-- 2015/09/17: 找曲线
local nMax = 7;
local nBase = 2
for i=0.1, 0.6, 0.1 do
	local x = i;
	local n = nBase^x;
	-- print(n);
end

-- 随机函数
function gf_Random(nBegin, nEnd)
	--test
	if _DEBUG_ then
		g_nTestCount = g_nTestCount + 1;
		if not nBegin or not nEnd then
			print("ni ma de ")
		end
		print("g_nRandomSeed", g_nRandomSeed, nBegin, nEnd)
	end
	--test end
	g_nRandomSeed = g_nRandomSeed or 1;
	nEnd = nEnd or 1;
	nBegin = nBegin or nEnd;
	g_nRandomSeed = (g_nRandomSeed * 3877 + 29573) % 0xffffffff;
	if nEnd < nBegin then
		nBegin, nEnd = nEnd, nBegin;
	end
	--test
	if _DEBUG_ then
		local seed = io.open('rand.txt', 'a+');
		seed:write(g_nRandomSeed .. "," .. (nBegin + g_nRandomSeed % (nEnd - nBegin + 1)) .. "," .. nBegin .. "," .. nEnd .. '\n');
		seed:close();
	end
	--test end
	return nBegin + g_nRandomSeed % (nEnd - nBegin + 1)
end

-- print(1, gf_Random(1), math.random(1));
-- print(1, gf_Random(10), math.random(10));
-- for i = 1, 1000 do
	-- print(i, gf_Random(i), math.random(i));
-- end

--0-1背包
local tbValue = {10, 40, 30, 50};
local tbWeight = {5, 4, 6, 3};
local n = 4;
local w = 10;

local tbRet = {};
function bag(n, w, tbValue, tbWeight)
	-- 初始化所有列
	for row = 0, n do
		tbRet[row] = {};
	end
	
	-- 初始化第一行
	for col = 0, w do
		tbRet[0][col] = 0;
	end
	
	-- 开始计算
	for row = 1, n do
		tbRet[row][0] = 0;
		for col = 1, w do
			local wT = tbWeight[row];
			if wT <= col then
				local nNew = tbValue[row] + tbRet[row-1][col-wT];
				tbRet[row][col] = math.max(nNew, tbRet[row-1][col]);
			else
				tbRet[row][col] = tbRet[row-1][col];
			end
		end
	end
	
	-- 打印计算结果
	for row = 0, n do
		local szRet = "";
		for col = 0, w do
			local szTemp = string.format("%5d", tbRet[row][col]);
			szRet = szRet .. szTemp .. ",";
		end
		-- print(szRet);
	end
	print("the ret is: ", tbRet[n][w]);
	
	-- 求解
	local tbSelect = {};
	local col = w;
	for row = n, 1, -1 do
		if tbRet[row][col] > tbRet[row-1][col] then
			tbSelect[row] = true;
			col = col - tbWeight[row];
		else
			tbSelect[row] = false;
		end
	end
	
	-- 打印求解结果
	for row, ret in pairs(tbSelect) do
		if ret then
			print("the item " .. row .. "(" .. tbValue[row] .. ")" .. "is selected!");
		end
	end
end

-- eg1: 
local tbValue = {8, 14, 18};
local tbWeight = {19, 38, 59};
local n = 3;
local w = 100;
bag(n, w, tbValue, tbWeight);


------------------------------------------------------------
-- 同生日的问题
-- 1. 50个人
function fact( n )
	if n <= 1 then
		return 1;
	else
		return n * fact(n-1);
	end
end
local n1 = (fact(365) / ( fact(50) * fact(365-50))) / (365 ^ 50);
print(n1)

-- 大正数计算
function calcLargeNum( n )
	local YEAR = 365;
	local nRet = 1;
	for i = 0, n - 1 do
		print( "i = ", i)
		nRet = nRet * (YEAR - i) / YEAR;
	end

	print("nRet = ", 1 - nRet);
	return 1 - nRet;
end

calcLargeNum(50)

-- 大正数
BigInt = {
	
}