--2014/01/16

--print(debug.traceback())
--[[
excute:error as follower:
stack traceback:
	stack_study.lua:3: in main chunk
	[C]: ?
]]

--2. a example
--[http://www.cnblogs.com/baiyanhuang/archive/2013/01/01/2841398.html]
-- debug.lua
gvar1 = 100
function foo()
        local var2 = 10
        function bar()
                debug.debug()
                return var2
        end
        bar()
end

debug.debug()
foo()