local socket = require("socket")
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
        local ret = tcpClient:connect("127.0.0.1", 8080)
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