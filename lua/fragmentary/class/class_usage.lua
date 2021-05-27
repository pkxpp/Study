
print(package.path);

-- require "script\\class\\class"
require "class"

--基类
---@class BaseClass
BaseClass = class("Baseclass", nil);
--构造函数
function BaseClass:ctor()
   print("BaseClass:ctor");
   self.name = "Base";
end


--析构函数
function BaseClass:dtor()

    if self.super ~=nil then
       print("super destory");
    end

	print("BaseClass:dtor");
end


function  BaseClass:GetName()
    print("BaseClass:GetName");
    return self.name;
end




--子类
---@class DerivedClass: BaseClass
DerivedClass = class("DerivedClass", BaseClass);

function DerivedClass:ctor()
    print("DerivedClass:ctor");
    self.name = "derived";
    self.age = 10;
end

function DerivedClass:dtor()  
    print("DerivedClass:dtor");  
end


function DerivedClass:GetName()
    print("DerivedClass:GetName");
    self.super:GetName();
    return self.name;
end

function DerivedClass:GetAge()
    return self.age;
end


--另一个类
---@class Worker
Worker = class("Worker");
function Worker:ctor()
    print("Worker:ctor")
    self.job = "码农";    
end
function Worker:dtor()
    print("Workd:dtor");
end

function Worker:Say()
    print("Worker:Say");
    print("happy coding");
end

function Worker:Build()
    print("work build");
end




--孙子类
---@class GrandSon: DerivedClass Worker
GrandSon = class("GrandSon", DerivedClass, Worker);
function GrandSon:ctor()
    print("GrandSon:ctor");
    self.name = "john";
end

function GrandSon:dtor()
    print("GrandSon:dtor");
end

function GrandSon:Say()
    print("GrandSon:Say");
    
    --多继承用这种可能会有问题，多个父类的同名方法存在覆盖关系，单继承没有问题
    self.super:Say();

    --多继承推荐用下面这个方法
    self.superByName["Worker"]:Say();
end


---@type BaseClass
base = BaseClass:new();
print(base:GetName());




collectgarbage("collect") 

--derived = DerivedClass:new();
--print(derived:GetName());

---@type GrandSon
grandson = GrandSon:new();
grandson:Say();

--grandson = nil;


print("================================");

local function gctest(self)
    print("cleaning up: ", self._name)
end

local test = {_name = "yangx"};
setmt__gc(test, {__gc = gctest})
test = nil;
collectgarbage("collect") -- 强制垃圾回收

a = 100;


---@alias Handeler fun(type:string, data:any):void