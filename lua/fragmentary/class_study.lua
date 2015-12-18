--

--16.1 class
--[[
Account = {
	balance = 0,
	withdraw = function (self, v)
		self.balance = self.balance - v
	end
}

function Account:deposit(v)
	self.balance = self.balance + v
end

Account.deposit(Account, 200.00)
print(Account.balance)
Account:withdraw(100.00)
print(Account.balance)
]]
--[[
书上例子：有两个对象a和b，想让b作为a的prototype只需要：
setmetatable(a, {__index = b})
注：设置元表为{__index = b}，元表的元方法为__index = b
]]
--[[
function Account:new(o)
	o = o or {}
	setmetatable(o, self)
	--self.__index = self
	return o;
end

a = Account:new{balance = 0}
print(a.balance, a.deposit)
Account.deposit(a, 100.00)
print(a.balance)
a:deposit(100.00)

print(Account.balance)]]
--[[
注：
(1)如果self.__index = self这一句注释掉了之后，执行到a:deposit(100.00)会报错，如下：
lua: class_study.lua:34: attempt to call method 'deposit' (a nil value)
stack traceback:
	class_study.lua:34: in main chunk
	[C]: ?	
(2)如果没有上面一句的话
Account是a的元表，a中有balance，但是没有deposit
print(a.balance, a.deposit) ==> 0 nil
]]

------------------------------
--16.5 single-method
function newObject(value)
	return function(action, v)
		if action == "get" then return value
		elseif action == "set" then value = v
		else error("invalid action")
		end
	end
end
d = newObject(0)
print(d("get"))
d("set", 10)
c = newObject(1)
print(d("get"), c("get"))

--class study
--继承
--参数tbInitInfo是为了对基类的成员进行初始化或为派生类中增加新成员
function gf_Inherit(base,tbInitInfo)	--定义一个继承函数
	local derive = {};
	local metatable = {};
	metatable.__index = base;
	setmetatable(derive,metatable);
	for i,v in pairs(base) do
		if type(v) == "table" then
			derive[i] = gf_CopyTable(v);
		end;
	end;
	if tbInitInfo then	--如果要改变基类的成员或增加新的成员
		for i,v in pairs(tbInitInfo) do
			derive[i] = v;
		end;
	end;
	return derive;
end;

--复制一个table
--org为源table，des为复制出来的新table
function gf_CopyTable(tbOrg)
    local tbSaveExitTable = {}
    local function _copy(object)
        if type(object) ~= "table" then
            return object;
        elseif tbSaveExitTable[object] then	--检查是否有循环嵌套的table
            return tbSaveExitTable[object];
        end
        local tbNewTable = {};
        tbSaveExitTable[object] = tbNewTable;
        for index, value in pairs(object) do
            tbNewTable[_copy(index)] = _copy(value);	--要考虑用table作索引的情况
        end
        return setmetatable(tbNewTable, getmetatable(object));
    end
    return _copy(tbOrg);
end

--[[
1. 回忆
（1）metatable：
*设置某个table的扩展表，类似于c++中类继承关系，metatable本质上也是一个table，被设置的talbe先在自己的table查找某个元素，如果没有找到的话就去元表中去查找
*getRaw接口是取不了元表中的内容的
（2）__index：
*就是取元表数据的方法，他是一个function,简单的理解可以是
table.__index = function(key)
	if talbe[key] then
		return table[key]
	end
	return metatable[key]
end
* 使用的时候操作符'.'就会调用__index
]]
-------------------------------------
--参考：月影
-- gf_CopyTable, gf_Inherit
class_base = {}
function class_base:create(tbArg)
	local tbNew = gf_Inherit(self, tbArg)
	return tbNew;
end 
-------------------------------------
--参考：云风
local _class={}
 
function class(super)
	local class_type={}
	class_type.ctor=false
	class_type.super=super
	class_type.new=function(...) 
			print("new", ...)
			local obj={}
			do
				local create
				create = function(c,...)
					if c.super then
						create(c.super,...)
					end
					if c.ctor then
						c.ctor(obj,...)
					end
				end
 
				create(class_type,...)
			end
			setmetatable(obj,{ __index=_class[class_type] })
			return obj
		end
	local vtbl={}
	_class[class_type]=vtbl
 
	setmetatable(class_type,{__newindex=
		function(t,k,v)
			vtbl[k]=v
		end
	})
 
	if super then
		setmetatable(vtbl,{__index=
			function(t,k)
				local ret=_class[super][k]
				vtbl[k]=ret
				return ret
			end
		})
	end
 
	return class_type
