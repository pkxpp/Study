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
	f:write("ÖÐÎÄµÎ")
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
local resty_ini = require "ini"
local conf, err = resty_ini.parse_file("/path/to/file.ini")