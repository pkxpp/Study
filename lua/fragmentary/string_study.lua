--2013-09-18
--[[
a = "hello, world!hello, li!"
p, q = print(string.find(a, "hello"))
while(p) do
	print(p, q)
	p, q = print(string.find(a, "hello"))
end
]]
--2
--[[
b = "test(char)"
print(b)
print(string.find(b, "%(.*%)"))
c = string.reverse(b)
print(c)
p1, p2 = string.find(c, "%).*%(")
d= string.sub(c, p2+1)
print(d)
print(string.reverse(d))
]]
--[[
szFamilyName = "sdf(Title)"
nPos1 = string.find(szFamilyName, "%(")
szFamilyName1 = string.sub(szFamilyName, 1, nPos1-1)
print(11, szFamilyName1)
]]

----------------------------------------
--20.3 captures
--[[
pair = "name = Anna"
local n1, n2, key, value = string.find(pair, "(%a+)%s*=%s*(%a+)")
print(n1, n2, key, value)
--
local szName = "a,b,c"
--迭代

function split(str,splitor)
	if(splitor==nil) then
		splitor=','
	end
	local strArray={}
	local strStart=1
	local splitorLen = string.len(splitor)
	local index=string.find(str,splitor,strStart,true)
	if(index==nil) then
		strArray[1]=str
		return strArray
	end
	local i=1
	while index do
		strArray[i]=string.sub(str,strStart,index-1)
		i=i+1
		strStart=index+splitorLen
		index = string.find(str,splitor,strStart,true)
	end
	strArray[i]=string.sub(str,strStart,string.len(str))
	return strArray
end

local tbData = split(szName)
for k, v in pairs(tbData) do
	print(k, v)
end
]]

------------------------------
local szTest = "A"
--print(string.len(szTest))
--kmp
function kmp_next(szMode)
	local tbNext = {}
	szMode = tostring(szMode)
	tbNext[1] = 0;
	for j=2, string.len(szMode) do
		local i = tbNext[j-1];
		while szMode[j] ~= szMode[i+1] and i >= 1 do
			i = tbNext[i]
		end
		if szMode[j] == szMode[i+1] then
			tbNext[j] = i + 1
		else
			tbNext[j] = 0;
		end
	end
	return tbNext
end

function get_next(szMode)
	--print(szMode)
	local tbNext = {}
	szMode = tostring(szMode)
	local len = string.len(szMode)
	tbNext[1] = 0;
	local i = 1;
	local k = 0;
	--print("len", len)
	while i < len do
		--print("k, i", k, i, szMode[i], szMode[k])
		local szI = string.sub(szMode, i, i)
		local szK = string.sub(szMode, k, k)
		--print("szI, szK", szI, szK)
		
		if k == 0 or szI == szK then
			k = k + 1;
			i = i + 1;
			local szI = string.sub(szMode, i, i)
			local szK = string.sub(szMode, k, k)
		
			if szI ~= szK then
				tbNext[i] = k
			else
				tbNext[i] = tbNext[k];
			end
		else			
			k = tbNext[k];
		end
	end
	return tbNext
end

function kmp1(szDst, szMode)
	local tbNext = get_next(szMode)
	if #tbNext <= 0 then return; end
	
	local p = 1
	local s = 1
	while p <= string.len(szMode) and s <= string.len(szDst) do
		local cDst = string.sub(szDst, p, p)
		local cMod = string.sub(szMode, s, s)
		if cDst == cMod then
			p = p + 1;
			s = s + 1;
		else
			if p == 1 then
				s = s + 1;
			else
				p = tbNext[p-1] + 1;
			end
		end
	end
	
	if p < string.len(szMode) then
		return -1;
	end
	return s - string.len(szMode)
end

function kmp(szDst, szMode)
	local tbNext = get_next(szMode)
	if #tbNext <= 0 then return; end
	
	local p = 1
	local s = 1
	while p <= string.len(szMode) and s <= string.len(szDst) do
		local cDst = string.sub(szDst, p, p)
		local cMod = string.sub(szMode, s, s)
		if cDst == cMod then
			p = p + 1;
			s = s + 1;
		else
			p = tbNext[p];
		end
	end
	
	if p == string.len(szMode) then
		return s - p;
	end
	return -1;
end

function kmp_ex(szDst, szMode)
	--print(szDst, szMode)

	local lenDst = string.len(szDst)
	local lenMod = string.len(szMode)

	--if lenDst ~= lenMod then return -2; end
	local tbNext = get_next(szMode)

	if #tbNext <= 0 then return -2; end
	
	local p = 1
	local s = 1
	while p <= string.len(szMode) and s <= string.len(szDst) do
		local cDst = string.sub(szDst, p, p)
		local cMod = string.sub(szMode, s, s)
		--print("cDst, cMod", cDst, cMod)
		if cDst == cMod then
			p = p + 1;
			s = s + 1;
		else
			--if tbNext[p] == 0 then
				--return tbNext[p] + 1
			--else
				return s, tbNext[p];
			--end
		end
	end
	--print(p, s)
	--if p == string.len(szMode) then
	--	return -1;
	--end
	return -1;
