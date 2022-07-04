
------------------------------------------------------------------------------------------------------
---协程和xpcall的问题：协程里面调用xpcall，里面又有yield，结果会报错：tempt to yield across metamethod/C-call boundary
--- http://bbs.chinaunix.net/forum.php?mod=viewthread&action=printable&tid=4065715


---http://lua-users.org/wiki/PcallAndCoroutines
local function tuple(...)
    return {n=select('#', ...), ...}
end

function pcall(f, ...)
    local co = coroutine.create(f)
    local res = tuple(coroutine.resume(co, ...))
    if res[1] and coroutine.status(co) == "suspended" then
        res[1] = false
    end
    return unpack(res, 1, res.n)
end

local handlers = setmetatable({}, {__mode='kv'})

function xpcall(f, err, ...)
    local co = coroutine.create(f)
    handlers[co] = err
    local res = tuple(coroutine.resume(co, ...))
    if res[1] and coroutine.status(co) == "suspended" then
        res[1] = false
    end
    if not res[1] and err then
        res[2] = err(co, res[2])
        -- note: assumes err can accept coroutine as first argument.
    end
    return unpack(res, 1, res.n)
end

function error(e, level_) --FIX:level not handled
    coroutine.yield(e)
end

-- test

local function g(x)
    if x == "fail" then
        error "fail"
    elseif x == "fail2" then
        local y = nil + nil  -- error raised by Lua
    end
end
  
local function f(x)
    print(1)
    g(x)
    print(2)
    return 3,4
end

print(pcall(f, "ok"))
print(pcall(f, "fail"))
print(pcall(f, "fail2"))
print(xpcall(f, debug.traceback, "fail"))
print(xpcall(f, debug.traceback, "fail2"))

------------------------------------------------------------------------------------------------------