set modname=服务端配置文件
cvs.exe "-q" "checkout" "-r" %version% "-d" %outdir%\src\%modname% "-P" %modpath%\%modname%
set modname=客户端配置文件
cvs.exe "-q" "checkout" "-r" %version% "-d" %outdir%\src\%modname% "-P" %modpath%\%modname%
set modname=设定文件
cvs.exe "-q" "checkout" "-r" %version% "-d" %outdir%\src\%modname% "-P" %modpath%\%modname%
