socket = require ("socket")

print("start..............1111111")
function download(host, file)
	local c = assert(socket.connect(host,80))
	local nCount = 0;	--count nbyte
	c:send("GET" .. file .. "HTTP/1.0\r\n\r\n");
	
	while true do
		local s, status = receive2(c)
		nCount = nCount + string.len(s)
		if status == "closed" then break; end
	end
	
	c:close()
	print(file, nCount)
end

function receive1(connection)
	return connection:receive(2^10)
end

function receive2(connection)
	connection:timeout(0)
	local s, status = connection:receive(2^10)
	print("11111111111")
	if status == "timeout" then 
		coroutine.yield(connection)
	end
	return s, status;
end

local threads = {}

function get(host, file)
	--create coroutine
	print("22")
	local co = coroutine.create(function ()
		download(host, file)
	end)
	
	table.insert(threads, co)
end

function dispatch()
	while true do
		local n = table.getn(threads)
		if n == 0 then break; end;
		
		for i=1, n do
			local status, res = coroutine.resume(threads[i])
			if not res then
				table.remove(threads, i)
				break;
			end
		end
	end
end
--
print("start..............")
host = "www.w3.org"
get(host, "/TR/html401/html40.txt")
get(host, "/TR/2002/REC-xhtml-20020801/xhtml.pdf")
get(host, "/TR/REC-html32.html")
get(host, "/TR/2000/REC-DOM-Level-2-core-20001113/DOM2-Core.txt")
--main loop
dispatch()