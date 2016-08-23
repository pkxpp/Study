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
