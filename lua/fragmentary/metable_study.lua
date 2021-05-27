--
--pk
--2013/06/27
--

-- 两种写法都可以^_^
local t1 = {20, 1, key1 = "hello", key2 = "world", lang = "lua"}
local t2 = {
	key1 = "hello new",
	key2 = "world new",
}
print("Before setmetatable: ")
print("a2 metable = ", getmetatable(t2))
print("language =", t2["lang"])

-- 使用用setmetatable设置元表
setmetatable(t2, {__index = t1})
print("\nAfter setmetatable:")
print("a2 metable =", getmetatable(t2))
print ("language =", t2["lang"])


--@function: 复制一个table
--@tbOrg: 源table
function copy(tbOrg)
	-- 注意：放置互引用情况下的死循环
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
            tbNewTable[_copy(index)] = _copy(value);	
        end
		
		-- 使用：保持元表一致
        return setmetatable(tbNewTable, getmetatable(object));
    end
    return _copy(tbOrg);
end

--20131107
--gf_Inherit函数会不会覆盖
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

tbSrc = {
	[1] = 5,
	[2] = 7,
	src1 = 1,
	src2 = 2,
	tbSrc1 = {2, 3}
}

tbSrc2 = {
	[1] = 6,
	src3 = 3,
	9,
}

function TestInherit(base, tbInitInfo)
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
end

local tbTest = gf_Inherit(tbSrc, tbSrc2)
-- print("..........")
-- for i, v in pairs(tbSrc) do
	-- print(i, v)
-- end
-- print("..........")
-- for i, v in pairs(tbSrc2) do
	-- print(i, v)
-- end
-- print("..........")
-- for i, v in pairs(tbTest) do
	-- print(i, v)
-- end
-- print(tbTest[1], tbTest[2], tbTest.src1, tbTest.src2)

--------------------------------------------------------
--(1.1)__index
-- print(1.1)
Window = {}
Window.prototype = {x = 0, y = 0, width = 100, height = 100, }
Window.mt = {}
function Window.new(o)
	setmetatable(o, Window.mt)
	return o;
end

Window.mt.__index = function ( table, key)
	return Window.prototype[key]
end

w = Window.new{x=10, y = 20}
-- print(w.width)

--(1.2) __newindex
--[[
function setDefault(t, d)
	local mt = {__index = function() return d end}
	setmetatable(t, mt)
end

local mt = {__index = function (t) return t.___ end}
function setDefault(t, d)
	t.___ = d
	setmetatable(t, mt)
end
]]

local key = {}
local mt = {__index = function (t) return t[key] end}
function setDefault(t, d)
	t[key] = d
	setmetatable(t, mt)
end

tab = {x = 10, y = 20}
-- print(tab.x, tab.z)

setDefault(tab, 0)
-- print(tab.x, tab.z)

tab1 = {x = 15, y = 25}
setDefault(tab1, 1)
-- print(tab.x, tab.z, tab1.x, tab1.z)
--[[
注：这里直接用table当做key，可以理解为key的table实质上就是一个地址
(eg:0x0000ABCD)
所以可以当做key，而且也是唯一的
但是仔细想一下table的结构，略觉得不妥，毕竟还是消耗table的存储结构大小
]]

--（1.3）监控表
--其实就是在索引和值之间插入一道程序，实现使用index和newindex实现

t = {}

--[[local _t = t
t = {}
local mt = {
	__index = function(t, k)
	print("*access to element " .. tostring(k))
	return _t[k]
	end,
	
	__newindex = function(t, k, v)
	print("*update of element " .. tostring(k) ..
		" to " .. tostring(v))
	_t[k] = v
	end

}
setmetatable(t, mt)]]

--私有key
local index = {} --用table做key，同1.2中的【注】
local mt = {
	__index = function(t, k)
	print("*access to element " .. tostring(k))
	return t[index][k]
	end,
	
	__newindex = function(t, k, v)
	print("*update of element " .. tostring(k) ..
		" to " .. tostring(v))
	t[index][k] = v
	end
}	
function track(t)
	local proxy = {} --每调用一次track，都会生成一个新的proxy
	proxy[index] = t
	setmetatable(proxy, mt)
	return proxy
end

print("1.3 .................................... start.")
t = track(t)
t[2] = 'hello'
print(t[2])
print(t[1])

-- t1 = track(t)
-- print(t1[3])
--[[
^_^
这里打印了两次
*access to element 3
*access to element 3
因为这里用到的t是上面一个track过的，所以t1是track过两次的，哈哈
^_^
]]
--print(t[2])
print("1.3 ................................ end.")
------------
--(1.4)只读表
function readOnly(t)
	local proxy = {}
	local mt = {
		__index = t,
		__newindex = function (t, k, v)
			error("attempt to update a read-only table", 2)
		end
	}
	
	setmetatable(proxy, mt)
	return proxy;
end

days = readOnly{"Sunday", "Monday", "Tuesday", "Wednesday",
	"Thursday", "Friday", "Saturday"
}

-- for k, v in pairs(days) do
	-- print("metatable", k, v);
-- end

-- print(days[1])
-- days[2] = "Noday"
-- print(days[2])

--sum:
--(1)元表的内容pairs读取不到
--(2)直接__newindex是没有效果的


----------------------------------
--2015、07、07 弱引用
--问题：道具多处被使用，如果背包中道具被删除需要删除该处道具对象
-- print("Test 弱引用 ...................................")
local tbItem1 = {1, 3};
local tbItem2 = {2, 2};
local tbItem3 = {3, 5};
local tbItems = {
	[1] = tbItem1,
	[2] = tbItem2,
	[3] = tbItem3,
}
local tbWeak = {
	[1] = tbItems[1]
}

setmetatable(tbWeak, {__mode = "v"});
-- print("before:");
-- for k, v in pairs(tbWeak) do
	-- print(k, v)
-- end
tbItem1 = nil;
tbItems[1] = nil;
collectgarbage();
-- print("after:");
-- for k, v in pairs(tbWeak) do
	-- print(k, v)
-- end
-- print("...........................end..............................")

-- 2015、12、30
-- __index和metatable
local a = {}
local b = {
	[1] = 3,
	[5] = 5,
}
local c,d = 5, 6;
local e,f = "str1", "str2"
setmetatable(a, b)
print(a[1])
print("getmetatable：", getmetatable(a), b)
print(getmetatable(c), getmetatable(d), getmetatable(e), getmetatable(f))

------------------------------------------------------------
-- meta method 2018-10-19 16:01:37  jit email list
local Class = {}

function Class:__tostring()
    print("1")
    return "3"
end

local t = setmetatable({}, Class)

print("2\n", tostring(t)) -- Produces proper output
print()
print("2\n", t) 

-- strang result
--[[
1
2
	3

2
1
	3
]]

