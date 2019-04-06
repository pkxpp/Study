
-- print("hello")

-- print(Test)

-- print("hi")
-- print(test)
-- print(test.m_data)
-- test.m_data = 5;
-- print(type(test.m_data))
-- print(222, getmetatable(test))
-- for k,v in pairs(test) do
-- 	print(k,v)
-- end;

-- print(test.m_data2)
-- test:SetData(1003)
-- test.m_data2 = 1004
-- print(test:GetData())
-- print(test.m_data2)
-- test.m_data3 = 2001
-- print(test.m_data3)

-- print(test:GetStr())

-- print("000", test:FuncTest(1, "test"))
-- print("FuncTest2")
-- print("222", test:FuncTest2())
-- print("xxxxxxxxx")
-- local a = test:FuncTest1("test1")
-- print("33333333333333")
-- print(111, a)

-- print(333, test:FuncTest3(3))

-- class object
-- print(test.m_SubTest)
-- print(test.m_SubTest.GetSubTestData)
-- print(test.m_SubTest:GetSubTestData())
------------------------------------------------------------
-- test vector
-- problem: vector iterators incompatible

-- print("IsExist: ", test:IsExist(5))
-- test:PushData(10);
-- print("IsExist: ", test:IsExist(10))

-- print("GetName() = ", test:GetName())
------------------------------------------------------------
--- *register c++ function to lua*
-- local g_objTest = FuncTest1();
-- print(g_objTest);
-- print(g_objTest:GetStr())

-- local subTest = g_objTest:GetSubClassPoint()
-- for k, v in pairs(getmetatable(subTest)) do
-- 	print("\t", k, v)
-- end

-- print(222)
-- print(subTest:GetSubTestData())
------------------------------------------------------------
-- *virtual function
-- print(111, test.VirtualFunc)
-- print(test:VirtualFunc())
-- print(222, base)
-- print(base:VirtualFunc())

------------------------------------------------------------
-- *new to malloc*
-- print("000", base2)
-- local nBase2Data = base2:GetData()
-- print(111, nBase2Data)

------------------------------------------------------------
-- gc study
-- collectgarbage("setpause", 100)
-- collectgarbage("setstepmul", 200)
-- local a = 13
local a = {}
-- function test()
-- 	local b = {};
-- 	return b;
-- end
-- test();
-- print(collectgarbage("collect"))
-- mytable = {"apple", "orange", "banana"}

-- print(collectgarbage("count"))

-- mytable = nil

-- print(collectgarbage("count"))

-- print(collectgarbage("collect"))

-- print(collectgarbage("count"))