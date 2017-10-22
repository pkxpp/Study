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
-- bag(n, w, tbValue, tbWeight);


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
-- print(n1)

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

-- calcLargeNum(50)

-- 大正数
BigInt = {
	
}

------------------------------------------------------------
-- 堆
local Heap = {
	tbData = {},
}

function Heap:print(  )
	local szHeap = table.concat(self.tbData or {}, ",")
	print("Heap: ", szHeap);
end

function Heap:build(tbData)
	local tbData = tbData or {};

	-- 初始化
	self.tbData = {};

	for k, v in ipairs(tbData) do
		self:push(v);
	end
end

function Heap:push( nValue )
	if not nValue then
		return;
	end
	if not self.tbData then
		self.tbData = {};
	end

	table.insert(self.tbData, nValue);
	print("nValue: ", nValue);
	self:print();

	-- 更新新节点
	local nIndex = #(self.tbData);
	local nFatherIndex = math.floor(nIndex/2);
	while nFatherIndex > 0 do
		print(string.format("[nIndex: %d] = %d, [nFatherIndex:%d] = %d", nIndex, self.tbData[nIndex], nFatherIndex, self.tbData[nFatherIndex]));
		if self.tbData[nIndex] < self.tbData[nFatherIndex] then
			self.tbData[nIndex], self.tbData[nFatherIndex] = self.tbData[nFatherIndex], self.tbData[nIndex];
		end
		
		nIndex = nFatherIndex;
		nFatherIndex = math.floor(nIndex/2);
	end
end

function Heap:pop(  )
	if not self.tbData then
		return nil;
	end

	local nValue = self.tbData[1];

	local nMaxIndex = #(self.tbData);
	-- 更新新节点
	local nLeaf = table.remove(self.tbData);
	self.tbData[1] = nLeaf;
	local nIndex = 1;
	while nIndex < nMaxIndex do
		local nLeftIndex = 2 * nIndex;
		local nRightIndex = 2 * nIndex + 1;

		-- 找最小的一个子节点(nLeftIndex < nRightIndex)
		local nSelectIndex = nLeftIndex;
		if nRightIndex < nMaxIndex then
			nSelectIndex = (self.tbData[nLeftIndex] > self.tbData[nRightIndex]) and nRightIndex or nLeftIndex;
		end

		if nSelectIndex < nMaxIndex and self.tbData[nIndex] > self.tbData[nSelectIndex] then
			self.tbData[nIndex], self.tbData[nSelectIndex] = self.tbData[nSelectIndex], self.tbData[nIndex];
		end
		
		nIndex = nSelectIndex;
	end

	return nValue;
end

-- test
local tbData = {1, 3, 5, 11, 4, 6, 7, 12, 15, 10, 9, 8};
-- table.sort( tbData )
-- Heap:build(tbData)
-- Heap:print();

-- push
-- Heap:push(2);
-- Heap:print();

-- pop
-- Heap:pop();
-- Heap:print();

------------------------------------------------------------
-- 求地图上两个坐标点表示的方向 2017-03-21 16:33:07

function GetRadian(X1, Y1, X2, Y2)
	if X1 == X2 and Y1 == Y2 then
		return -1;
	end

	local fXOffset = (X2 - X1);
	local fYOffset = (Y2 - Y1);
	local fDistance = math.sqrt(fXOffset * fXOffset + fYOffset * fYOffset);
	local fRadian = math.acos(fXOffset / fDistance);
	if fYOffset < 0.0 then
		fRadian = 2 * math.pi - fRadian;
	end

	print("Radian: ", fRadian)
	return fRadian;
end

function Round(value)
	if value > 0 then
		value = value + 0.5
	else
		value = value - 0.5
	end

	return math.floor(value);
end

local MAX_MAP_DIR = 256
function Radian2MapDir( fRadian)
	return Round(fRadian * MAX_MAP_DIR / (2.0 * math.pi));
end

function ClampMapDir(nDir)
	return (nDir % MAX_MAP_DIR + MAX_MAP_DIR) % MAX_MAP_DIR;
end

-- local fRadian = GetRadian(11716, 6250, 11840, 6176)
-- local nRet = Radian2MapDir(fRadian)
-- local nDir = ClampMapDir(nRet)
-- print("nDir = ", nDir)

------------------------------------------------------------
-- 追踪子弹分配问题 2017-03-23 10:02:38
-- x个NPC, NPC[x]
-- N个子弹, y[N]
-- 求y[i]分配哪几个NPC
-- eg:3个NPC，8颗子弹分配情况 {1, 1, 1, 2, 2, 2, 3, 3}
-- nPivot之前的多分配一个
function AssignAverage( nNpc, nMissile )
	local nAverage = math.floor(nMissile/nNpc);
	local nRemainder = nMissile % nNpc;
	local nPivot = nRemainder * (nAverage + 1)
	local tbAssign = {}
	for i = 1, nMissile do
		if i <= nPivot then
			tbAssign[i] = math.floor((i-1) / (nAverage + 1)) + 1
		else
			tbAssign[i] = math.floor((i-nPivot-1)/nAverage) + 1 + nRemainder
		end
	end
	return tbAssign;
end

-- eg:3个NPC，8颗子弹分配情况 {1, 2, 3, 1, 2, 3, 1, 2}
function AssignAverage1( nNpc, nMissile )
	local nAverage = math.floor(nMissile/nNpc);
	local nRemainder = nMissile % nNpc;
	local nPivot = nRemainder * (nAverage + 1)
	local tbAssign = {}
	for i = 1, nMissile do
		tbAssign[i] = (i-1) % nNpc + 1;
	end
	return tbAssign;
end

local tbAssign = AssignAverage(3, 8)
print(table.concat( tbAssign, ", "))
tbAssign = AssignAverage(8, 3)
print(table.concat( tbAssign, ", "))
tbAssign = AssignAverage1(3, 8)
print(table.concat( tbAssign, ", "))
tbAssign = AssignAverage1(8, 3)
print(table.concat( tbAssign, ", "))
------------------------------------------------------------
-- 计算加速度测试
-- 2017-06-21 20:34:08
local CurSpeed = {X=-485.14301, Y=-20.359489}
local Journey ={X=-148.54984, Y=-13.699066}
local Time = 0.15768620;
local DeltaTime = 0.26341701;
local AcclerSpeed = (2 * Journey.X - 2 * CurSpeed.X * Time)  / (Time * Time);
-- print("acc = ", AcclerSpeed)
local Velocity = CurSpeed.X + AcclerSpeed * DeltaTime
-- print("Velocity = ", Velocity)
-- 333AcclerSpeed: {X=-5795.2671 Y=-843.65021 Z=-1021.6241 ...}, {X=-485.14301 Y=-20.359489 Z=-243.45200 ...}, {X=-148.54984 Y=-13.699066 Z=-51.090332 ...}, 0.15768620, 0.26341701
-- 112 {X=232.89420 Y=505.34073 Z=2284.9568 ...}
-- 222 ---{X=-349.20822 Y=423.03687 Z=2370.1584 ...}, {X=232.89420 Y=505.34073 Z=2284.9568 ...}, {X=-198.62787 Y=431.45392 Z=2419.9751 ...}

