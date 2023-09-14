-----------------------------------------------------------------------------
--脚本文件：shader_assembly.lua
--功能设计：
--功能开发：page(pk)
--开发时间：2023/05/26 09:51:27
--脚本功能：分析shader机器码
--修改记录：
-----------------------------------------------------------------------------
require ("io/fs")
ShaderAssembly = {
};

local ShaderKeyword = {
    ["BRANCH"] = true,
    ["return"] = true,
    ["#elif"] = true,
    ["elif"] = true,
    ["else"] = true,
}

function ShaderAssembly:_ParseShaderCode(szShaderPath, nType)
    local file = FS.Open(szShaderPath, "r");
    -- print(file);
    if not file then
        print("[Warning]no this file.", szShaderPath);
        return;
    end
    local tbLines = {};
    local tbFunctions = {
        --[name] = {};
    };
    local nLine = 0;
    file:Seek(0);
    local strLine = file:ReadNextLine();
    local szCurFunction = nil;
    local tbBrace = {};
    while strLine do
        table.insert(tbLines, strLine);
        nLine = nLine + 1;
        -- 去掉空格
        local szLine = (string.gsub(strLine, "^%s*(.-)%s*$", "%1"));
        local szReturn, szFuncName = self:_GetShaderFunctionName(szLine, nType);
        if szFuncName then
            -- print("Name = ", nLine, szReturn, szFuncName);
            szCurFunction = szFuncName;
            -- for k, v in ipairs(tbBrace) do
            --     print("\t", v)
            -- end
            tbBrace = {};
            tbFunctions[szFuncName] = {}
            table.insert(tbFunctions[szFuncName], nLine);
        end
        if self:_IsValidLeftBrace(szLine) then
            table.insert(tbBrace, nLine);
        end
        if self:_IsValidRightBrace(szLine) then
            table.remove(tbBrace);

            if szCurFunction and #tbBrace <= 0 then
                -- print("Function end.", nLine, szCurFunction, tbFunctions[szCurFunction]);
                if tbFunctions[szCurFunction] then
                    table.insert(tbFunctions[szCurFunction], nLine);
                end
                szCurFunction = nil;
            end
        end
        
        strLine = file:ReadNextLine();
    end
    file:Close();

    -- print("Line = ", nLine);
    return tbFunctions, tbLines;
end

function ShaderAssembly:_GetShaderFunctionName(szLine, nType)
    if not szLine then
        return;
    end
    
    local nStartPos, nEndPos = string.find(szLine, "//");
    if nStartPos == 1 and nEndPos == 2 then
        return;
    end
    
    local szPattern = "([%w_]+)%s+([%w_]+)%s*%(";
    if nType == 2 then
        szPattern = "([%w_]+)%s+([%w_]+)%s*=";
    end
    local szReturn, szName = string.match(szLine, szPattern);
    if ShaderKeyword[szReturn] then
        return;
    end
    if nType == 2 and szName then
        local nStartPos, nEndPos = string.find(szLine, "=");
		local nNextPos = string.find(szLine, "%w", nStartPos + 1);
		if nNextPos then
			return;
		end
    end
    return szReturn, szName;
end

function ShaderAssembly:_IsValidLeftBrace(szLine)
    local nStartPos = string.find(szLine, "{");
    if not nStartPos then
        return false;
    end
    -- 可能在注释里面
    return true;
end

function ShaderAssembly:_IsValidRightBrace(szLine)
    local nStartPos = string.find(szLine, "}");
    if not nStartPos then
        return false;
    end
    return true;
end

function ShaderAssembly:_IsAllLineAnnotation(szLine)
    local nStartPos, nEndPos = string.find(szLine, "//");
    if nStartPos == 1 and nEndPos == 2 then
        return true;
    end
    return false;
end

function ShaderAssembly:_GetShaderPathFromAssembly(szLine)
    if not szLine then
        return;
    end
    local szPattern = "%s*#line%s+([%d]+)%s+\"([%w\\/:%.-_]+)\"";
    local szLineNum, szPath = string.match(szLine, szPattern);
	return szLineNum, szPath;
end

function ShaderAssembly:_ParseAssemblyLine(szLine)
    -- 空的
    if not szLine or string.len(szLine) <= 0 then
        return;
    end

    -- 注释
    if self:_IsAllLineAnnotation(szLine) then
        -- print(szLine);
        return;
    end
    
    -- #line 开头
    local szPattern = "%s*#line%s+([%d]+)%s*";
    local szLineNum = string.match(szLine, szPattern);
    if szLineNum then
        -- 带路径的形式
        -- #line 196 "h:\svn\BDCode\sword3-products\trunk\client\data\material\Shader_DX11_HD\Voxelization.h5"
        local szLineNumber, szShaderPath = self:_GetShaderPathFromAssembly(szLine);
        local nLine = tonumber(szLineNumber);
        if nLine and szShaderPath then
            -- print(nLine, szShaderPath);
            return nLine, szShaderPath;
        end

        -- 不带路径的形式
        -- #line 200
        nLine = tonumber(szLineNum);
        if nLine then
            return nLine;
        end
    end
    
    -- 剩下的都是指令了
    return szLine;
