package.loadlib("dir", "luaopen_dir")()

for fname in dir(".") do print(fname) end
