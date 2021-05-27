local socket = require ( "socket" )

local tcpServer = nil

--创建指定端口的tcp server
local function createTCPServer( port )

    -- Create Socket
    local tcpServerSocket , err = socket.tcp()
    local backlog = 5

    -- Check Socket
    if tcpServerSocket == nil then
        return nil , err
    end

    -- Allow Address Reuse
    tcpServerSocket:setoption( "reuseaddr" , true )

    -- Bind Socket
    local res, err = tcpServerSocket:bind( "*" , port )
    if res == nil then
        return nil , err
    end

    -- Check Connection
    res , err = tcpServerSocket:listen( backlog )
    if res == nil then
        return nil , err
    end

    -- Return Server
    return tcpServerSocket

end

--接受来自客户端的连接，并保存在tcpClient
local tcpClient = nil
local function acceptClient()
    tcpServer:settimeout( 0 )
    clientIn , _ = tcpServer:accept()
    if clientIn then
        tcpClient = clientIn
    end
end

--接收已连接的客户端（tcpClient）数据
local function receiveLoop()
    if tcpClient ~= nil then
        local tcpClientMessage , _ = tcpClient:receive('*l')

        if ( tcpClientMessage ~= nil ) then
            print(tcpClientMessage)
            tcpClient:send( "back:" .. tcpClientMessage .. "\n")
        end
    end
end

local function main()
    tcpServer , _ = createTCPServer( 8080 )
    if tcpServer then
        -- Runtime:addEventListener( "enterFrame" , acceptClient )
        -- Runtime:addEventListener( "enterFrame" , receiveLoop )
        while true do
            acceptClient();
            receiveLoop();
        end
    end
end

main()