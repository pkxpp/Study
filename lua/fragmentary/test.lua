local tbX = {
	259,259,260,260,261,262,262,262,263,264,265,266,266,266,266,266,266,267,268,269,270,270,270,270,270,270,270,271,272,272,272,272,273,274,274,275,275,276,277,278,279,280,281,282,283,284
}
local tbY = {
565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,582,583,584,585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,600,601,602,603,604,604,604,604,604,604
}

local nCount = 0;
for k, v in pairs(tbX) do
	nCount = nCount + 1;
end
-- print("nCount = ", nCount)

local nDis = 0;
local xPre = 258
local yPre = 564
for i = 1, nCount do
	local nX = tbX[i];
	local nY = tbY[i];
	nDis = nDis + math.abs(nX - xPre) + math.abs(nY - yPre);
	xPre = nX;
	yPre = nY;
end
-- print("nDis = ", nDis)

-- 涨幅验证
-- print((85/60) ^ (1/4) - 1) 
-- print(60 * ((1 + 0.09098032586468227887254727768119) ^ 4)) 


-- print(2 ^ 3, 2 ^ 0.5)

-- 坐标点转换
local tbPos = {
	{13717, 26184},
	{12651, 21741},
	{9779, 14820},
	{8054, 17257},
	{8815, 23318},
	{20542, 22632},
	{21578, 25933},
	{19021, 29211},
}

-- for _, pos in pairs(tbPos) do
-- 	local x, y = unpack(pos);
-- 	print(x/32, y/32)
-- end

-- print(22249/32, 25924/32)

-- 道具ID计算
function tst_CalcItemID(  )
	Param = 33620270
	local nG = math.floor(Param/(2^24));
	local n1 = math.floor(Param%(2^24));
	local nD = math.floor(n1/(2^16));
	local nP = n1 % (2^16);
	print("道具ID：", nG, nD, nP)

	local tbItems = {
		{2, 1, 7001}, 
		{2, 27, 31}, 
		{2, 1, 101}, 
		{2, 1, 301},
		{2, 27, 81}, 
		{2, 1, 102}, 
		{2, 1, 302}
	}
	for k, v in pairs(tbItems) do
		local g, d, p = unpack(v);
		local szKey = table.concat(v, ",");
		local nID = g * (2^24) + d * (2^16) + p;
		local szLog = string.format("[%s] = %d", szKey, nID);
		print(szLog)
	end
end
-- tst_CalcItemID();

-- 武器向量计算 2018-10-06 11:50:22
local tbAnim = {
	{{["x"] =-20,  ["y"] =169,  ["z"] =-19 }, {["x"] =39, ["y"] =-93,   ["z"] =-3 }},
	{{["x"] =-8,  ["y"] =82,  ["z"] =-34 }, {["x"] =-82, ["y"] =7,   ["z"] =-32 }},
	{{["x"] =-65, ["y"] =141, ["z"] =16  }, {["x"] =-51, ["y"] =74,  ["z"] =95  }},
	{{["x"] =-66, ["y"] =159, ["z"] =-9  }, {["x"] =37,  ["y"] =160, ["z"] =9   }},
	{{["x"] =66,  ["y"] =56,  ["z"] =-19 }, {["x"] =166, ["y"] =37,  ["z"] =-44 }},
	{{["x"] =68,  ["y"] =69,  ["z"] =-34 }, {["x"] =152, ["y"] =64,  ["z"] =-98 }},
	{{["x"] =-19, ["y"] =78,  ["z"] =51  }, {["x"] =-46, ["y"] =41,  ["z"] =145 }},
}

function tst_CalcWeaponVector2D(tbAnim)
	for i, tbOnePosition in pairs(tbAnim or {}) do
		local tbStart = tbOnePosition[1]
		local tbEnd = tbOnePosition[2]
		if tbStart and tbEnd then
			local x = tbEnd.x - tbStart.x
			local y = tbEnd.y - tbStart.y
			local z = tbEnd.z - tbStart.z
			print(i .. ": (" .. x .. ", " .. y .. "," .. z .. ")" .. "-->" .. math.sqrt(x^2 + y^2 + z^2))
		end 
	end
end
-- tst_CalcWeaponVector2D(tbAnim);

----------------------------------------------------------------------------
-- https://stackoverflow.com/questions/61099927/how-to-iterate-through-results-in-sets
local t = {
	{6, 5, -3},
	{12, 12.4, -5},
	{18, 19.8, -7},
}
for _, v in ipairs(t) do
	local l, p1, p2 = unpack(v)
	print(l, p1, p2)
end

function emptyContents()
    contents = self.getObjects()

    for i, _ in ipairs(self.getObjects()) do
		for _, v in ipairs(t) do
			local l, p1, p2 = unpack(v)
			if i <= l then
				self.takeObject(setPosition(p1, p2))
				break;
			end
		end
    end
end

----------------------------------------------------------------------------
-- https://stackoverflow.com/questions/61168399/how-to-transform-a-key-value-table-into-a-list-lua

local tExpand = {a="hello", b="world"}
function expand(t)
  local i = 1
  local res = {}
  for k, v in pairs(t) do
    res[i] = k
    i = i + 1
    res[i] = v
    i = i + 1
  end
  return res
end

function expand1(t)
	local res = {}
	for k, v in pairs(t or {}) do
		res[#res + 1] = k;
		res[#res + 1] = v;
	end
	return res;
end

-- local tExpandRet = expand(tExpand);
local tExpandRet = expand1(tExpand);
for k, v in pairs(tExpandRet) do
	print(k, v)
end
