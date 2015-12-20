-------------------------------------------
--page@2015/01/27
--content:
--1.luaƒ⁄¥Êπ‹¿Ì
-------------------------------------------
function test1()
	collectgarbage("collect")
	local c1 = collectgarbage("count")
	print("lua storage:", c1)
	local colen = {}
	for i=1, 5000 do
		table.insert(colen, {})
	end
	local c2 = collectgarbage("count")
	print("after lua storage:", c2)
end

function mem()
	print("collectgarbage...")
	local c2 = collectgarbage("count")
	collectgarbage("collect")
	print("after collectgarbage:", c2)
end

function test2()
	collectgarbage("collect")
	local c1 = collectgarbage("count")
	print("lua storage 2:", c1)
	colen = {}
	for i=1, 5000 do
		table.insert(colen, {})
	end
	local c2 = collectgarbage("count")
	print("after lua storage 2:", c2)
end