end
--use
base_type = class()
base_type.ty = {"base", 313}
function base_type:ctor(x)
	print("base_type ctor")
	self.x = x;
end

function base_type:print_x()
	print(self.x)
end

function base_type:hello()
	print("hello base_type");
end
base_type.y = "a"
print("y, hello", base_type.y, getmetatable(base_type))
--
test = class(base_type)
test.y = "b"
test.tz = {"test", "314"}
print("y, metatable", test.y, getmetatable(test))
function test:ctor()
	print("test ctor")
end

function test:hello()
	print("hello test")
end

--base_type:print_x();
--test:print_x()

a = test.new(1)
print("address:", base_type, test, a)
b = test.new(2)
b.ty = {"test", "315"}
print("print_x", a.print_x, b.print_x, a.tz, b.tz, a.ty, b.ty)
a:print_x();
a:hello()
function a:print_y()
	print("y")
end
a:print_y()
--a:new(2)
print("_class")
print(base_type, test, a)
for k, v in pairs(_class) do
	print(k, v, getmetatable(v))
	if type(v) == "table" then
		for k1, v1 in pairs(v) do
			print("\t", k1, v1)
		end
	end
end

function tst_print_lua_table (lua_table, indent)
	local getTableName = function(tb)
			local tbName = "tbName"
			for k,v in pairs(_G) do
				if v == tb then
					tbName = k
					break;
				end
			end
			return tbName;
		end
	--check
	if type(lua_table) ~= "table" then
		print("It's not a table:", lua_table)
		return;
	end
	
    indent = indent or 1
	if indent == 1 then
		print(getTableName(lua_table) .. " = {")
	end
	
	for k, v in pairs(lua_table) do
		if type(k) == "string" then
			k = string.format("%q", k)
		end
		local szSuffix = ""
		if type(v) == "table" then
			szSuffix = "{"
		end
		local szPrefix = string.rep("    ", indent)
		formatting = szPrefix.."["..k.."]".." = "..szSuffix
		if type(v) == "table" then
			print(formatting)
			tst_print_lua_table(v, indent + 1)
			print(szPrefix.."},")
		else
			local szValue = ""
			if type(v) == "string" then
				szValue = string.format("%q", v)
			else
				szValue = tostring(v)
			end
			print(formatting..szValue..",")
		end
	end
	if indent == 1 then
		print("}")
	end
end

tst_print_lua_table(base_type)
print()
tst_print_lua_table(test)
print()
tst_print_lua_table(a)
--练习
--(1)
function copytable(tbArg)
	local tbNew = {}
	local tbCollsion = {}
	copy = function (obj)
		if type(obj) ~= "table" then
			return obj;
		end
		if tbCollsion[obj] then
			return tbCollsion[obj];
		end
		tbCollsion[obj] = obj;
		print("copy", copy)
		return copy(obj)
	end
	
	for k, v in pairs(tbArg) do
		tbNew[k] = copy(v)
	end
	return tbNew;
end
--哈哈还是写错了！！！
function copytable1(tbArg)
	local tbCollsion = {}
	
	--local copy = function(obj)
	--上面这一句会出现问题！！！没有复制成功！
	--local function copy(obj)这样也是没有问题
	local copy = function (obj)
		if type(obj) ~= "table" then
			return obj;
		end
		if tbCollsion[obj] then
			return tbCollsion[obj];
		end
		local tbNew = {}
		print("obj, tbNew:", obj, tbNew)
		tbCollsion[obj] = tbNew;
		print("copy", copy)
		for k, v in pairs(obj) do
			tbNew[k] = copy(v);
		end
		--这个也要注意：复制完之后，元表关系也要保留下来
		return setmetatable(tbNew, getmetatable(obj))
	end
	print("collsion:")
	tst_print_lua_table(tbCollsion)
	return copy(tbArg);
end

