local socket = require("socket")
local http = require("socket.http");
local tcpClient = nil
local index = 0;

--接收来自服务器的数据
local function receiveData()
    if tcpClient then
        tcpClient:settimeout(100)
        local msg = tcpClient:receive("*l")
        if msg then
            print(msg)

            index = index + 1;
            local msg = "index = " + index;
            tcpClient:send("hi.\n")
        end
    end
end

--向服务器发送数据
-- local index = 0
--     local function btnListener(event)
--     index = index + 1
--     if tcpClient then
--         tcpClient:send("pack index:" .. index .. ".\n")
--     end
-- end

--初始化
local function main()
    -- local btn = ui.newButton{
    -- default = "buttonBlue.png",
    -- over = "buttonBlueOver.png",
    -- onRelease = btnListener,
    -- id = "btn"
    -- }
    -- btn:setReferencePoint(display.TopLeftReferencePoint)
    -- btn.x = 10
    -- btn.y = 100

    --创建tcp连接
    tcpClient = socket.tcp()
    if tcpClient then
        -- local ret = tcpClient:connect("127.0.0.1", 8080)
        local ret = tcpClient:connect("127.0.0.1", 60636)
        if ret then
            tcpClient:send("hi, server, i'm client.\n")
            while true do
                -- Runtime:addEventListener("enterFrame", receiveData)
                receiveData();
            end
        end
    end
end
main()

------------------------------------------------------------------------------------------------------
function client_http()
    local request_body = [[req='what's your name']]
    local response_body = {}
    local res, code, response_headers = http.request{
        -- url = "http://10.11.8.207:8889/chat",
        url = "http://www.tecgraf.puc-rio.br/~diego",
        headers =
        {
            ["Content-Type"] = "application/x-www-form-urlencoded";
            ["Content-Length"] = #request_body;
        },
        method = "POST",
        source = ltn12.source.string(request_body),
        sink = ltn12.sink.table(response_body),
    }

    print(res)
    print(code)
    
    print("Response body:")
    if type(response_body) == "table" then
        for k, v in pairs(response_body) do
            print(k, v, type(v))
        end
        print(table.concat(response_body))
    else
        print("Not a table:", type(response_body))
    end
    
    -- b, c, h = http.request("http://10.11.8.207:8889/chat")
    -- print(b, c, h)
    
end
-- client_http();
------------------------------------------------------------------------------------------------------
--- 非阻塞的方式

-- -- 创建一个协程来执行非阻塞的HTTP请求
-- local function makeHttpRequest(url)
--     local response, status, headers = http.request(url)
--     -- local request_body = [[req='what's your name']]
--     -- local response_body = {}
--     -- local response, status, headers = http.request{
--     --     url = url,
--     --     headers =
--     --     {
--     --         ["Content-Type"] = "application/x-www-form-urlencoded";
--     --         ["Content-Length"] = #request_body;
--     --     },
--     --     method = "POST",
--     --     source = ltn12.source.string(request_body),
--     --     sink = ltn12.sink.table(response_body),
--     -- }

--     -- print("Response body:")
--     -- if type(response_body) == "table" then
--     --     for k, v in pairs(response_body) do
--     --         print(k, v, type(v))
--     --     end
--     --     print(table.concat(response_body))
--     -- else
--     --     print("Not a table:", type(response_body))
--     -- end
--     print(33, response);
--     coroutine.yield(response, status, headers)
-- end

-- -- 创建一个协程
-- local co = coroutine.create(makeHttpRequest)
-- print("111111111111111")
-- -- 启动协程
-- coroutine.resume(co, "http://www.cs.princeton.edu/~diego/professional/luasocket/http.html")
-- print("22222222")

-- -- 检查协程是否完成
-- while coroutine.status(co) ~= "dead" do
--     -- 做一些其他的工作
--     socket.sleep(0.1) -- 暂停一小段时间，避免占用过多CPU资源

--     -- 检查协程是否完成
--     local success, response, status, headers = coroutine.resume(co)
--     if success then
--         -- 处理HTTP响应
--         print(response, status, headers)
--     else
--         -- 处理协程错误
--         print("Coroutine error:", response)
--         break
--     end
-- end


-- 使用 LuaWebSocket 库创建 WebSocket 客户端
-- local websocket = require("websocket")

-- local client = websocket.client.copas.new()
-- local ok, err = client:connect("ws://10.11.8.207:8889/chat")
-- if ok then
--   client:send([[req='what's your name']])
--   local response = client:receive()
--   print(response)
-- else
--   print("Failed to connect:", err)
-- end




-- 发送HTTP POST请求
local function http_post(url, data, callback)
--   local host, path = string.match(url, "http://([^/]+)(.+)")
--   local host = "10.11.8.207"
--   local path = "chat";
    -- local port = 8889;
--   local data = [[req='what's your name']]
    -- local host = "http://www.lua.org"
    local host = "www.lua.org"
    local path = "";
    local data = [[name=Lua&age=100]]
    local port = 80;
  local con = assert(socket.connect(host, port))
  print("con = ", con)
  local request = string.format("POST /%s HTTP/1.0\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s",
                                path, host, #data, data)
--   print("request", request)
  con:send(request)
  print("111")
  local response = ""
  while true do
    local chunk, status, partial = con:receive(1024)
    response = response .. (chunk or partial)
    print("status = ", status);
    if status == "closed" then break end
  end
  
  con:close()
  callback(response)
end

-- 异步回调函数
local function handle_response(response)
  print("Response:", response)
end

-- 使用示例
local url = "http://example.com"
local data = "name=John&age=30"
-- http_post(url, data, handle_response)

------------------------------------------------------------------------------------------------------
