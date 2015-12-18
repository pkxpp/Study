-- require study
-- page@2015/04/25
--------------------------------------------------------
--1. 
--------------------------------------------------------
local mRandom = require("random")
print (mRandom)

print(package.path)

function TestPrintPackageLoaded()
	print(getfenv(1), _env)
	for k, v in pairs(package.loaded) do
		print (k, v)
	end
end

print(getfenv(1), getfenv(0), getfenv(TestPrintPackageLoaded))
TestPrintPackageLoaded();

--sum: 
--	(1)'1'只打印了一次，所以require其实只是加载了一次random.lua文件
--	(2)正因为require的加载方式是把已经加载的内容放到了package.loaded中了，所以后面就不会加了
-- local mRandom_1 = require("random")


-- print(getfenv(2))