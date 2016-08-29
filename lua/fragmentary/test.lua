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
tst_CalcItemID();
