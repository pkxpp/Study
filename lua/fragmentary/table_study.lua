--------------------------------------------------------
--1. #table 操作法问题 page@2013/12/23/15:03
--------------------------------------------------------
--eg

-- tbT1 = {
-- 	[2] = 2;
-- }
-- print("#tbT1", #tbT1)

-- print(6%1)

-- tbIPairsTest = {
-- 	["x"] = 1, 1200, 2100,["y"]=2, 1300,[2] = 2200,[5]=100,
-- }

-- local nIdx = 0;
-- for k, v in ipairs(tbIPairsTest) do
-- 	print(k, v)
-- end

-- for k, v in pairs(tbIPairsTest) do
-- 	print(k, v)
-- end

--[[
summary:
(1)iparis是按照索引来遍历的，如果index中断了，那么遍历结束
(2)没有索引的默认是按数组索引来的，如上面的1200,2100,字符串也是一样的
(3)后面指定索引的值会被覆盖掉，如上面的[2]=2200就没有了
]]
--[[
tbIPairs = {
	[1] = 2,
	[3] = 4,
}
for k, v in ipairs(tbIPairs) do
	print(k, v)
end
]]
--socket
--socket = require("socket")
--print(socket._VERSION)

--socket = require("socket.http")
--print(http.request("http://www.cs.princeton.edu/~diego/professional/luasocket"))

------------------------------
--19.1 size of array

-- local tbTest = {10, 2, nil; n=3}
-- for k, v in pairs(tbTest) do
-- 	print(k,v)
-- end
-- print(tbTest.n)
-- print(table.getn({10,2,4}))
-- print(table.getn({10,2,nil}))
-- print(table.getn({10,2,nil, n=3}))		--> 2不是书上讲的3
-- print(table.getn({n=1000}))

--page@2015/04/21
local t = {1,888,x= 999,b=2,5,nil,6,7,[10]=1,8,{z = 1,y = 1},nil,nil}
local t = {111, x = 222, nil, 333, [10] = 555, {}, nil, nil}

print(table.getn(t))
print(#t)
print(table.maxn(t))
-- print("list:")
-- for k, v in ipairs(t) do
-- 	print (k, v)
-- end
-- print("list2:")
-- for i=1, #t do
-- 	print (t[i])
-- end



-----------------------------
--insert 是不是可重入的
--[[
local tbData = {}
local n = 0;
co1 = coroutine.create(function()
	while(1) do
		n = n + 1;
		if n % 1000 == 0 then
			for k, v in ipairs(tbData) do
				print()
			end
		end
	end)

co2 = coroutine.create(function()
		local nRand = math.random(100)
		table.insert(tbData, nRand);
	end)
]]
--[[
local tbInsert = {}
table.insert(tbInsert, 1, 5)
table.insert(tbInsert, 3, 6)
for k, v in pairs(tbInsert) do
	print(k, v)
end
]] 
local tbTest = {}
tbTest[1] = {1, 3}
--tbTest[2] = {3, 5}
--tbTest[3] = {5, 4}
--tbTest[4] = {2, 3}
--[[
function cmp(a, b)
	return a[2] > b[2]
end

for k,v in ipairs(tbTest) do
	print(k,v[2])
end
table.sort(tbTest, cmp)
for k,v in ipairs(tbTest) do
	print(k,v[2])
end

for i=1,5 do
	print(i)
end
]]

--不定长参数
function test_arg(...)
	print(unpack(arg))
end
--test_arg(1,2)

--#table
local tbTest = {
	[5] = 1,
}
--print(#tbTest)

function GroupDetail(nArena, aPlayers, tbArena)
	local tbTemp = {};
	local tbGroup = {};
	local nNum = nArena;
	assert(nArena and aPlayers and tbArena);
	if not nArena or nArena <= 0 then
		return 0;
	end
	
	for i = 1, nArena do
		if not tbArena[i] then
			tbArena[i] = {};
		end
		tbTemp[i] = tbArena[i];	
	end
	
	local nCount = 1;
	for _, nId in pairs(aPlayers) do
		local nIndex = math.random(nNum);
		if nIndex <=0 or nIndex >= nArena then
			nIndex = 1;
		end
		
		table.insert(tbTemp[nIndex], nId);
		table.insert(tbGroup, tbTemp[nIndex]);
		table.remove(tbTemp, nIndex);
		nNum = nNum - 1;
		if #tbTemp == 0 or nNum == 0 then
			nNum = nArena;
			tbTemp = tbGroup;
			tbGroup = {};
		end
	end
end
local nArena = 3
local aPlayers = {
	[3] = {1,2},
	[7] = {3,4},
	[2] = {5,6},
}

local tbArena = {}
GroupDetail(nArena, aPlayers, tbArena)
--[[for k, v in pairs(tbArena) do
	for k1, v1 in pairs(v) do
		print(k1, v1)
	end
end
]]
local tbTemp = {}
tbTemp[1] = {}
tbTemp[2] = {}
-- for k, v in pairs(tbTemp) do
	-- print(k, v)
-- end
local tbGroup = {
	[3] = 'a',
	["b"] = 5,
}
table.insert(tbTemp[1], 5)
table.insert(tbGroup, tbTemp[1])
table.remove(tbTemp[1], 1)
--table.insert(tbTemp[1], 3)
-- table.insert(tbGroup, tbTemp[1])
--table.remove(tbTemp, 1)
-- print("before:")
-- for k, v in pairs(tbGroup) do
	-- print(k, v)
-- end
table.remove(tbGroup, 3)
tbGroup[3] = nil;
-- print("After...")
-- print("tbGroup:")
-- for k, v in pairs(tbGroup) do
	-- print(k, v)
-- end

--print("tbTemp")
for k, v in pairs(tbTemp) do
	--print(k, v)
end
--print(#tbTemp)
-- print("Test Remove ... ")
local tbRemove = {1, 2, 3}
--错误：循环中删除元素
-- for k, v in pairs(tbRemove) do
	-- table.remove(tbRemove, k)
-- end
--方法1：
-- for i=#tbRemove, 1, -1 do
	-- print("i = ", i)
	-- if i == 2 then
		-- table.remove(tbRemove, i)
	-- end
-- end

-- for k, v in pairs(tbRemove) do
	-- print (k, v)
-- end
--[[
sum:
1. table的remove函数只能以索引为参数remove，而且这个索引是在数组中的索引，不是在hash表中的数字，即ipairs能够遍历找到的索引
2. 对于hash表的remove，直接table[key] = nil就可以了
3. 【重要】table.remove的之后，table中剩下的元素会自动重新排序，但是如果这个时候使用ipairs操作这个table的时候，会漏掉被删除元素的下一个元素，因为他的索引会自动填到前面一个删掉元素的坑
]]
-------------------------------------------------
--table.sort 比较函数
local tbTest = {
}
local tbTest1 = {}
table.insert(tbTest, {nWin = 1, szName="1"})
table.insert(tbTest, {nWin = 2, szName="2"})
table.insert(tbTest1, {1, 18});
table.insert(tbTest1, {2, 56});
table.insert(tbTest1, {3, 37});
--print("Before Sort...")
for k, v in ipairs(tbTest) do
	--print(k, v.nWin, v.szName)
end
local comps = function (a, b) 
	return a.nWin > b.nWin 
end

local comps1 = function (a, b) 
	print("test...", a[2], b[2])
	return a[2] > b[2] 
end

function tbTest1:comps1(a, b)
	print("test1...", a[2], b[2])
	return a[2] > b[2] 
end

function tbTest1:testFunc(fFunc, a, b)
	print("a, b", a, b)
	fFunc(self, a, b)		--默认就是 tbTest1.comps1(self, a, b)
end

--tbTest1:testFunc(tbTest1.comps1, tbTest1[1], tbTest1[2])

--table.sort(tbTest, comps)
--table.sort(tbTest1, tbTest1.comps1)
--print("After Sort...")
for k, v in ipairs(tbTest1) do
	--print(k, v[1], v[2])
end

--[[
	tbSort = {
		{nIndex, nRand}
	}
]]

local tbSort = {}

local nNum = 3;
for i=1, nNum do
	local nRand = math.random(nNum*10)
	table.insert(tbSort, nRand);
end
-- print("Before Sort:")
-- for i=1, nNum do
	-- print(i, tbSort[i])
-- end

--全局函数
local fComp = function (a, b) 
	return a < b 
end

--面向对象
function tbSort.fComp(a, b)
	return a < b;
end

--面向对象隐藏self
function tbSort:fCompWrap(a, b)
	print("fCompWrap(self, a, b)", self, a, b)
	return a < b;
end

--table.sort(tbSort, fComp)
--table.sort(tbSort, tbSort.fComp)
-- table.sort(tbSort, tbSort.fCompWrap)

-- print("After Sort...")
for k, v in ipairs(tbSort) do
	-- print(k, v)
end
--[[
总结：
(1)table.sort的比较函数在面向对象编程时候要注意了
(2)lua面向对象中的self, table:func(param) <==>table.func(self, param)
(3)table:func作为参数传递
	*作为参数传递就是默认table:func(param)的形式, 即table.func(self, param)的形式调用，那么怎么样是table.func(param)的形式呢？
	*lua_call调用table的方法就是table:func(param)的形式
	*应该理解为定义function table:func(param)的时候就是定义function table.func(self, param)。也就表示函数是三个参数了
(4)为什么打印是fCompWrap	15	25	nil?
看源码，快速排序，交换第一个和最后一个，第一个参数是后面索引的，第二个是前面一个的索引
]]
----------------------------------------------
--[[
local tbTest2 = {2, 3 ,6, 5, 8, 4, 9, 7}
table.sort(tbTest2)
for k, v in ipairs(tbTest2) do
	print (v)
end
]]

--sort 2014/08/28
--[[
local tbT1 = {1, 5, 3}
local tbT2 = {5, 1, 3}
for i=1, 3 do 
	print(i, tbT1[i], tbT2[i])
end

table.sort(tbT1)
table.sort(tbT2)

for i=1, 3 do 
	print(i, tbT1[i], tbT2[i])
end

--]]
--nSort顺序，倒叙
function number2table(nNum, nSort)
	nSort = nSort or 0
	local tbRet = {}
	nNum = nNum or 0
	local nCount = 0;
	while nNum > 0 and nCount <= 5 do
		nCount = nCount + 1
		local nOne = math.mod(nNum, 10)
		table.insert(tbRet, nOne)
		nNum = math.floor(nNum/10)
		print(nCount, nOne, nNum)
	end
	
	if nSort == 0 then
	else
	end
	
	return tbRet;
end
--[[
tbRet = number2table(12)
print("Result:")
for _, v in pairs(tbRet) do
	print(v)
end
]]

--疑问：通过函数获取的一个table，能修改其中的值，但是赋值没有效果?
local tbConfig = {
	tbData = {1, 2, 3},
	n = 100,
	m = 1000,
}

function getData()
	return tbConfig.tbData;
end

function printConfig()
	for k, v in pairs(tbConfig) do
		print(tostring(k), tostring(v))
	end
end
-- printConfig()

local tbData = getData();
-- print(111, tostring(tbData))
tbData = {};
-- print(112, tostring(tbData))
-- printConfig();
--[[sum:
(1) table通过函数得到的引用是指这个table的地址，可以操作这个地址增加内容或者删除内容
(2) 但是不能改变这个地址本身，需要存放这个地址的变量才可以，在c语言里面一个指针指向一个地址，可以获取或者修改里面的内容，*p即可。但是如果希望这个指针指向另外一个地方就需要修改指向的内容p = another
(3) 反过来也是，如果一个指针变量p指向一个地方(比如我们这里的table), 如果p直接赋值p = another是指向另外一个指针，如果需要改变这个指针指向的地方的内容，需要使用*
]]

-- 2015、09、22
-- 数字和字符串数字作索引
local heroqualityconsume = {
		["1"] = {15001,10},
		["2"] = {15002,10},
		["3"] = {15001,10},
		["4"] = {15002,10},
		["5"] = {15001,10},
		["6"] = {15002,10},
		["7"] = {15001,10},
		["8"] = {15002,10},
		["9"] = {15001,10},
		["10"] = {15002,10},
		["11"] = {15001,10},
		["12"] = {15002,10},
		["13"] = {15001,10},
}
-- for k, v in pairs(heroqualityconsume) do
	-- print(k, v);
	-- print(type(k));
-- end

-- 2015/11/25 table遍历 有nil项
local tbDataWithNil = {
	[1] = 1,
	[2] = nil,
	[3] = 3,
}
-- for k,v in pairs(tbDataWithNil) do
	-- print(k, v);
-- end

-- 2016/01/06
-- 判断表是否为空
local t = {
	["xxx"] = 2,
}
-- print(111, next(t));


-- 2016/02/18
-- 实现pairs和ipairs
function selfIpairs(t)
	local i = 0;
	return function()
		i = i + 1;
		return t[i];
	end
end

function selfPairs(t)
	local index = nil;
	return function()
		local v = next(t, index);
		index = v;
		return v;
	end
end

-- test
local tbIpairs = {1, 3, 5, 2, 4, 6, 8, 9, 10}
local tbPairs = {1, a = 2, b = 3}
for k, v in selfIpairs(tbIpairs) do
	-- print(k, v)
end

-- print(next(tbPairs, nil))
for k, v in selfPairs(tbPairs) do
	-- print(k, v)
end

-- 参考实现
--* http://www.jellythink.com/archives/506 *
local function iter(a, i)
	-- print("control val: ", i)
	local i = i + 1;
	local v = a[i];
	if v then
		return i, v;
	end
end

local function iter1(a, i)
	local k, v = next(a, i);
	-- if v then
		-- i = k;
		return k, v;
	-- end
end

function ipairs1(t)
	return iter, t, 0;
end

function pairs1(t)
	return iter1, t, nil;
end

function pairs2(t)
	local index = nil
	return function ()
		index = next(t, index)
		return index, t[index]
	end
end

for k, v in ipairs1(tbIpairs) do
	-- print(k, v);
end

for k, v in pairs1(tbPairs) do
	-- print( k, v);
end

------------------------------------------------------------
-- 2016/04/18 __index只有在key为nil的时候才调用（by 1-）
local s = {
	[1] = 1,
	a = 'b',
}
s.__index = function (t, i)
	print("call __index ...")
	return t[i]
end

function testIndex()
	for k,v in pairs(s) do
		print(k,v)
	end

	print("call nil key: ", s[2])
end
-- testIndex( )
--[[
sum:
]]

local tNormal = {
	[1] = 1,
	a = 'a',
	__index = function ()
		print("__index 1.")
	end
}

local tMetatable = {
	[2] = 2,
	b = 'b',
	__index = function ()
		print("__index 2.")
	end
}

local tMetatable2 = {
	[3] = 3,
	c = 'c',
}

local tTest = {
	__index = function ()
		print("__index 3.")
	end
}

local tTest2 = {
	__index = function ()
		print("__index 4.")
	end
}

setmetatable(tTest, tMetatable)
setmetatable(tTest, tMetatable)
print(tNormal[3])
print(tTest[3])
print(tTest2[3])

--[[
sum:
1. __index被执行的条件：Note that the metamethod is tried only when key is not present in table.[参考5.2 Reference Manual]
2. 一个table的__index单独拿出来是没有意义的，只有这个table作为元表的时候才有意义，也就是才有执行的可能性。
eg: 例如tNormal这个表在执行tNormal[3]的时候不会访问自己的__index方法
3. metatable必须有__index方法，不然也是没有意义的。
eg: tMetatable2没有__index方法，结果tTest2在访问tTest2[3]的时候还是nil
4. 可以看源码
/********************************************
  function gettable_event (table, key)
       local h
       if type(table) == "table" then
         local v = rawget(table, key)
         -- if key is present, return raw value
         if v ~= nil then return v end
         h = metatable(table).__index
         if h == nil then return nil end
       else
         h = metatable(table).__index
         if h == nil then
           error(···)
         end
       end
       if type(h) == "function" then
         return (h(table, key))     -- call the handler
       else return h[key]           -- or repeat operation on it
       end
     end
*********************************************/
]]

-- 2016/06/01 六一儿童节
-- table contact
function fnUnPack( ... )
	local p = {...};
	return unpack(p);
end

function fnUnPack1( ... )
	-- local p = table.pack(...);
	-- return table.unpack(p, 1, p.n);
end

function fnUnPack2( ... )
	local p = {...}
	local n = select('#', ...)
	return unpack(p, 1, n);
end

local tbTestLength = {1, 2, 3, nil ,4};
local tbTestLength1 = {1, 2, 3, nil ,4, nil};
local tbTestLength2 = {1, 2, 3, nil ,nil, 4};
local tbTestLength3 = {nil, 2, 3, nil, 4};
local tbTestLength4 = {nil, 2, 3, nil, nil, nil, 4, nil};

print(fnUnPack(1, 2, 3));
print(fnUnPack(1, 2, 3, nil ,4));
print(fnUnPack(1, 2, 3, nil ,4, nil));
print("Length = ", #tbTestLength, #tbTestLength1, #tbTestLength2, #tbTestLength3, #tbTestLength4)

print(fnUnPack2(1, 2, 3));
print(fnUnPack2(1, 2, 3, nil ,4));
print(fnUnPack2(1, 2, 3, nil ,4, nil));

print("select: ", select(3, 5, 6, 7, 8))
print("select: ", select('#', 5, 6, 7, 8))