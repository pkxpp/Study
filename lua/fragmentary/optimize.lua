-----------------------------------------------------------------------------
--脚本文件：optimize.lua
--功能设计：
--功能开发：page(pk)
--开发时间：2022/06/27 14:26:58
--脚本功能：优化相关
--修改记录：
-----------------------------------------------------------------------------
local t = {
		{1, 100},
		{2, 200},
		{3, 300},
		{4, 400},
		{5, 500},
	};

function TestPairs()
	local nTestTime = 100000;
	local n = 0;
	local tStart = os.clock();
	for i =1, nTestTime do
		for k, v in pairs(t) do
			n = v;
		end
	end
	local tEnd = os.clock();
	print("CostTime: ", tEnd - tStart)

	tStart = os.clock();
	for i =1, nTestTime do
		for k, v in ipairs(t) do
			n = v;
		end
	end
	tEnd = os.clock();
	print("CostTime: ", tEnd - tStart)
end
-- TestPairs();


table.unpack = function(table, ...)
    -- if not table or type(table) ~= "table" then
    --     return nil;
    -- end
    return unpack(table, ...);
end

function TestUnpack()
	local nTestTime = 1000000;
	local tStart = os.clock();
	local sin = math.sin
	for i = 1, nTestTime do
		local x = sin(i)
	end
	local tEnd = os.clock();
	-- print("CostTime: ", tEnd - tStart)

	-- tStart = os.clock();
	-- for i = 1, nTestTime do
	-- 	local x = math.sin(i)
	-- end
	-- tEnd = os.clock();
	-- print("CostTime: ", tEnd - tStart)


	tStart = os.clock();
	for i = 1, nTestTime do
		local x = table.unpack(t[1]);
	end
	tEnd = os.clock();
	print("CostTime1: ", tEnd - tStart)

	tStart = os.clock();
	for i = 1, nTestTime do
		local x = unpack(t[1]);
	end
	tEnd = os.clock();
	print("CostTime2: ", tEnd - tStart)

	tStart = os.clock();
	local fnUnpack = table.unpack;
	for i = 1, nTestTime do
		local x = fnUnpack(t[1]);
	end
	tEnd = os.clock();
	print("CostTime3: ", tEnd - tStart)

	tStart = os.clock();
	for i = 1, nTestTime do
		local x = t[1][1];
	end
	tEnd = os.clock();
	print("CostTime4: ", tEnd - tStart)
end

-- TestUnpack();

local LOptimize = {
	m_Attribute = {
		[1] = 100,
		[2] = 200,
	}
};

function LOptimize:_TestGetAttribute(nType)
	return self.m_Attribute[nType] or 0;
end

function LOptimize:TestLocalFunction()
	local nTestTime = 1000000;
	local tStart = os.clock();
	for i = 1, nTestTime do
		local x = self:_TestGetAttribute(1);
		local y = self:_TestGetAttribute(2);
	end
	local tEnd = os.clock();
	print("CostTime1: ", tEnd - tStart)

	-- tStart = os.clock();
	-- for i = 1, nTestTime do
	-- 	local x = math.sin(i)
	-- end
	-- tEnd = os.clock();
	-- print("CostTime: ", tEnd - tStart)


	tStart = os.clock();
	local fnGetAttribute = self._TestGetAttribute;
	for i = 1, nTestTime do
		local x = fnGetAttribute(self, 1);
		local y = fnGetAttribute(self, 2);
	end
	tEnd = os.clock();
	print("CostTime2: ", tEnd - tStart)

	tStart = os.clock();
	for i = 1, nTestTime do
		local x = self.m_Attribute[1] or 0;
		local y = self.m_Attribute[2] or 0;
	end
	tEnd = os.clock();
	print("CostTime2: ", tEnd - tStart)
end

LOptimize:TestLocalFunction();