mkdir %outdir%\server\relay
mkdir %outdir%\server\settings_s\maps
mkdir %outdir%\server\script
mkdir %outdir%\server\settings_s\settings

mkdir %outdir%\client\clientpkg\maps
mkdir %outdir%\client\clientpkg\script
mkdir %outdir%\client\clientpkg\settings
mkdir %outdir%\client\clientpkg\ui

xcopy %outdir%\out\服务端配置文件\relay %outdir%\server\relay /e/y/r/k
xcopy %outdir%\out\服务端配置文件\gamesvr\maps %outdir%\server\settings_s\maps /e/y/r/k
xcopy %outdir%\out\服务端配置文件\gamesvr\script %outdir%\server\script /e/y/r/k
xcopy %outdir%\out\服务端配置文件\gamesvr\settings %outdir%\server\settings_s\settings /e/y/r/k

xcopy %outdir%\out\设定文件\script %outdir%\server\script /e/y/r/k
xcopy %outdir%\out\设定文件\settings %outdir%\server\settings_s\settings /e/y/r/k

xcopy %outdir%\out\客户端配置文件\maps %outdir%\client\clientpkg\maps /e/y/r/k
xcopy %outdir%\out\客户端配置文件\script %outdir%\client\clientpkg\script /e/y/r/k
xcopy %outdir%\out\客户端配置文件\settings %outdir%\client\clientpkg\settings /e/y/r/k
xcopy %outdir%\out\客户端配置文件\ui %outdir%\client\clientpkg\ui /e/y/r/k

xcopy %outdir%\out\设定文件\script %outdir%\client\clientpkg\script /e/y/r/k
xcopy %outdir%\out\设定文件\settings %outdir%\client\clientpkg\settings /e/y/r/k

PackFile P -P%outdir%\server\settings_s.pak -R%outdir%\server\settings_s\ -D.
PackFile P -P%outdir%\client\clientpkg.pak -R%outdir%\client\clientpkg -D.

path=%envpath%\gz;%path%
cd %outdir%\server
tar.gz.bat script
cd %envpath%