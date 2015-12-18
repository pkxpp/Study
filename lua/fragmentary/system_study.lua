--22.1 date&time
print(os.time())
print(os.time{year=2014,month=2,day=25,hour=13})
local tbTime = os.date("*t", 1393305214)
print(tbTime.wday, tbTime.min, tbTime.sec)
print(os.date("%x", 1393305214))


function getTimeFormat(nSecond)
	local nHour = math.floor(nSecond / 3600)
	local nLeft1 = nSecond % 3600
	local nMin = math.floor(nLeft1 / 60)
	local nSec = nLeft1 % 60
	
	local szString = string.format("%02d:%02d:%02d", nHour, nMin, nSec)
	print(szString);
end
getTimeFormat(10000)

