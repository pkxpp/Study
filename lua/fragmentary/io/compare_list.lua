require "lfs"
-- 想比较某个文件不在另外一个文件的列表
-- 两个shaderlist，比如某次shader编译是12000个，另外一次是7000个，想看下这5000个是哪些？
function GetListFromFile(szFile)
    if not szFile then
        print("[Error]not file.")
        return;
    end
    local file = io.open(szFile, "r")
    if not file then
        print("[Error]can't opne file.")
        return;
    end

    local tbList = {};
    local nSize = 0;
    for line in file:lines() do
        if string.len(line) > 0 then
            -- print("line = ", line)
            nSize = nSize + 1;
            tbList[line] = true;
        end
    end
    file:close()

    return nSize, tbList;
end

function WriteListToFile(tbList, szOutFile, tbHash2RealName)
    if not tbList or not szOutFile then
        return;
    end
    local file = io.open(szOutFile, "w")
    if not file then
        return;
    end
    for k, v in ipairs(tbList) do
        local name = "{" .. v .. "-0000-000000000000}.fx";
        -- for k1, v1 in pairs(tbHash2RealName or {}) do
        --     if string.find(k1, v) then
        --         print(name == k1, string.len(name), string.len(k1), name, k1, v1)
        --         break;
        --     end
        -- end

        if tbHash2RealName and tbHash2RealName[name] then
            name = tbHash2RealName[name];
        end
        file:write(name .. "\t" .. v .. "\n")
    end
    file:close();
end

function GetHash2RealNameList(szDir)
    if not szDir then
        return;
    end
    local tbList = {};
    local nSize = 0;
    for entry in lfs.dir(szDir) do
        -- print("entry = ", entry)
        if entry~='.' and entry~='..' then
            local szFullName = szDir .. entry;
            local file = io.open(szFullName, "r");
            if file then
                local szContent = file:read(256);
                local p = string.find(szContent, ".fx5");
                local szRealName = string.sub(szContent, 1, p + string.len(".fx5"));
                -- print(szRealName);
                tbList[entry] = szRealName;
                nSize = nSize + 1;
                file:close();
            else
                print(entry)
            end
        end
    end
    return nSize, tbList;
end

function CompareList(szFile1, szFile2)
    local szDir = "h:\\svn\\BDCode\\sword3-products\\trunk\\client\\plugin\\ShaderCompiler\\";
    if not string.find(szFile1, ":") then
        szFile1 = szDir .. szFile1;
    end
    if not string.find(szFile2, ":") then
        szFile2 = szDir .. szFile2;
    end
    local nSize1, tbList1 = GetListFromFile(szFile1);
    local nSize2, tbList2 = GetListFromFile(szFile2);

    local nRealNameSize, tbHash2RealName = GetHash2RealNameList(szDir .. "Input\\");
    print("nRealNameSize = ", nRealNameSize)

    local tbList1NotIn2 = {};
    local tbList2NotIn1 = {};

    -- for k, v in pairs(tbList1 or {}) do
    --     if not tbList2[k] then
    --         table.insert(tbList1NotIn2, k);
    --     end
    -- end
    -- print("#tbList1NotIn2 = ", #tbList1NotIn2)
    -- WriteListToFile(tbList1NotIn2, "h:\\svn\\BDCode\\sword3-products\\trunk\\client\\plugin\\ShaderCompiler\\7000-diff.txt", tbHash2RealName);

    for k, v in pairs(tbList2 or {}) do
        if not tbList1[k] then
            table.insert(tbList2NotIn1, k);
            if k == "88be21e1-562f-1a16" then
                print("--------------------")
            end
        end
    end
    print("#tbList2NotIn1 = ", #tbList2NotIn1)

    local szOutFile = szDir .. "12000-diff.txt";
    WriteListToFile(tbList2NotIn1, szOutFile, tbHash2RealName);

end

CompareList("7000-list.txt", "12000-list.txt");