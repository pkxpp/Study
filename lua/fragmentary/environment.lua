--pre

--for n in pairs(_G) do print(n) end
--_G._G = _G在打印里面可以看到_G这个成员
--14.1 _G

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

-- setfield("t.x.y", 10)
-- print(t.x.y, t.x)
-- print(getfield("t.x.y"), getfield("t.x"))

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
-- a=1
-- decalre "a"
-- a = 1
--]]
-------------------------------------------
--14.3
--[[
a=1
local tEnv = {}
setfenv(1, tEnv)
-- print(a)		-- error: attempt to call global 'print' (a nil value)
]]

-- 20160218
-- a = 1
-- local tEnv = {}
-- setmetatable(tEnv, {__index = _G})
-- a = 2
-- setfenv(1, tEnv)
-- print(a, _G)
-- for k, v in pairs(tEnv) do
	-- print(k, v)
-- end

------------------------------------------------------------
-- setenv
local env1 = {
	-- print = print,
}
function testSetfenv( )
	setfenv(1, env1)
	-- print("testSetfenv")	-- Error: testSetFenv函数所在的环境设置为env1，但是env1是没有print函数的
end
-- testSetfenv()

-- local MyModel={}
-- setmetatable(MyModel, {__index = _G})
-- local func = loadfile("testSetfenv.lua")
-- print(111, func)
-- func()
-- -- 等价于setenv(func,MyModel);func();
-- local fenv = setfenv(func, MyModel)
-- fenv() -- 新环境中没有print函数
-- print("test = ", MyModel.test)
-- MyModel就是新的模块啦，可以attempt to call field 'test' (a nil value)用其中的函数啦,其实，lua内部的model命令或者函数也是用的这个原理
-- print("var = ", MyModel.testvar)
-- MyModel.test()

-- @2020/06/17 setfenv loadstring返回的结果正确？
function testImport(  )
	local MyModel={}
	setmetatable(MyModel, {__index = _G})
	local func = loadfile("testSetfenv.lua")

	local nType = 2
	-- not setfenv
	if nType == 1 then
		func()
		print("1 test = ", test)	-- test是有的，因为这个时候，我没有设置setfenv，所以环境就是当前文件
	elseif nType == 2 then
		-- setfenv
		setfenv(func, MyModel)
		func()
		print("2 test = ", test)	-- test = nil
	elseif nType == 3 then
		local fenv = setfenv(func, MyModel)
		fenv()
		print("3 test = ", test)	-- test = nil
	end

end
testImport()
--[[
sum:
1. 注释掉setmetatable一行之后，print(111, func)正常执行，因为当前环境还是_G;但是f1()这一句报错，因为他的话你就能够是FuncEnv，而FuncEnv当中是没有print函数的，所以执行print会报错
2. 不执行f1()而执行FuncEnv.test()还是报错，提示attempt to call field 'test' (a nil value)，这是为什么？f1()这一句需要运行一下就行了，说明已经在新的环境了，loadfile是不运行的
3. 从函数testSetfenv看，setfenv的范围是整个函数
]]

-- 2016/05/19
-- require "test_module"
-- testModule.testModule()
-- testModule()
-- print(package.loaded["test_module"])

-- local nCount = 0;
-- for k, v in pairs(package.loaded) do
	-- nCount = nCount + 1;
	-- print(nCount, k, v);
-- end

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

-------------------------------
-- @2017/12/25 lua5.3 tail call 问题：因为tail call 用的是上一个函数的栈，所以我们用getfenv(2).ENV_SCRIPT_FILE得到的并不是当前函数的环境，加上写的函数会用到_ENV，所以报错[by 严总]
--[[问题：
(1)5.1的getfenv 尾调用有没有这个问题？
(2)针对5.3碰到的问题有没有更好的解决办法？
]]
-- 5.3问题，需要5.3环境
USE_VERSION_53 = false;
if USE_VERSION_53 then
function setfenv(fn, env)
	fn = (type(fn) == 'function' and fn or debug.getinfo(fn + 1, 'f').func)
	local i = 1
	while true do
		local name = debug.getupvalue(fn, i)
		if name == "_ENV" then
			debug.upvaluejoin(fn, i, (function()
				return env
			end), 1)
			break
		elseif not name then
			break
		end
		i = i + 1
	end
	return fn
