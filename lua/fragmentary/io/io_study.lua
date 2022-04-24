--
-- pk@ 2013-12-28
--

--------------------------------------------------------------
--open,read, write
function openfile(szFileName)
	
end

function read_file(szFileName)
	local pFile = nil
	if not pFile then
		pFile = io.open(szFileName, "r")
		if not pFile then
			DebugOutput("Open Test Log File Failed!")
			return;
		end
	end
	for line in pFile:lines() do
		print(line)
	end
	
	pFile:close();
end

--[[
local f = io.open("io_study.txt", "w")
if (not f) then
	print("Open File Failed!")
else
	print("Open File Successed!")
	f:write("中文滴")
	f:write("xxx")
	f:write(2, 5)
	f:write(123, tostring(unpack({12, 34, 7})))
	
	local a = 5
	local b = -1
	print(a+b)
end
f:close()

read_file("io_study.txt")
]]

------------------------------------------------------------
-- dir: 2017-05-09 18:59:58
-- dir
-- function DoTest( file )
	
-- end

-- function Reverse( path )
-- 	local ret = pl.dir(path)
-- 	if ret == "dir"
-- 		Reverse(ret)
-- 	else
-- 	 	DoTest(ret)
-- 	end
-- end

require "io"
-- require "lfs"
----------------------------------------------------------------------------------  
--It will return a table that contents all the file paths in the rootpath  
function recursive(rootpath, pathes)  
    pathes = pathes or {}  
    for entry in lfs.dir(rootpath) do  
        if entry ~= '.' and entry ~= '..' then  
            local path = rootpath .. '\\' .. entry  
            local attr = lfs.attributes(path)  
            assert(type(attr) == 'table')  
              
            if attr.mode == 'directory' then  
                getpathes(path, pathes)  
            else  
                table.insert(pathes, path)  
            end  
        end  
    end  
    return pathes  
end 

-------------------------------------------------------------
local width, height, area, perimeter

-- intro text and user inputs
-- print("Welcome to the Rectangle Area & Perimeter Calculator")

-- print("Enter Rectangle Width")
-- width = io.read()
-- if width <0 then 
-- 	print "Error: Please enter a positive value for width"
-- end

-- print("Enter Rectangle Height")
-- height = io.read()
-- if height <0 then 
-- 	print "Error: Please enter a positive value for height"
-- end 

-- --Calculator
-- area = width * height
-- print("The area of the rectangle is ", area)
-- perimeter = 2 * (width + height)
-- print("The perimeter of the rectangle is ", perimeter)

------------------------------------------------------------
--- ini.lua
-- local resty_ini = require "ini"
-- local conf, err = resty_ini.parse_file("/path/to/file.ini")

------------------------------------------------------------------------------------------------------
---
require "lfs"
-- local path = lfs.currentdir()
-- print(path)
-- print(package.path)
require ("io/fs")
require ("string/string")

function GetFilesByExt(szPath, szExt, bRecursive)
	if not FS.Path.IsDir(szPath) then
		return;
	end

	local tbFiles = {};
	local fnGetFiles = nil;
	fnGetFiles = function(szInPath, tbInFiles)
		local tbFiles = FS.ListDir(szInPath);
		for k, v in ipairs(tbFiles or {}) do
			local szFullName = szInPath .. "/" .. v;
			if bRecursive then
				if FS.Path.IsDir(szFullName) then
					fnGetFiles(szFullName, tbInFiles);
				end
			end
			-- print(k, szFullName, FS.Path.IsFile(szFullName))
			if FS.Path.IsFile(szFullName) then
				local szFileExt = FS.Path.SplitExt(szFullName);
				-- print(111, szFullName, szFileExt)
				if szFileExt == szExt then
					table.insert(tbInFiles, szFullName);
				end
			end
		end
	end

	fnGetFiles(szPath, tbFiles);

	return tbFiles;
end

-- local tbFiles = GetFilesByExt("f:/work/svn/MiniClient_Sim/EditorTool/MiniClient/tools/pakv4_tools/JX3Tool", ".list", true);
-- for k, v in ipairs(tbFiles or {}) do
-- 	print(k, v)
-- end

function CompareList(szPakDir, szList)
	if not FS.Path.IsDir(szPakDir) then
		return;
	end

	if not FS.Path.IsFile(szList) then
		return;
	end

	local tbList = {};
	local tbNotExist = {};
	-- 现在打全包生成的一个list里面的文件
	local file = FS.File(szList);
	local szLine = file:ReadNextLine();
	while(szLine) do
		tbList[szLine] = true;
	end

	-- 打包工具生产的list里面所有的文件
	local tbListFiles = GetFilesByExt(szPakDir, ".list", true);
	for k, v in pairs(tbListFiles) do
		local file = FS.File(szList);
		local szLine = file:ReadNextLine();
		while(szLine) do
			if not tbList[szLine] then
				table.insert(tbNotExist, szLine);
			end
		end
	end

	for i = 1, 10 do
		if not tbNotExist[i] then
			break;
		end

		-- print(tbNotExist[i]);
	end

end
CompareList("f:/work/svn/MiniClient_Sim/EditorTool/MiniClient/tools/pakv4_tools/JX3Tool", "F:/work/svn/MiniClient_Sim/EditorTool/MiniClient/tools/PakV4PlusMaker/OutPutNameList/common_2.txt");