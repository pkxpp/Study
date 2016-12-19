set v=tw
if %v% == tw call cfg_tw.bat
if %v% == viet call cfg_viet.bat
if %v% == viet_ch call cfg_viet_ch.bat

rem call cfg.bat
rem call cvslogin.bat

call checkout.bat
call cp.bat
call translate.bat
BC2.exe %outdir%\src %outdir%\out /filters=-cvs\
call pack.bat

cd %envpath%
call upload.bat