end

function ShaderAssembly:_ParseAssemblyCode(tbFunctions, tbLines, tbFunctionMappingNames)
    local tbShaderFunctions = {
        --[[
            [szPath] = {
                [szFuncName] = {nStartLine, nEndLine}
            }
        ]]
    }
    for szFuncName, tbInfo in pairs(tbFunctions or {}) do
        local szShaderFunctionName = tbFunctionMappingNames[szFuncName];
        if not szShaderFunctionName then
            print("[Warning]no mapping name......................................................");
        end
        local nStart, nEnd = unpack(tbInfo);
        local nInstruction = 0;
        print(szFuncName, nStart, nEnd, szFuncName == "MainVoxelizationWithGbufferGeometryShader");
        local szCallFunction = nil;
        local szShaderName = nil;
        local nCallInstruction = 0;
        for i = nStart, nEnd do
            -- 去掉空格
            local szLine = (string.gsub(tbLines[i], "^%s*(.-)%s*$", "%1"));
            local nLine, szShaderPath = self:_ParseAssemblyLine(szLine);
            
            if szShaderPath then
                if not tbShaderFunctions[szShaderPath] then
                    local tbSubFunctions = ShaderAssembly:_ParseShaderCode(szShaderPath, 1);
                    tbShaderFunctions[szShaderPath] = tbSubFunctions;
                end
                
                for k, v in pairs(tbShaderFunctions[szShaderPath] or {}) do
                    local nCallStart, nCallEnd = unpack(v);
                    if nCallStart and nCallEnd and nLine >= nCallStart and nLine <= nCallEnd then
                        print("\t", szCallFunction, nCallInstruction);
                        print("\t", k, szShaderPath, nLine)
                        szCallFunction = k;
                        nCallInstruction = 0;
                        break;
                    end
                end
            end

            if nLine == szLine then
                nInstruction = nInstruction + 1;
                nCallInstruction = nCallInstruction + 1;
            end
        end
        print("all instruction = ", nInstruction, szFuncName);
    end
end

function ShaderAssembly:_GetAssemblyFunctionNameToShaderFunctionName(szShaderPath)
    local file = FS.Open(szShaderPath, "r");
    if not file then
        print("[Warning]no this file.", szShaderPath);
        return;
    end

    local tbFunctionNames = {
        --[szAssemblyFunctionName] = szShaderFunctionName;
    };

    file:Seek(0);
    local strLine = file:ReadNextLine();
    local szPattern = "[%w]+%s+([%w_]+)%s*=%s*KG3D_CompilePS%(([%w_]+)(%([%w_/%*,%s]*%))%)";
    while strLine do
        -- 去掉空格
        local szLine = (string.gsub(strLine, "^%s*(.-)%s*$", "%1"));
        local szAssemblyFunctionName, szShaderFunctionName = string.match(szLine, szPattern);
        if szAssemblyFunctionName and szShaderFunctionName then
            tbFunctionNames[szAssemblyFunctionName] = szShaderFunctionName;
        end
        strLine = file:ReadNextLine();
    end
    file:Close();

    return tbFunctionNames;
end

function TestFunction()
    local tbFunctionMappingNames = {};
    local tbCompileNamesShaderPath = {
        "H:\\svn\\BDCode\\sword3-products\\trunk\\client\\data\\material\\Shader_DX11_HD\\MeshSharedTechnique.h",
    }
    for k, v in pairs(tbCompileNamesShaderPath) do
        local tbNames = ShaderAssembly:_GetAssemblyFunctionNameToShaderFunctionName(v);
        for name1, name2 in pairs(tbNames or {}) do
            tbFunctionMappingNames[name1] = name2;
        end
    end
    for k, v in pairs(tbFunctionMappingNames) do
        print(k, "--->", v)
    end
    
    local szShaderPath = "h:\\svn\\BDCode\\sword3-products\\trunk\\client\\data\\material\\Shader_DX11_HD\\ModelBasePixelShader.fx5"
    -- local tbFunctions = ShaderAssembly:_ParseShaderCode(szShaderPath, 1);
    local szAssemblyPath = "F:\\study\\lua\\fragmentary\\mtl_sss_face_debug1.fxo";
    local tbFunctions, tbAllLines = ShaderAssembly:_ParseShaderCode(szAssemblyPath, 2);
    print("Lines = ", #tbAllLines)
    print("-------------------------------------")
    -- for k, v in pairs(tbFunctions) do
    --     print(k);
    -- end
    ShaderAssembly:_ParseAssemblyCode(tbFunctions, tbAllLines, tbFunctionMappingNames);
end

------------------------------------------------------------------------------------------------------
TestFunction();