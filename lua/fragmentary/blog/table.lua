-- 1. table的长度
local t = {111, x = 222, nil, 333, [10] = 555, {}, nil, nil}
-- 4
print(table.getn(t))
-- 4
print(#t)
-- 10
print(table.maxn(t))

local t1 = {1, 2, nil}
print("#t1 = ", #t)
--[[
i=0, j=3
(1)m=1, array[m-1]=1不为nil, i=m=1
(2)m=2, array[m-1]=2不为nil, i=m=2
(3)j-i <=1循环结束，return i，结果为2
]]

local t1 = {1, nil, 2, nil}
print("#t1 = ", #t1)
--[[
i=0, j=4
(1)m=2, array[m-1]=nil, j=m=2
(2)m=1, array[m-1]=1不为nil, i=m=1
(3)j-i <=1循环结束，return i，结果为1
]]

------------------------------------------------------------
-- 2. sort
local tbTest = {
    {1, 3},
    {3, 5},
    {5, 4},
    {2, 3},
}

local tbSort = {25, 18, 15}

-- 比较函数
function cmp(a, b)
    return a[2] < b[2]
end

-- 通常用法
table.sort(tbTest, cmp)

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

-- table.sort(tbSort, fComp)
-- table.sort(tbSort, tbSort.fComp)
table.sort(tbSort, tbSort.fCompWrap)