end;

function getfenv(fn)
	fn = fn or 1;
	fn = (type(fn) == 'function' and fn or debug.getinfo(fn + 1, 'f').func)
	print("fn = ", fn)
	local i = 1
	while true do
		local name, val = debug.getupvalue(fn, i)
		if name == "_ENV" then
			print("i = ", i)
			return val
		elseif not name then
			break
		end
		i = i + 1
	end
end
end

-- a = -999
-- 问题1测试设计
local env0 = {
	a = 0;
}
local env1 = {
	a = 1;
}
local env2 = {
	a = 2;
}
local env3 = {
	a = 3;
}
setmetatable(env0, {__index = _G})
setmetatable(env1, {__index = _G})
setmetatable(env2, {__index = _G})
setmetatable(env3, {__index = _G})

function fGetEnv0()
if USE_VERSION_53 then
	_ENV = env0;
end
	local env =  fGetEnv();
	return env;
end

function fGetEnv()
if USE_VERSION_53 then
	_ENV = env1;
end
	local env = fGetEnv1();
	return env;
end

function fGetEnv1()
if USE_VERSION_53 then
	_ENV = env2;
end
	return fGetEnv2();
end

function fGetEnv2()
if USE_VERSION_53 then
	_ENV = env3;
end
	return getfenv(2);
end

if USE_VERSION_53 then
setfenv(fGetEnv0, env0)
setfenv(fGetEnv, env1)
setfenv(fGetEnv1, env2)
setfenv(fGetEnv2, env3)
end

-- local env = fGetEnv();
-- print("env = ", env);
-- print("env = ", env.a);

-- 测试设计2
function f1()
	local t = debug.getinfo(2);
	print(t, t.func, t.what)
	return f2()
end

function f2()
	local t = debug.getinfo(2);
	print(t, t.func, t.what)
	return f3()
	
end

function f3()
	local t = debug.getinfo(2);
	print(t, t.func, t.what)
	return 
end

-- print(getfenv(f1), debug.getinfo(2), getfenv(f2), _G, f1, f2)
print(f1, f2)
f1()


--[[
sum
(1)5.1下fGetEnv会报错，但是如果fGetEnv中直接调用fGetEnv2就正常，而且返回的环境也是正确的
(2)5.1下函数fGetEnv0也会报错，这就很奇怪~
(3)测试设计2中5.1版本获取的当前层级(参数为1)的结果是正确的，和f1，f2值是一致的；而上一层级(参数为2)就不一样了，而且不知道是什么~
(4)测试设计2中5.3版本就比较一致，当前层级(1)的结果是f1,f2两函数，而上一层级(2)的结果是一样的，按照严总说法就是理解为这个文件所对应的函数的环境_ENV
(5)按照测试设计2中的打印信息：
* 5.1中的每一个尾调用都会占用一个栈，eg:{func = nil, what = tail}，而正常调用是{func = function, what = Lua}, 文件内容当作函数是{func = function, what = main}
堆栈信息：
function: 006AC668	function: 006AC688
table: 006AC150	function: 006AC568	main
table: 005CF738	nil	tail
table: 005CF800	nil	tail
* 5.3中的每一个尾调用不会占用一个栈的，这就是最大的区别
堆栈信息：
function: 000000000034c540	function: 000000000034c570
table: 000000000034a590	function: 000000000034eb50	main
table: 000000000034a6d0	function: 000000000034eb50	main
table: 000000000034a790	function: 000000000034eb50	main
]]



------------------------------------------------------------
