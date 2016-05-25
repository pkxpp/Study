--pre

--for n in pairs(_G) do print(n) end
--_G._G = _G在打印里面可以看到_G这个成员
--14.1 _G
--[[
function getfield(f)
	local v = _G
	for w in string.gfind(f, "[%w_]+") do
		v = v[w]
	end
	return v
end

function setfield(f, v)
	local t = _G
	for w, d in string.gfind(f, "([%w_]+)(.?)") do
		if d == "." then
			t[w] = t[w] or {}
			t = t[w]
		else
			t[w] = v
		end
	end
end

setfield("t.x.y", 10)
print(t.x.y, t.x)
print(getfield("t.x.y"), getfield("t.x"))
]]

--------------------------------------
--14.2
--[[
function decalre(name, initval)
	rawset(_G, name, initval or false)
end

setmetatable(_G, {
	__newindex = function (_, n)
		error("attempt to write to undeclared variable " .. n, 2)
	end,
	__index = function(_, n)
		error("attempt to read undeclared variable " ..n, 2)
	end
})
--a=1
decalre "a"
a = 1
]]

-------------------------------------------
--14.3
--[[
a=1
local tEnv = {}
setfenv(1, tEnv)
-- print(a)		-- error: attempt to call global 'print' (a nil value)
]]

-- 20160218
a = 1
local tEnv = {}
setmetatable(tEnv, {__index = _G})
a = 2
setfenv(1, tEnv)
-- print(a, _G)
for k, v in pairs(tEnv) do
	print(k, v)
end

------------------------------------------------------------
-- setenv
local env1 = {
	-- print = print,
}
function testSetfenv( )
	-- block
	if true then
		setfenv(1, env1)
		-- print("111111111")
	end
	-- print("22222")
end
testSetfenv()

local FuncEnv={}
-- setmetatable(FuncEnv, {__index = _G})
local func = loadfile("testSetfenv.lua")
print(111, func)
func()
--等价于setenv(func,FuncEnv);func();
local f1 = setfenv(func,FuncEnv)
f1()
print(222, f1, FuncEnv.test)
--FuncEnv就是新的模块啦，可以attempt to call field 'test' (a nil value)用其中的函数啦,其实，lua内部的model命令或者函数也是用的这个原理
-- FuncEnv.test()
print(FuncEnv.testvar)

--[[
sum:
1. 注释掉setmetatable一行之后，print(111, func)正常执行，因为当前环境还是_G;但是f1()这一句报错，因为他的话你就能够是FuncEnv，而FuncEnv当中是没有print函数的，所以执行print会报错
2. 不执行f1()而执行FuncEnv.test()还是报错，提示attempt to call field 'test' (a nil value)，这是为什么？f1()这一句需要运行一下就行了，说明已经在新的环境了，loadfile是不运行的
3. 从函数testSetfenv看，setfenv的范围是整个函数
]]

-- 2016/05/19
require "test_module"
-- testModule.testModule()
-- testModule()
-- print(package.loaded["test_module"])

local nCount = 0;
for k, v in pairs(package.loaded) do
	nCount = nCount + 1;
	print(nCount, k, v);
end

-- code1:
-- require "test_module"
-- testModule.testModule()

-- code2:
-- a = require "test_module"
-- a.testModule()

--[[
sum:
1. module和package.seeall是设置模块，并且该模块能够访问_G
2. module的第一个参数是模块名：
eg: 如code1所示就可以调用模块中的内容
3. module第一个参数如果是字符串"..."也可以，可以看到package.loaded中有一个...的表
eg: 只能通过这种方式调用：package.loaded["..."].testModule(),也可以通过code2的方式访问
4. module第一个参数如果是...也是可以的，可以通过code2种的方式访问，因为这个时候不知道包名
5. module第一个参数如果是nil, 错误：bad argument #1 to 'module' (string expected, got nil)
]]