end

--local tbNext = get_next("abcabaa")
--local tbNext = get_next("abcaabbabcabaacbacba")
--local tbNext = get_next("aaaadd")
--for k, v in pairs(tbNext) do
--	print(k, v)
--end
--local szMode = "aab"
--local szDst = "aaa"
--local pos = kmp_ex(szDst, "aab")
--print("pos: ", pos)

function random_seq(len)
	local szSeq = ""
	for i=1, len do
		local r = math.random(2)
		szSeq = szSeq .. tostring(r);
	end
	return szSeq
end

function test_kmp()
	for i=1, 5 do
		local szDst = random_seq(2)
		local szMod = random_seq(3)
		print(szDst, szMod)
		local posDst, posMod = kmp_ex(szDst, szMod)
		print("pos: ", posDst, posMod)
	end
end

--[[
121	221
pos: 	0
122	212
pos: 	0
221	111
pos: 	0
112	111
pos: 	2
112	222
pos: 	0
]]
--print(kmp_ex("12", "112"))
--test_kmp();

--[[
12	122
pos: 	-1	nil
11	222
pos: 	1	0
12	221
pos: 	1	0
11	111
pos: 	-1	nil
21	111
pos: 	1	0
]]
--print(kmp_ex("222", "12"))

function CmpKill2Mode(szKill, szMode)
	--if string.len(szKill) ~= string.len(szMode) then return -1; end
	--if szKill == szMode then
	--	return 0;
	--end
	local nPos = string.find(szKill, szMode)
	if nPos then 
		return 0, nPos + string.len(szMode);	--这里只要第二个参数
	end
	local szTemp = szKill
	local nLen = string.len(szTemp)
	while nLen > 1 do
		szTemp = string.sub(szTemp, 2, nLen)
		local nPos = string.find(szMode, szTemp)
		--print(szTemp, nPos)
		if nPos and nPos == 1 then
			return nLen-1, nPos;
		end
		nLen = string.len(szTemp)
	end
	
	if nLen == 1 then
		local szMode_1 = string.sub(szMode, 1, 1)
		--print(szKill, szMode_1)
		if szKill == szMode_1 then
			return 1, 1
		end
	end
	return -1;
end
for i = 1, 10 do
	local sz1 = random_seq(6)
	local sz2 = random_seq(5)
	local nRet, nPos = CmpKill2Mode(sz1, sz2)
	if nRet >= 0 then
		--print(sz1, sz2, nRet, nPos)
	end
end

local nRet, nPos = CmpKill2Mode("1", "112")
-- print(nRet, nPos)
--local szTest = string.sub("221", nPos, string.len("221"))
--print(szTest, string.len(szTest))

-------------------------
--这是什么用法？
function CheckLua()
	local s = "a string with \r and \n and \r\n and \n\r"
	print(10, s)
	local c = string.format("return %q", s)
	local z = assert(loadstring(c))()
	print(11, z)
	print(12, a == s)
	assert(assert(loadstring(c))() == s)
end
-- print(111, CheckLua())


local tbTest = {1, 2, 3}
--tbTest[2], tbTest[3] = tbTest[3], tbTest[2]
for k, v in ipairs(tbTest) do
	--print (k, v)
end

function AllPerm(tbTest)
	--test
	--print("AllPerm start......", #tbTest)
	for k, v in ipairs(tbTest) do
		--print(v)
	end
	
	if #tbTest == 1 then
		print(tbTest[1], ",")
		return;
	end
	
	--copy
	local tbData = {}
	for _, v in ipairs(tbTest) do
		table.insert(tbData, v)
	end
	
	local tbPerm = {}
	for k, v in ipairs(tbTest) do
		--change
		tbData[k], tbData[#tbData] = tbData[#tbData], tbData[k]
		print(tbData[#tbData], ",")
		
		local n = table.remove(tbData)
		AllPerm(tbData)
		print(".....", n)
		
		--copy back
		tbData = {}
		for _, v in ipairs(tbTest) do
			table.insert(tbData, v)
		end
	end
	
	--copy back
	--for _, v in ipairs(tbTest) do
	--	table.insert(tbData, v)
	--end
end

--AllPerm(tbTest)

--2015/01/29 string.len("")
-- print(string.len(""))

--2015/08/25 捕获
-- local pair = "name = Anna"
-- local key, value = string.match(pair, "(%a+)%s*=%s*(%a+)")
-- print(key, value)
-- local pair1 = "【xxx】 jiu是这样的e"
-- local s1, s2 = string.match(pair1, "【(.*)】%s*(.*)");
-- print(pair1, s1, s2);
-- local s3, s4 = string.find(pair1, "【%s")
-- print(s3, s4);

-- print(os.date("%X", os.time()))
-- print(os.date("%p", os.time()))
-- print(os.date("%H:%M", os.time()))

-- 2015/08/26 不定长参数
function addMessageTipsEx(...)
	local massage = string.format("%s:%d", ...)
	print(massage);
end

addMessageTipsEx("test", 6);