function inherit(tbBase, tbClass)
	--月影那里是用的元表+复制，其实没必要，不复制base的非table成员(__index方式)和直接复制没有区别，因为__index的方法不会改变base表里面的数据
	local tbNew = copytable1(tbBase)
	
	--这里用元表的方法比月影里面的gf_Inherit方法要好，因为结果是一样的！
	local tm = {}
	tm.__index = tbClass;
	setmetatable(tbNew, tm)
	-- or
	--setmetatable(tbNew, {__index = tbBase})
	-- or end
	
	--使用上面元表的方法，这里根本不需要！
	--here needn't think about tbClass is a table: both share table, be careful!
	--for k, v in pairs(tbClass) do
	--	tbNew[k] = v;
	--end
	return tbNew;
end

-- tbTestBase1 = {"a", 9}
-- tbTestBase11 = {
	-- "a11", 10,
	-- tb11 = tbTestBase11,
-- }
-- tbTestBase1.tb1 = tbTestBase11;

-- local tbSubTestBase1 = inherit(tbTestBase1, {x = 314})
-- tst_print_lua_table(tbSubTestBase1)
-- print("x", tbSubTestBase1[1], tbSubTestBase1.x)

--嵌套table
local tbCircle1 = {
	[1] = 2,
	[2] = tbCircle2,
}

local tbCircle2 = {
	[1] = 2,
	[2] = tbCircle1,
}
tbCircle1[2] = tbCircle2

-- local tbCircle3 = copytable1(tbCircle2)

-- print("tbCircle1", tbCircle1)
-- for k, v in pairs(tbCircle1) do
	-- print(k, v)
	-- if type(v) == "table" then
		-- for k1, v1 in pairs(v) do
			-- print("\t", k1, v1)
		-- end
	-- end
-- end

-- print("tbCircle2", tbCircle2)
-- for k, v in pairs(tbCircle2) do
	-- print(k, v)
	-- if type(v) == "table" then
		-- for k1, v1 in pairs(v) do
			-- print("\t", k1, v1)
		-- end
	-- end
-- end

-- print("tbCircle3", tbCircle3)
-- for k, v in pairs(tbCircle3) do
	-- print(k, v)
	-- if type(v) == "table" then
		-- for k1, v1 in pairs(v) do
			-- print("\t", k1, v1)
		-- end
	-- end
-- end

--[[
record:
1. {x = 314}没有打印出来
2. 嵌套table的时候，copy()函数报错:定义的时候加了一个local, local copy = ，那么local的含义是什么？
* 
3. 嵌套关系有没有改变？
* 
4. 
]]
--(2)
local _G_class = {}
function class(super)
	local tbBase_type = {}
	tbBase_type.ctor = nil
	tbBase_type.super = super;
	tbBase_type.new = function(...)
		local tbNew = {}
		--local的问题，下面的方法要改变
		-- local create = function(c, ...)
			-- if c.super then
				-- create(c.super, ...)
			-- end
			-- c.ctor()
		-- end
		do
			local create
			create = function(c, ...)
				if c.super then
					create(c.super, ...)
				end
				if c.ctor then
					c.ctor(obj, ...)
				end
			end
			create(tbBase_type, ...)
		end
		--setmetatable(tbNew, tbBase_type)
		setmetatable(tbNew, {__index = _G_class[tbBase_type]})
		return tbNew;
	end
	
	local tbVtl = {}
	_G_class[tbBase_type] = tbVtl;
	setmetatable(tbBase_type, {__newindex = 
		function(t, k, v)
			tbVtl[k] = v;
		end
	})
	
	if super then
		setmetatable(tbVtl, {__index =
			function (t, k)
				local ret = _G_class[super][k]
				--这一句为什么要定义？对象自己可以添加新成员~~~
				tbVtl[k] = ret
				return ret;
			end
		})
		--下面还是写错了
		-- tbBase_type.__index = function(t, k)
			-- return _G_class[super][k];
		-- end
	end
	
	return tbBase_type;
end

-------------------------
-- class中成员变量和函数有区别么？
C_Base = class()
C_Base.z = 2;
function C_Base.ctor()
	self.m = 1;
end

function C_Base:A()
	print("333")
end
print("22222222222222222222")
local a = C_Base.new();
for k, v in pairs(a) do
	print(k, v);
	a:A();
end

