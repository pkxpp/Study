--演示函数foo
function foo(...)
   local args={...}
   print("---lua code begin---")
   local s="foo("
   for i=1,#args do
       if i~=1 then s=s..",";end
       s=s..tostring(args[i])
   end
   s=s..")"
   print(s)--显示函数的参数
   print("---lua code end---")
   return "result val1!","result val2"--返回两个值
end
