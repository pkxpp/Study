-- lua test.lua 2 5
-- arg[0]= lua_execute_with_param.lua 
-- arg[1]= 2
-- arg[2]= 5

print(#arg)
if arg[1] and arg[1] == "2" then
	print("arg[1]=2")
end
