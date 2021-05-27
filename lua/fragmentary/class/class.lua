local _, _, majorv, minorv, rev = string.find(_VERSION, "(%d).(%d)[.]?([%d]?)")
local VersionNumber = tonumber(majorv) * 100 + tonumber(minorv) * 10 + (((string.len(rev) == 0) and 0) or tonumber(rev))




function setmt__gc(t, mt)
    if VersionNumber == 510 then
        local prox = newproxy(true);
        getmetatable(prox).__gc = function()
            mt.__gc(t);
        end
        t[prox] = true;
    end
    return setmetatable(t, mt);
end


--递归析构基类
function dtorclass(obj)
    if obj ~= nil then
        obj:dtor();
        if obj.superByName ~= nil then
            for k, v in pairs(obj.superByName) do
                dtorclass(v);
            end
        end
    end
end


function class(classname, ...)
    local cls = {};
    local superlist = {...};
    local supercount = table.getn(superlist);
    if supercount > 0  then --复制基类方法
        local clss_super_byname = {};
        if supercount == 1 then
            --单继承
            local super = superlist[1];
            for k,v in pairs(super) do cls[k] = v end;
            cls.super = super;
            clss_super_byname[super.__cname] = super;

        else
            --多继承
            local super = {};
            local i = 0;
            local supername = nil;
            for _,s in ipairs(superlist) do
                for k,v in pairs(s) do
                    cls[k] = v;
                    super[k] = v;
                end;

                if i == 0 then
                    supername = s.__cname;
                else
                    supername = supername .. ",".. s.__cname;
                end
                i= i + 1;

                clss_super_byname[s.__cname] = s;
            end
            super.__cname = supername;

            cls.super = super;
        end
        cls.superByName = clss_super_byname;
    else
        cls.super = {};
        cls = {ctor = function() end, dtor = function() end};
    end

    cls.__cname = classname;
    cls.__index = cls
    cls.__gc = function(t)
        dtorclass(t);
    end

    function cls:new(...)
        --实例化
        local newtable = {};
        local instance = setmt__gc(newtable, cls);

        instance.class = cls;
        instance:ctor(...);
        return instance;
    end

    return cls;
end



KVector = KVector or class("KVector")

function KVector:ctor()
    self.data = {};
end

function KVector:PushBack(value)
    table.insert(self.data, value);
end

function KVector:PopBack()
    if self:IsEmpty() then
        return nil;
    end

    local ln = #self.data;
    local value = self.data[ln];
    table.remove(self.data, ln);
    return value;
end

---@return boolean
function KVector:Insert(i, value)
    local p = self.data[i];
    table.insert(self.data, i, value);
    if i ~= 1 and p == nil then      --下标为1的位置是肯定能插入的，但如果不是1，这个位置也没东西，那就插不进去了
        LOGE("KVector:Insert - invalid index %d", i);
        return false;
    else
        return true;
    end
end

---@return boolean
function KVector:Remove(i)
    if table.remove(self.data, i) == nil then
        LOGE("KVector:Remove - invalid index %d", i);
        return false;
    else
        return true;
    end
end

function KVector:IsEmpty()
    if #self.data == 0 then
        return true;
    end
    return false;
end

function KVector:Set(i, v)
    if self.data[i] == nil then
        LOGE("KVector:Set - invalid index %d", i);
        return false;
    else
        self.data[i] = v;
        return true;
    end
end

function KVector:Get(i)
    return self.data[i];
end

---@return number
function KVector:Size()
    return #self.data;
end

function KVector:Sort(func)
    if func == nil then
        table.sort(self.data);
    else
        table.sort(self.data, func);
    end
end

function KVector:Clear()
    self.data = {};
end