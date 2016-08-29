--coroutine
-- 2014/01/17
--[[
function producer1()
	while true do
		local x = io.read()
		send(x)
	end
end

function consumer1()
	while true do
		local x = receive()
		io.write(x, "\n")
	end
end

function receive()
	return coroutine.resume(producer)
end

function send(x)
	coroutine.yield(x)
end
]]
--[[
（1）直接调用函数producer1,出现下面错误
F:\我的文件\data\kuaipan\work\work_diary\task\Lua_Study\code>lua coroutine_study
.lua
1
lua: attempt to yield across metamethod/C-call boundary
stack traceback:
        [C]: in function 'yield'
        coroutine_study.lua:23: in function 'send'
        coroutine_study.lua:6: in function 'producer'
        coroutine_study.lua:26: in main chunk
        [C]: ?
（2）coroutine的基本使用
]]

--（2）基本使用，要有个创建的过程，上面的错误导致就应该和这个有关系，没有协同程序在啊
--[[
co = coroutine.create(function ()
	print("Hello wrold!")
end)
print(co)
print(coroutine.status(co))
coroutine.resume(co)
print(coroutine.status(co))

co = coroutine.create(function()
	for i = 1, 10 do
		print("co", i)
		coroutine.yield()
	end
end
)
coroutine.resume(co)
print(coroutine.status(co))
coroutine.resume(co)
coroutine.resume(co)

co = coroutine.create(function(a,b,c)
	print("co", a, b, c)
end)
//coroutine.resume(co, 1, 2, 3)
coroutine.resume(co, 1, 2)

//yield的参数会传给resume
co = coroutine.create(function(a, b)
	coroutine.yield(a+b, a-b)
end)

print(coroutine.resume(co, 20, 10)) //true 30 10
print(coroutine.resume(co, 20, 10)) //true【注后面没有了，主函数没有返回任何职】

//修改之后如下
co = coroutine.create(function(a, b)
	coroutine.yield(a+b, a-b)
	return 5
end)
print(coroutine.resume(co, 10, 10)) --true 20 0
print(coroutine.resume(co, 20, 10)) --true 5

//resume的参数会传给yield
co = coroutine.create(function ()
	print("co", coroutine.yield())
end)

coroutine.resume(co, 3)
coroutine.resume(co, 4, 5)
]]
--(3)再写生产者和消费者
--[[
producer = coroutine.create(function ()
	while true do
		local x = io.read()
		print("before send(yield)")
		send(x)
		print("after send(yield)")
	end
end)

consumer = coroutine.create(function()
	while true do
		print("before resume")
		local s, v = receive()
		if s then
			io.write(v, "\n")
			print("after resume1")
		end
		print("after resume2")
	end
end)
print("start............")
coroutine.resume(producer)
coroutine.resume(consumer)
]]
--[[
(1)消费者和生产者都是一个协同程序
(2)加入一些打印信息，清楚了许多
start............
5
before send(yield)
before resume
after send(yield)
6
before send(yield)
6
after resume1
after resume2
before resume
after send(yield)
如上面的例子中加的打印信息：
（1）read函数会阻塞，所以start打印之后没有打印信息了
这个时候消费者函数都还没开始执行，就是这个线程还没开
（2）receive函数（也就是resume）也会阻塞，等待他resume的
协同程序返回（包括yield的参数以及主函数的最后返回值）
（3）yield会挂起协同程序（线程），但是不会阻塞在那里。
所以第一次输入5的时候，producer程序在send地方挂起，打印before信息
之后挂起，然后comsuer线程继续执行了，打印before之后再receive地方
（resume）有两件事：唤醒producer程序，所以打印了after send；然后就是
阻塞在哪里等待resume的程序producer返回，但是producer被read阻塞了，
所以这个时候生产者和消费者都被阻塞了
（4）当再次输入6的时候，生产者先打印before send，然后yield再度挂起，参数为6；
消费者comsuer收到了producer的yield参数为2,（还是上一把阻塞到现在的）；
接着打印receive中的信息，并且由循环阻塞到receive的地方，并且唤醒producer；
所以producer的after send打印出来了
]]

--(4)过滤器和管道
--[[
function receive (prod)
	local status, value = coroutine.resume(prod)
	return value
end

function send (x)
	coroutine.yield(x)
end

function producer()
	return coroutine.create(function ()
		while true do
			local x = io.read()
			send(x)
		end
	end)
end

function filter (prod)
	return coroutine.create(function ()
		local line = 1
		while true do
			local x = receive(prod)
			x = string.format("%5d %s", line, x)
			send(x)
			line = line + 1
		end
	end)
end

function consumer (prod)
	while true do
		local x = receive(prod)
		io.write(x, "\n")
	end
end
]]
--[[
p = producer()
f = filter(p)
consumer(f)
]]

--------------------------------------
--(5)抢占coroutine
--[[
注：这里只做了请求的发送，而没有接受数据
require "luasocket"
host = "www.w3.org"
file = "/TR/REC-html32.html"
c = assert(socket.connect(host, 80))
c:send("GET" .. file .. "HTTP/1.0\r\n\r\n")
c:close()
]]

--(5.1)coroutine
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
-- print("start..............")
-- host = "www.w3.org"
-- get(host, "/TR/html401/html40.txt")
-- get(host, "/TR/2002/REC-xhtml-20020801/xhtml.pdf")
-- get(host, "/TR/REC-html32.html")
-- get(host, "/TR/2000/REC-DOM-Level-2-core-20001113/DOM2-Core.txt")
--main loop
-- dispatch()

--2015/09/02 携程是否阻塞？
function Test()
	local co = coroutine.create(function(a, b, c)
		print("co", a, b, c)
	end)
	coroutine.resume(co, 1, 2, 3);
	print("run here ")
end

Test();
--

function LoadSpine()
	local function spineLoaded(id)
		print("33333333333333333333333333333", id)
		for i=1, 100 do
			print(id, i)
		end
	end
	
	local tbSpines = {
		[1] = 1001,
		[2] = 1002,
		[3] = 1003,
	}
	for _, id in pairs(tbSpines) do
		print(666, id)
		local co = coroutine.create(function(id)
			print(555, id);
			spineLoaded(id);
		end)
		coroutine.resume(co, id)
		print(777)
	end
end

-- LoadSpine();