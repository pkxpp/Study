

-------------------------------------------------------------
--lua面向对象：简单粗暴的实现
function copytable(tbArg)
	local tbCollsion = {}
	
	--local的作用域问题
	local copy	
	copy = function (obj)
		if type(obj) ~= "table" then
			return obj;
		end
		--检查是否有嵌套循环的table
		if tbCollsion[obj] then
			return tbCollsion[obj];
		end
		
		local tbNew = {}
		tbCollsion[obj] = tbNew;
		--递归复制
		for k, v in pairs(obj) do
			tbNew[k] = copy(v);
		end
		--复制完之后，元表关系也要保留下来
		return setmetatable(tbNew, getmetatable(obj))
	end

	return copy(tbArg);
end

function inherit(tbBase, tbClass)
	--复制基类
	local tbNew = copytable(tbBase)
	
	
	local tm = {}
	tm.__index = tbClass;
	setmetatable(tbNew, tm)
	
	--修改tbBase为tbClass中的成员或者新增加tbClass中的成员
	if tbClass then
		for k, v in pairs(tbClass) do
			tbNew[k] = v
		end
	end

	return tbNew;
end
--使用
local tbObject = {}
local tbFruit = inherit(tbObject)
local tbApple = inherit(tbFruit)
local tbBanana = inherit(tbFruit)
local tbBanana1 = inherit(tbFruit)
local tbBanana2 = inherit(tbFruit, tbBanana1)


--lua面向对象：概念意义上的实现
local _class={}
 
function class(super)
	local class_type={}
	--注意：因为重载了__newindex函数, 所以ctor不要定义为nil
	class_type.ctor=false
	class_type.super=super
	class_type.new=function(...) 
			local obj={}
			--下面的块只做了一件事：依次从父类到当前子类调用构造函数ctor
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
	
	--新加成员：防止定义类调用函数
	local vtbl={}
	_class[class_type]=vtbl
 
	setmetatable(class_type,{__newindex=
		function(t,k,v)
			vtbl[k]=v
		end
	})
 
	--只有定义类修改了__newindex
	--vbtl只属于定义类
	--new出来的对象共享所有定义类的引用，但独享自己新增加的成员变量
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


--test
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

local tbCircle3 = inherit(tbCircle2, {[1] = 3, [4] = "test"})

print("tbCircle1", tbCircle1)
for k, v in pairs(tbCircle1) do
	print(k, v)
	if type(v) == "table" then
		for k1, v1 in pairs(v) do
			print("\t", k1, v1)
		end
	end
end

print("tbCircle2", tbCircle2)
for k, v in pairs(tbCircle2) do
	print(k, v)
	if type(v) == "table" then
		for k1, v1 in pairs(v) do
			print("\t", k1, v1)
		end
	end
end

print("tbCircle3", tbCircle3)
for k, v in pairs(tbCircle3) do
	print(k, v)
	if type(v) == "table" then
		for k1, v1 in pairs(v) do
			print("\t", k1, v1)
		end
	end
end

for i=1, 4 do
	print(tbCircle3[i])
end
-------------------------------------------------------------------------

-- 2. table的长度
