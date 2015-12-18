print("环境：(1, 0)", getfenv(1), getfenv(0), getfenv(3))
--math.random
--(1)no pragram
function random_0()
	print(math.random())
end

--(2) 1 pragram
function random_1(nMax)
	local nMax = nMax
	if not nMax or nMax == 0 or nMax < 0 then
		print(math.random(nMax))
	end
	local tbRandom = {}
	for i = 1, nMax do
		tbRandom[i] = 0
	end
	
	for i = 1, 10 do 
		local nRandom = math.random(nMax)
		tbRandom[nRandom] = tbRandom[nRandom] + 1
	end
	
	for i = 1, nMax do
		print(i, tbRandom[i])
	end
	
end

-- print(math.random(1))

--random_1()
for i=1, 10 do
--print(math.random(0,5))
end
--random_1(5)
--[[
summary:
(1)没有参数，范围是[0,1)之间的一个小数
(2)有一个参数，即最大值n(n>=1)，范围是[1,n]
(3)两个参数，即最小值nMin和最大值nMax(nMax>=nMin)，范围是[nMin,nMax]
--]]


print(math.random(8))
--同一个数，每次random的值都一样
-----------------------------

--2014/10/08 随机数生成器

--Lib库的随机函数,只支持正整数随机 by.sunduoliang
--该函数存档一些bug，因为lua语言%的算法问题，会导致有些随机数永远都不会随到。慎用
Lib = {}
-- print(-5%10)
-- print(math.mod(-5, 10))
Lib.nRandomSeed = -10;
function Lib:random(nBegin, nEnd)
	local nTime = os.clock();
	self.nRandomSeed = math.floor(self.nRandomSeed or nTime);
	--print(110, self.nRandomSeed)
	self.nRandomSeed = (self.nRandomSeed * 3877 + 29573) % (4294967296);
	--self.nRandomSeed = math.mod(self.nRandomSeed * 3877 + 29573, (0xFFFFFFFF+1));
	print("nRandomSeed", self.nRandomSeed, nBegin + math.mod(self.nRandomSeed, (nEnd - nBegin + 1)))
	nBegin = nBegin or 0
	nEnd = nEnd or 65535
	if nEnd < nBegin then
		nBegin, nEnd  = nEnd, nBegin;
	end
	return nBegin + self.nRandomSeed % (nEnd - nBegin + 1)
	--return nBegin + math.mod(self.nRandomSeed, (nEnd - nBegin + 1))
end

local tbRand = {}
local tbRand1 = {}
local tbCounter = {}
g_nMaxNum = 100
g_nMaxCircle = 1000

for i=1, g_nMaxNum do
	tbCounter[i] = 0;
end

function Lib:Test()
	for i=1, g_nMaxCircle do
		--[[
		local n = Lib:random(1, g_nMaxNum)
		if i <= 10 then	
			table.insert(tbRand, n)
		end
		if i > 10 then
			for _, v in ipairs(tbRand) do
				if 
			end
		end]]
		--统计
		
		local n = Lib:random(1, g_nMaxNum)
		--print(n)
		
		tbCounter[n] = tbCounter[n] + 1
	end
end

local nCount = 0;
for i=1, g_nMaxNum do
	if tbCounter[i] == 0 then
		--print(i, tbCounter[i])
		nCount = nCount + 1;
	end
end

-- print("nCount = ", nCount);
-- print((745991512 * 3877 + 29573)%0xffffffff, (745991512 * 3877 + 29573)%(0xffffffff+1))
-- print(0xFFFFFFFF, 0xffffffff+1, 0x100000000)
--[[745991512 * 3877 + 29573 = 2892209121597 (ok)
4294967295 == 0xffffffff
--(745991512 * 3877 + 29573)%0xffffffff = 
--(1)lua结果：1696132062
--(2)calc结果：1696132062
--(3)c++程序结果：1696131389 = (2892209121597)%(0xffffffff+1)
]]
