print("Hello World.")
print(package.path)

print(math.floor(19.9 * 100))  --1989
print(222)
-- local prf = require("profiler") -- 引入profiler模块
-- print(111, prf)
-- print(333)

function TestInvalidKetToNext()
	local t = {1,2}

	local remove_ele =function ()
	 	t[1] = nil
	end

	local add_ele = function ()
	  t[3] = 3 --err:增加原来没有的元素
	  --t[1] = 1 --correct:修改原有的元素
	end

	for k,v in pairs(t) do
		print(k, v)
		 remove_ele()
		 print(111)
		 add_ele()
		 print(222)
	end
end

-- local _, szErr = pcall(TestInvalidKetToNext);
-- print(szErr)
-- print(debug.traceback());
-- print("end.")

function TestInsertTableWhenPair()
	local tb = {
		-- [3] = 0,
		[4] = 1,
		[5] = 5
	}

	for k, v in pairs(tb) do
		if k == 5  then
			tb[6] = 6;
		end
		print(k ,v);
		--[[
			这一句插入，导致迭代器会把k=4这个插入两遍
			4       1
			5       5
			6       6
			4       1
		]]
	end
end
TestInsertTableWhenPair();