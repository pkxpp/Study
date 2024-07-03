require('LuaXML')
print("xml = ", xml)

-- 一些接口可以参考：https://github.com/n1tehawk/LuaXML/blob/master/unittest.lua

-- local xfile = xml.load("test.xml");
-- print(xfile)

function GetIncludeDependencies(szPath)
    local xfile = xml.load(szPath);
    if not xfile then
        return;
    end

    local tbHeadNodes = {};
    local tbLinksSource = {};
    local tbLinksTarget = {};
    local tbName2ID = {};
    local tbNodes = xfile:find("Nodes");
    -- print(type(tbNodes))
    for k, v in pairs(tbNodes or {}) do
        if k == 1 then
            for k1, v1 in pairs(v) do
                print (k1, v1);
            end
        end
        if type(v.FilePath) == "string" and string.find(v.FilePath, "%.h") then
            -- print(v)
            -- print(v.Id, string.find(v.FilePath, "%.h"), v.FilePath);
            tbHeadNodes[v.Id] = v;
            local szFileName = string.match(v.FilePath, ".*\\([^\\]*%.%w+)$");
            -- print(szFileName)
            tbName2ID[szFileName] = v.Id;
        end
    end

    ------------------------------------------------------------------------------------------------------
    --[[
        {
            Source
            Target
        }
        * 表示Source引用了Target，比如：
            <Link Source="@16948" Target="@11839" Category="PoundInclude" />
            <Link Source="@16948" Target="@13345" Category="PoundInclude" />
            <Link Source="@16948" Target="@13537" Category="PoundInclude" />
            <Link Source="@16948" Target="@15646" Category="PoundInclude" />
            <Link Source="@16948" Target="@1750" Category="PoundInclude" />
            <Link Source="@16948" Target="@3586" Category="PoundInclude" />
            <Link Source="@16948" Target="@5731" Category="PoundInclude" />
            <Link Source="@16948" Target="@5978" Category="PoundInclude" />
            <Link Source="@16948" Target="@8899" Category="PoundInclude" />
        表示@16948(PrimitiveSceneProxy.h)引用了Target这些文件
            #include "CoreMinimal.h"
            #include "Stats/Stats.h"
            #include "Misc/MemStack.h"
            #include "PrimitiveViewRelevance.h"
            #include "SceneTypes.h"
            #include "Engine/Scene.h"
            #include "UniformBuffer.h"
            #include "SceneView.h"
            #include "PrimitiveUniformShaderParameters.h"
    ]]
    ------------------------------------------------------------------------------------------------------
    local tbLinks = xfile:find("Links");
    for k, v in pairs(tbLinks or {}) do
        if k == 1 then
            for k1, v1 in pairs(v) do
                print(k1, v1)
            end
        end
        if tbHeadNodes[v.Source] then
            if not tbLinksSource[v.Source] then
                tbLinksSource[v.Source] = {};
            end
            tbLinksSource[v.Source][v.Target] = v;

            if not tbLinksTarget[v.Target] then
                tbLinksTarget[v.Target] = {};
            end
            table.insert(tbLinksTarget[v.Target], v.Source);
        end
    end
    return tbHeadNodes, tbLinksSource, tbLinksTarget, tbName2ID;
end
-- GetIncludeDependencies("test.xml");

function GetIncludePaht(szFilePath, szSource, szTarget)
    if not szSource or not szTarget then
        return;
    end

    local tbHeadNodes, tbHeadLinks, tbLinksTarget, tbName2ID = GetIncludeDependencies(szFilePath);
    print(tbName2ID[szSource]);
    print(tbName2ID[szTarget]);
    local tbSource = tbHeadNodes[tbName2ID[szSource]];
    local tbTarget = tbHeadNodes[tbName2ID[szTarget]];
    if not tbSource or not tbTarget then
        return;
    end
    print(tbSource.FilePath)
    print(tbTarget.FilePath)
    print("Source---------------------")
    for k, v in pairs(tbHeadLinks[tbName2ID[szSource]] or {}) do
        local node = tbHeadNodes[v.Target];
        print(node.FilePath);
    end

    print("Targetn--------------------")
    for k, v in pairs(tbLinksTarget[tbName2ID[szSource]] or {}) do
        local node = tbHeadNodes[v];
        print(node.FilePath);
    end
end

-- GetIncludePaht("MeshPassProcessor.xml", "MeshPassProcessor.h", "PrimitiveSceneProxy.h");
GetIncludePaht("test.xml", "PrimitiveSceneProxy.h", "MeshPassProcessor.h");