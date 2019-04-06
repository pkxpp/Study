set srcdir=e:\Sword2\游戏运行资源\chib_tw
set outdir=e:\Sword2\游戏运行资源\chib_tw_

set CVSROOT=:pserver:wuwei1@192.168.22.47:/Scheme
set path=C:\Program Files\TortoiseCVS;%path%
set path=e:\temp\jau\mywork\jau\work_db;%path%
set path=C:\Program Files\Beyond Compare 2\;%path%
set path=E:\work\Packfile;%path%
set envpath=e:\temp\jau\mywork\jau\work_db\env\
rem set version=sword2_viet_2007_09_15_001
set version=sword2_t_ib_2008_07_10_100
set modpath=Sword2\游戏运行资源\中文繁体

mkdir %outdir%\src
mkdir %outdir%\out

copy ftp_tw ftp /y
