--
--23.2 hook
function trace(event, line)
	local s = debug.getinfo(2).short_src
	print(s .. ":" .. line)
end
debug.sethook(trace, "l")

for i=1,5 do
	print(i)
end