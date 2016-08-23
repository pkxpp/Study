-- 一个函数
function test()
    print("Test setfenv.")
end
-- 一个变量
testvar = 666
-- print是_G的函数，在新环境中不存在
-- print("test ... ")

-- print("222222")
-- local _G = _G

-- _G.print("test ...")
