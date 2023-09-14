require ("io/fs")

local function __load__(script, key, from)
    local mt = {__index = _G};
    -- local refenv = __loaders__[key];
    -- local loader = refenv and refenv.__loader__ or {
    --     script = script,
    --     from = from and from.__loader__ or {},
    --     loadcount = 0,
    --     children = {},
    -- }; --- 这里的from一第一次import时的为准，后面的都不再修改
    local tab = { __loader__ = loader }; --- 环境还是要重置的
    if from and from.__loader__ then
        table.insert(from.__loader__.children, tab.__loader__);
    end
    setmetatable(tab, mt);
    -- __loaders__[key] = nil;
    -- __loaders__[key] = tab;
    local file = FS.Open(script, "r");
    if not file then
        print("not file: ", script);
        return;
    end

    local text = file:Read();
    if type(text) ~= "string" then
        print(string.format(
            "[loader] Import <%s> failed. Type <string> required, got <%s> instead.",
            script,
            type(text)
        ));
        return nil;
    end
    
    local fn, err = loadstring(text, script);
    if not fn then
        print(string.format(
            "[loader] Import <%s> but loadstring failed, raw msg is: %s", script, err
        ));
        return nil;
    end

    setfenv(fn, tab);
    fn();
    return tab;
end

function ImportScript(script)
    -- script = __pathnorm__(script);
    -- local key = "FILE:" .. script;
    -- local refenv = __loaders__[key];
    -- if not refenv then
    --     refenv = __load__(script, key, getfenv(2));
    --     if refenv and type(refenv.__onload__) == "function" then
    --         xpcall(refenv.__onload__, Traceback, refenv);
    --     end
    -- end
    -- __loader_ref_add__(refenv.__loader__);
    local refenv = __load__(script)
    return refenv;
end