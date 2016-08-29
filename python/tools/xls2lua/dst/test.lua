-- excel xlstable format (sparse 3d matrix)
--{	[sheet1] = { [row1] = { [col1] = value, [col2] = value, ...},
--					 [row5] = { [col3] = value, }, },
--	[sheet2] = { [row9] = { [col9] = value, }},
--}
-- nameindex table
--{ [sheet,row,col name] = index, .. }
sheetname = {
["property"] = 1,
};

sheetindex = {
[1] = "property",
};

local test = {
[1] = {
	[10000] = {
		["test1"] = 10000,
		["test2"] = "测试10001",
		["test3"] = {1, 2, 3},
	},
	[10001] = {
		["test1"] = 10001,
		["test2"] = "测试1002",
		["test3"] = {4, 5, 6},
	},
	[10002] = {
		["test1"] = 10002,
		["test2"] = "测试1003",
		["test3"] = {7, 8, 9},
	},
},
};

-- functions for xlstable read
local __getcell = function (t, a,b,c) return t[a][b][c] end
function GetCell(sheetx, rowx, colx)
	rst, v = pcall(__getcell, xlstable, sheetx, rowx, colx)
	if rst then return v
	else return nil
	end
end

function GetCellBySheetName(sheet, rowx, colx)
	return GetCell(sheetname[sheet], rowx, colx)
end

__XLS_END = true

return test[1]
