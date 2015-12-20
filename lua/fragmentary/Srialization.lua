--
--pk
--2013/06/25
--
function test1()
	local count = 0
	function Entry(_) count = count + 1 end
	dofile("data")
	print ("number of entries: " .. count)
end

function serialize(o)
	if type(o) == "number" then
		io.write(o)
	end
	if type(o) == "string" then
		io.write("[[", o, "]]")
	end
end

co = coroutine.create(function ()
	for i=1, 10 do
		print("co", i)
		coroutine.yield()
	end
end)


--serialize("]]..os.execute(rm a.txt)..[[")
coroutine.resume(co)
print(coroutine.status(co))
coroutine.resume(co)
