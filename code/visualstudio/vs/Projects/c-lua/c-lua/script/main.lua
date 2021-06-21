print("Hello World.")

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

local _, szErr = pcall(TestInvalidKetToNext);
print(szErr)
print(debug.traceback());
print("end.")