local LIP = require 'LIP';

-- Data loading
local data = LIP.load('sample.ini');

print(data.sound.right); --> 80
print(data.screen.caption); --> Window's caption
print(data.screen.focused); --> true

print(111, ClassLIP)

local fIni = ClassLIP:new("sample.ini");
print(222, fIni)
local tbSound = fIni:getSection("sound") or {};
print(333, tbSound.right, type(tbSound.right))

local nRight = fIni:getNumber("sound", "right");
print("right = ", nRight)