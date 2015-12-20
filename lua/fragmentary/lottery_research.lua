local tbInput = {
	[1] = 3.12,
	[2] = 3.30,
	[3] = 2.00,
}

function calc_percent(tbInput)
	local tbPercent = {1.0/3, 1.0/3, 1.0/3}
	local nSum = 0;
	for k, v in pairs(tbInput) do
		nSum = nSum + tonumber(v);
	end
	
	tbPercent[1] = tbInput[3]*1.0/nSum;
	tbPercent[3] = tbInput[1]*1.0/nSum;
	tbPercent[2] = 1.0 - tbPercent[1] - tbPercent[3];
	--[[local nCount = 0.0
	for k, v in ipairs(tbInput) do
		if k == #tbInput then
			tbPercent[k] = 1.0 - nCount;
		else
			tbPercent[k] = v*1.0/nSum;
			nCount = nCount + tbPercent[k]
		end
	end]]
	
	--test
	for _, v in ipairs(tbPercent) do
		print(v)
	end
	--
	return tbPercent;
end

--calc_percent(tbInput)

tbAllInput = {
	[1] = {
		[1] = 3.12,
		[2] = 3.30,
		[3] = 2.00
	},
	[2] = {
		[1] = 1.86,
		[2] = 3.20,
		[3] = 3.65,
	},
}
function cmp_element(tbA, tbB)
	if tbA[5] > tbB[5] then
		return true;
	end
	return false;
end

--所有比赛集合，场数，成功概率，赔率
function calc_result(tbAllInput, nCount, nProSuccess, nMulti)
	local tbRet = {}
	local nCount = 1
	--for k, v in ipairs(tbAllInput)	
	--	for k1, v1 in ipairs(v) do
	--	end
	--end
	local tbAllPer = {}
	for k, v in ipairs(tbAllInput) do
		local tbPer = calc_percent(v)
		--for k1, v1 in ipairs(tbPer) do
		--	print(k1, v1)
		--end
		tbAllPer[k] = tbPer
	end
	
	for i=1, 3 do
		for j=1, 3 do
			local nMulti = tbAllInput[1][i] * tbAllInput[2][j]
			local nPer = tbAllPer[1][i] * tbAllPer[2][j]
			table.insert(tbRet, {1, i, 2, j, nMulti, nPer})
		end
	end
	
	table.sort(tbRet, cmp_element)
	--test
	local nSum = 0
	for k, v in pairs(tbRet) do
		nSum = nSum + v[6]
		print(unpack(v))
	end
	print("All Sum: ", nSum)
end

calc_result(tbAllInput)