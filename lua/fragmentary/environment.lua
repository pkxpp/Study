--pre

--for n in pairs(_G) do print(n) end
--_G._G = _G在打印里面可以看到_G这个成员
--14.1 _G
--[[
function getfield(f)
	local v = _G
	for w in string.gfind(f, "[%w_]+") do
		v = v[w]
	end
	return v
end

function setfield(f, v)
	local t = _G
	for w, d in string.gfind(f, "([%w_]+)(.?)") do
		if d == "." then
			t[w] = t[w] or {}
			t = t[w]
		else
			t[w] = v
		end
	end
end

setfield("t.x.y", 10)
print(t.x.y, t.x)
print(getfield("t.x.y"), getfield("t.x"))
]]

--------------------------------------
--14.2
--[[
function decalre(name, initval)
	rawset(_G, name, initval or false)
end

setmetatable(_G, {
	__newindex = function (_, n)
		error("attempt to write to undeclared variable " .. n, 2)
	end,
	__index = function(_, n)
		error("attempt to read undeclared variable " ..n, 2)
	end
})
--a=1
decalre "a"
a = 1
]]

-------------------------------------------
--14.3
a=1
setfenv(1, {})
print(a)