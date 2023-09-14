--第四篇CAPI
----------------------------
--26
--(1)internet
--【http://www.cnblogs.com/stephen-liu74/archive/2012/07/23/2469902.html】
--require "mystudylib"  --指定包名称

--在调用时，必须是package.function
--print(mystudylib.add(1.0,2.0))
--print(mystudylib.sub(20.1,19))

--(2)《Programming in Lua》
--mylib = loadlib("mylib", "luaopen_mylib")
--注：不行啊！
----------------------------
--27
--27.1
--[[
function Encode(nNumber)
	return nNumber+3
end
local tbTest = {1, 2, 3}
print(unpack(tbTest))
mystudylib.map(tbTest, Encode)
print(unpack(tbTest))
]]

--27.2 string
--[[
local tbSplit = mystudylib.split("hi,,there", ",")
print(tbSplit)
for _, v in pairs(tbSplit) do
	print(v)
end
]]
--[[
summary:从这里可以看到lua调用c函数，返回值是当前栈最上面的元素，所以你要返回什么样类型以及什么内容，你负责再调用该cpi之后，栈的最上面是你要返回的内容即可。
]]

--27.3 closure
--[[
local counter = mystudylib.newCounter()
for i=1, 5 do
	print(counter());
end
]]

------------------------------------------------------------
--28
--package.loadlib("mystudylib", "luaopen_mystudylib")()   --指定包名称
--print(mystudylib.add(1.0,2.0))
--另外一种调用c动态库的方式
--package.loadlib("mystudylib", "luaopen_mystudylib")()
--print(mystudylib.add(1.0,2.0))

--28.1
--package.loadlib("array", "luaopen_array")()
--报错：lua: capi_study.lua:57: attempt to call a nil value
--[[
解决：重新新建一个工程，名字叫做array，产生的dll即为array.dll。
总结：上面函数(package.loadlib)的第一个参数接收的字符串和dll名字要一样，可以去看下源代码，看如何解析的。
]]
--[[
a = array.new(1000)
print(a)
print(array.size(a))
for i=1, 1000 do 
	array.set(a, i, 1/i)
end
print(array.get(a, 10))
--success
]]

--28.2
--package.loadlib("array", "luaopen_array")()
--a = array.new(1000)
--print(a)
--print(array.size(a))

--28.3
--[[
a = array.new(1000)
print(a:size())
a:set(10, 3.4)
print(a:get(10))
--fail
]]
--[[
总结：
1.直接用28.2的dll是不行的，需要增加__index元方法
]]
--require "array"
--package.loadlib("array", "luaopen_array")()
--a = array.new(1000)
--print(a:size())
--a:set(10, 3.4)
--print(a:get(10))
--fail
--[[
总结：
1. 昨天通过去网上查资料搞清楚《programming in lua》中的luaL_openlib函数为什么没有了的内容，看到的一个原因大概是函数换了， 使用新的函数接口和使用方式去注册函数
2. 今天重新去官网查资料，看到自己的版本是5.1，直接在document中的5.1《reference manual》中的7.3中看到“Changes in  API”，看到了非常适合自己的解释：luaL_open接口不用了，修改成了luaL_register了，而且和luaL_openlib一样也支持第二个参数，package的名字为NULL，那么就是把当前第三个参数luaL_Reg中的函数注册到当前栈最上面的表中，所以如果这样使用的话，你自己要保证最上面的是一个table，并且是你想要的结果，阿门！
【http://www.lua.org/manual/5.1/manual.html#7.3】
3. a:size还是不行
]]
--[[
print(a.size)
print(getmetatable(a))
--print(a.size(a))
--print(a:size()) --lua: capi_study.lua:102: calling 'size' on bad self (luaBook.array expected, got userdata)
print(getmetatable(a).__index.size)
print(getmetatable(a).__index)
--print(getmetatable(a).__index.size())	--error as before
print(a:size())
]]
--fail

-- package.loadlib("array", "luaopen_array")()
-- a = array.new(1000)
-- print(a:size())
-- a:set(10, 3.4)
-- print(a:get(10))

--2014/03/24
--[[
总结：
1. 坑爹啊，终于知道原因是神马了！！！void *ud = luaL_checkudata(L, 1, "LuaBook.array");中的"LuaBook.array"写成了"luaBook.array"
]]

--------------------------------------------------------
--29.1
--linux
--[[
(1)luaL_Reg写成了luaL_reg提示： “错误：数组元素的类型不完全”
查资料：用gcc4编译时出现数组元素的类型不完全错误，这是因为gcc4不允许类型在声明前使用。 【http://blog.csdn.net/horsefaced/article/details/1678965】
	停下来去思考一下可以分析出来是在编译的时候没有发现这个结构体的定义，以为是新定义的，就会报这样的错。而这个结构体在前面的头文件中是定义了的，那说明你的结构体写错了！以后多这样思考分析！！！
解决：luaL_reg改成luaL_Reg即可
(2)linux下面还是使用luaL_openlib,使用luaL_register不成功
window下的lua的版本：5.1
linux下的lua版本：5.2.0这是要闹哪样？
----
手册上说这个函数被luaL_register取代，luaL_register在源码中的定义：
#define luaL_register(L,n,l) (luaL_openlib(L,(n),(l),0))
---
【http://blog.csdn.net/ym012/article/details/7188707】

(3)linux下面动态库编译
命令：gcc mylib.c -fPIC -shared -o libmylib.so

(4)报警告
mydir.c:18: 警告：传递参数 2 (属于 ‘lua_getmetatable’)时将指针赋给整数，未作类型转换
原因：代码有问题
* luaL_getmetatable写成了lua_getmetatable。少了一个大写的L
* lua_setmetatable写成了lus_setmetatable，这个竟然不报错，妹的

上面问题解决了，编译动态库文件mydir.so没有报错，ok

(5)linux下面使用动态库
*执行Lua进入交互模式
* package.loadlib("./libmydir.so", "luaopen_mydir")()--打开库
* for fname in mydir.mydir(".") do print(fname) end
注：
*这里没有直接用dir，因为这种事把dir注册成全局函数，而我还是使用上面的注册成一个库的形式
/*static const struct luaL_Reg mydir[] = {
        {"mydir", l_dir},
        {NULL, NULL},
};*/

//int luaopen_mydir (lua_State *L){
//      luaL_newmetatable(L, "LuaBook.dir");

        /* set its __Gc field */
//      lua_pushstring(L, "__gc");
//      lua_pushcfunction(L, dir_gc);
//      lua_settable(L, -3);

        /* register the 'dir' function */
//      lua_pushcfunction(L, l_dir);
//      lua_setglobal(L, "dir");
//      luaL_openlib(L, "mydir", mydir, 0);

//      return 0;
//}
*改成书上的形式，直接注册成全局函数也是可以ide
int luaopen_mydir (lua_State *L){
        luaL_newmetatable(L, "LuaBook.dir");

        /* set its __Gc field */
        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, dir_gc);
        lua_settable(L, -3);

        /* register the 'dir' function */
        lua_pushcfunction(L, l_dir);
        lua_setglobal(L, "dir");

        return 0;
}
]]
------------------------------------------------------------------------------------------------------
-- require "mystudylib"  --指定包名称
--[[ 报错：
        error loading module 'mystudylib' from file '.\mystudylib.dll':
        应用程序无法启动，因为应用程序的并行配置不正确。有关详细信息，请参阅应用程序事件日志，或使用命令行 sxstrace.exe 工具。
]]
-- package.loadlib("mystudylib", "luaopen_mystudylib")()
-- print(mystudylib.add(1.0,2.0))