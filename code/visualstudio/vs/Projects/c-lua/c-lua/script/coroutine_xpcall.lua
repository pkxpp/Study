------------------------------------------------------------------------------------------------------
local LBuildingObject = {
	
};
function LBuildingObject:_UpdateCoroutineTask(...)
    local tbFinishedIndex = {};
    for k, co in ipairs(self.m_tbCoroutineTask or {}) do
        local r, bFinished = coroutine.resume(co, ...);
		print("r, Finished = ", r, bFinished)
        if bFinished then
            table.insert(tbFinishedIndex, k);
        end
    end

    -- 删除掉结束的任务
    for i = #tbFinishedIndex, 1, -1 do
        table.remove(self.m_tbCoroutineTask, i);
    end
end

---@function 创建一个携程封装的Task
---@param fnTask function
function LBuildingObject:_CreateCoroutineTask(fnTask, ...)
    if type(fnTask) ~= "function" then
        return;
    end

    local co = coroutine.create(function (...)
		local tbParams = {...};
		while true do
			local bFinished = fnTask(unpack(tbParams));
            tbParams = {coroutine.yield(bFinished)};
			print("bRet = ", unpack(tbParams))
		end
	end)

	print("resume...")
    local r, bFinished = coroutine.resume(co, 5);
	print("22 bFinished = ", r, bFinished)
    if not bFinished then
		self:_AddCoroutineTask(co);
    end

    return co;
end

---@param co coroutine
function LBuildingObject:_AddCoroutineTask(co)
    if not self.m_tbCoroutineTask then
        self.m_tbCoroutineTask = {};
    end

    table.insert(self.m_tbCoroutineTask, co);
end

function TestCoroutineTask()
	local co = LBuildingObject:_CreateCoroutineTask(function(a)
		print(111, a);
		return false;
	end);

	LBuildingObject:_UpdateCoroutineTask(0.5);
end

-- TestCoroutineTask();

------------------------------------------------------------------------------------------------------
--- 分帧更新
local LItemMgr = {};

function LItemMgr:_CreateCoroutineTask(fnTask, ...)
    if type(fnTask) ~= "function" then
        return;
    end

    local co = coroutine.create(function (...)
		local tbParams = {...};
		while true do
			-- local bFinished = fnTask(unpack(tbParams));
            -- xpcall不能带参数
			local status , bFinished = xpcall(function ()
				return fnTask(unpack(tbParams));
			end, debug.traceback); 

            tbParams = {coroutine.yield(bFinished)};
		end
	end)

    return co;
end

function LItemMgr:TickByCoroutine(bFrame)
	local nCount = 0;

    for nIndex, tbInfo in pairs(self.ItemList) do
		local nBlockID, uuid = unpack(tbInfo);
		local nNum = self:TickBlockByCoroutine(nIndex, nCount, bFrame);
		nCount = nCount + nNum;
	end

	self.ItemList = {};
    print("One tick end. Count = ", nCount)
end

function LItemMgr:TickBlockByCoroutine(nIndex, nStart, bFrame)
	local nCount = 0;
    local fnToDo = function ()
        local nBlockID, uuid;
        local tbItemInfo = self.ItemList[nIndex];
        if tbItemInfo then
            nBlockID, uuid = unpack(tbItemInfo);
            nCount = nCount + 1;
        end
        
        print("Real Do Something: ", nIndex, nBlockID, uuid)
        -- a.b = 5;
    end
    -- fnToDo();
    local bRet, szErr = xpcall(fnToDo, debug.traceback)
    print("xpcall invoke result: ", bRet, szErr);
    
    local COUNT_PER_FRAME = 2;
    if bFrame and (nStart + nCount) % COUNT_PER_FRAME == 0 then
        _, bFrame = coroutine.yield(false)
        print("bNewFrame = ", bFrame)
    end

	return nCount;
end

function LItemMgr:FrameMove(fDeltaTimeInSecs)
	print("FrameMove start---------------------", fDeltaTimeInSecs)
	local r, bFinished = coroutine.resume(self.CoTick, self, true);
	print(coroutine.status( self.CoTick ), r, bFinished)
	if not r then
		
	end
	
end

function LItemMgr:Tick(fDeltaTimeInSecs)
	print("Tick =============================================", fDeltaTimeInSecs)
	print(coroutine.resume(self.CoTick, self, false));

	-- 离开前准备数据
	self.ItemList = {};
	for nBlockID, tbBuildings in pairs(self.m_tbItemList) do
		for k, v in pairs(tbBuildings) do
			self:AddFrameMoveItem(nBlockID, k);
		end
	end

    -- 中间报错可能会导致协程dead，重新唤起
	if coroutine.status( self.CoTick ) == "dead" then
		self.CoTick = self:_CreateCoroutineTask(self.TickByCoroutine, self);
	end
end

function LItemMgr:AddFrameMoveItem(nBlockID, uuid)
	table.insert(self.ItemList, {nBlockID, uuid});
end


function TestItemTickCoroutineTask()
	LItemMgr.m_tbItemList = {
		[1] = {
			[1] = 1,
			[2] = 1,
			[3] = 1,
		},
		[2] = {
			[4] = 1,
		},
		[3] = {
			[5] = 1,
		}
	}
	LItemMgr.ItemList = {
		-- [nBlockID] = {uuid1, uuid2, ...}
	};
	
	LItemMgr.CoTick = LItemMgr:_CreateCoroutineTask(LItemMgr.TickByCoroutine, LItemMgr);
	LItemMgr:Tick(1);
	LItemMgr:FrameMove(1);
	LItemMgr:FrameMove(2);

	-- 因为有报错，所以前面的所有FrameMove都会包一次错
	-- LItemMgr:Tick(2);
	-- LItemMgr:FrameMove(3);

end

TestItemTickCoroutineTask();