call cfg_viet_ch.bat
xcopy %srcdir% %outdir%\out /e/y/r/exclude:ignore_dir.conf
call pack.bat

cd %outdir%\
ftp -s:%envpath%\ftp_ch
cd %envpath%
