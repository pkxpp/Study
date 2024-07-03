-----------------------------------------------------------------------------
--�ű��ļ���shader_assembly.lua
--������ƣ�
--���ܿ�����page(pk)
--����ʱ�䣺2023/05/26 09:51:27
--�ű����ܣ�����shader������
--�޸ļ�¼��
-----------------------------------------------------------------------------
require ("class/class")
require ("io/fs")
require('LuaXML')
print("xml = ", xml)

---@class ShaderFunction
ShaderFunction = class("ShaderFunction", nil);
--���캯��
function ShaderFunction:ctor(name)
    self.Name = name or "";
    self.Range = {0, 0};                --���ļ��еĺ��� {StartLine, EndLine}
    self.InstructionNum = 0;        --��Чָ������ȥ���������һ��
    self.ShaderPath = nil;
    self.LineInShaderFile = -1;
    -- self.CalledNum = 0;             --���������õĴ���
    -- self.Callers = {};              --���øú����ĺ���
    -- self.CallerNum = 0;             --���øú����ĺ�������
    self.Callers = {};
    self.Callees = {};
end

 --��������
function ShaderFunction:dtor()
end

------------------------------------------------------------------------------------------------------
---@class ShaderFunction
ShaderCallTree = class("ShaderCallTree", nil);
--���캯��
function ShaderCallTree:ctor()
    self.Callers = {};
    self.Callees = {};
end

 --��������
 function ShaderCallTree:dtor()
 end
 
------------------------------------------------------------------------------------------------------
---@class ShaderAssembly
ShaderAssembly = class("ShaderAssembly", nil);
--���캯��
function ShaderAssembly:ctor()
    self.Name = "";
    self.InstructionNum = 0;
end

 --��������
function ShaderAssembly:dtor()
end

------------------------------------------------------------------------------------------------------
---@class ShaderCallGraph
ShaderCallGraph = class("ShaderCallGraph", nil);
--���캯��
function ShaderCallGraph:ctor(xmlFile, xmlDir)
    -- �ο���https://github.com/n1tehawk/LuaXML/blob/master/unittest.lua
    local xfile = xml.load(xmlFile);
    if not xfile then
        return;
    end
    self.CallGraphs = {};
    -- print("xfile = ", xfile);
    
    self:_ParseCallGraphFromFile(xmlFile);
    -- local tbNodes = xfile:find("compounddef");
    -- print(type(tbNodes))
    -- for k, v in pairs(tbNodes) do
    --     -- print(k, v.kind);
    --     if v.kind == "func" then
    --         print(v:tag())
    --         -- ������Ա����(shader�ļ��е����к���)
    --         for _, tbMemberFunctions in pairs(v) do
    --             if type(tbMemberFunctions) == "table" then
    --                 if tbMemberFunctions:tag() == "memberdef" then
    --                     -- print(tbMemberFunctions:tag(), tbMemberFunctions.name);
    --                     local szFuncName = nil;
    --                     for _, node in ipairs(tbMemberFunctions) do
    --                         if node:tag() == "name" then
    --                             print(node[1])
    --                             szFuncName = node[1];
    --                             self.CallGraphs[szFuncName] = {
    --                                 Callees = {
    --                                     --[[
    --                                         Name = "",
    --                                         FilePath = "",
    --                                     ]]
                                        
    --                                 }
    --                             };
    --                         end

    --                         -- ��Ա��������
    --                         if node:tag() == "references" then
    --                             print("\t", node[1], node.compoundref)
    --                             table.insert(self.CallGraphs[szFuncName].Callees, {
    --                                 Name = node[1],
    --                                 FilePath = node.compoundref,
    --                             });
    --                         end
    --                     end
    --                 end
    --             else
    --                 -- ����һ����Ա��tag: sectiondef
    --                 print(tbMemberFunctions);
    --             end
    --         end
    --     end
    -- end
    -- local tbNodes = xfile:find("references");
    -- print(type(tbNodes))
    -- for k, v in pairs(tbNodes) do
    --     print(k, v);
    -- end

    -- local tbNodes1 = xfile:find("invincdepgraph");
    -- print(type(tbNodes1))
    -- for k, v in pairs(tbNodes1) do
    --     -- print(k, v.id);
    -- end

    -- local tbNodes = xfile:find("invincdepgraph");
    -- print(type(tbNodes))
    -- for k, v in pairs(tbNodes) do
    --     -- print(k, v.id);
    -- end
end

function ShaderCallGraph:_ParseCallGraphFromFile(xmlFile)
    self.CallGraphs = {};
    local xfile = xml.load(xmlFile);
    if not xfile then
        return;
    end
 
    --[[
        -- �������� ������
        
      <sectiondef kind="func">
        <memberdef kind="function" id="_model_base_pixel_shader_8fx5_1a0f73107adea9e29c59186f6061a10e46" prot="public" static="no" const="no" explicit="no" inline="no" virt="non-virtual">
            <type>void</type>
            <definition>void CalculateAlphaInNoDiscardPixel</definition>
            <argsstring>(in out float Alpha)</argsstring>
            <name>CalculateAlphaInNoDiscardPixel</name>
            <param>
            <type>in out float</type>
            <declname>Alpha</declname>
            </param>
            <briefdescription>
            </briefdescription>
            <detaileddescription>
            </detaileddescription>
            <inbodydescription>
            </inbodydescription>
            <location file="ModelBasePixelShader.fx5" line="14" column="6" bodyfile="ModelBasePixelShader.fx5" bodystart="14" bodyend="18"/>
            <references refid="_common_param_8h_1aed3392d7088a901de463db2f34639455" compoundref="_common_param_8h" startline="752">g_bMatEnableBlend</references>
            <referencedby refid="_model_base_pixel_shader_8fx5_1acfcca820ceb48d0f45e973572b525dae" compoundref="_model_base_pixel_shader_8fx5" startline="25" endline="225">PixelShaderMain</referencedby>
        </memberdef>
      </sectiondef>
    ]]
    local tbNodes = xfile:find("compounddef");
    -- print(type(tbNodes))
    for k, v in pairs(tbNodes) do
        if v.kind == "func" then
            -- print(v:tag())
            -- ������Ա����(shader�ļ��е����к���)
            for _, tbMemberFunctions in pairs(v) do
                local fnMember = self:_ParseMemberFunction(tbMemberFunctions);
                -- if type(tbMemberFunctions) == "table" then
                --     if tbMemberFunctions:tag() == "memberdef" then
                --         -- print(tbMemberFunctions:tag(), tbMemberFunctions.name);
                --         local szFuncName = nil;
                --         for _, node in ipairs(tbMemberFunctions) do
                --             if node:tag() == "name" then
                --                 print(node[1])
                --                 szFuncName = node[1];
                --                 self.CallGraphs[szFuncName] = {
                --                     Callees = {
                --                         --[[
                --                             Name = "",
                --                             FilePath = "",
                --                         ]]
                                        
                --                     }
                --                 };
                --             end

                --             -- ��Ա��������
                --             if node:tag() == "references" then
                --                 print("\t", node[1], node.compoundref)
                --                 table.insert(self.CallGraphs[szFuncName].Callees, {
                --                     Name = node[1],
                --                     FilePath = node.compoundref,
                --                 });
                --             end
                --         end
                --     end
                -- else
                --     -- ����һ����Ա��tag: sectiondef
                --     print(tbMemberFunctions);
                -- end
            end
        end
    end
end


function ShaderCallGraph:_ParseMemberFunction(member)
    if not member then
        return;
    end

    -- ����һ����Ա��tag: sectiondef
    if type(member) ~= "table" then
        return;
    end

    if member:tag() ~= "memberdef" then
        return;
    end

    local funcMember = ShaderFunction:new();
    -- print(tbMemberFunctions:tag(), tbMemberFunctions.name);
    -- local szFuncName = nil;
    for _, node in ipairs(member) do
        if node:tag() == "name" then
            -- print(node[1])
            -- szFuncName = node[1];
            -- self.CallGraphs[szFuncName] = {
            --     Callees = {
            --         --[[
            --             Name = "",
            --             FilePath = "",
            --         ]]
                    
            --     }
            -- };
            funcMember.Name = node[1];
            self.CallGraphs[funcMember.Name] = funcMember;
        end

        -- ��Ա��������
        if node:tag() == "references" then
            -- print("\t", node[1], node.compoundref)
            local func = ShaderFunction:new(node[1]);
            table.insert(funcMember.Callees, func);
        end
    end
    return funcMember;
end

 --��������
function ShaderCallGraph:dtor()
end

local CallGraph = ShaderCallGraph:new("tools\\shader_parser\\xml\\_model_base_pixel_shader_8fx5.xml", "tools\\shader_parser\\xml\\");

------------------------------------------------------------------------------------------------------
ShaderAssemblyParser = {
    ---@type string[]
    Lines = {};

    ---@type table<string, {}>
    Functions = {
        --[[
            [function name] = {
                InstructionNum = 0,
                Lines = {������ʼ��������������������������}
            }
        ]]
    };

    ---@type table<string, table<string, ShaderFunction>>
    ShaderFunctions = {
        --[[
            [ShaderPath] = {
                [Function Name] = ShaderFunction
            }
        ]]
    };
};

local ShaderKeyword = {
    ["BRANCH"] = true,
    ["return"] = true,
    ["#elif"] = true,
    ["elif"] = true,
    ["else"] = true,
    ["UNROLL"] = true,
    ["define"] = true,
    ["if"] = true,
}

local EnumFileType = {
    EFT_SHADER_FILE = 1,
    EFT_ASSEMBLE_FILE = 2,
};

local FUNCTION_PATTERN_SHADER = "([%w_]+)%s+([%w_]+)%s*%(";
local FUNCTION_PATTERN_ASSEMBLY = "([%w_]+)%s+([%w_]+)%s*=";


---@function: ����һ��Assembly���ļ����������ʽ������
---@param szPath string �ļ�·��
function ShaderAssemblyParser:_Load(szShaderPath)
    self.Lines = {};
    self.Functions = {};
    local file = FS.Open(szShaderPath, "r");

    if not file then
        print("[Warning]no this file.", szShaderPath);
        return;
    end

    local nLine = 0;
    file:Seek(0);
    local strLine = file:ReadNextLine();
    local tbBraceStack = {};
    local szCurFunction = nil;  -- Assembley�еĺ���
    local nInstructionNum = 0;
    -- local tbCallFunctionStack = {};
    local tbCallShaderFunctions = {};
    while strLine do
        -- ȥ���ո�
        local szLine = (string.gsub(strLine, "^%s*(.-)%s*$", "%1"));
        if szLine and szCurFunction then
            nInstructionNum = self:_GetInstructionNum(szLine);
            if nInstructionNum > 0 then
                self.Functions[szCurFunction].InstructionNum = nInstructionNum;
            end
        end
        local tbLine = self:_ParseLine(szLine);
        if tbLine then
            table.insert(self.Lines, szLine);
            nLine = nLine + 1;

            if tbLine.FuncName then
                szCurFunction = tbLine.FuncName;
                ---@type ShaderFunction
                local func = ShaderFunction:new();
                func.Range[1] = nLine;
                self.Functions[szCurFunction] = func;
                -- {
                --     InstructionNum = 0,
                --     -- ������ʼ��
                --     Lines = {nLine},
                --     Callees = {},
                -- };
                -- tbCallTree[szCurFunction] = {
                --     Callee = {},
                --     Tree = {},
                -- };
                -- tbCallFunctionStack = {};
                tbCallShaderFunctions[szCurFunction] = {};
            end

            if tbLine.bLBrace  then
                table.insert(tbBraceStack, nLine);
            end

            if tbLine.bRBrace then
                table.remove(tbBraceStack);

                if szCurFunction and #tbBraceStack <= 0 then
                    -- print("Function end.", nLine, szCurFunction);
                    -- ����������
                    if self.Functions[szCurFunction] then
                        self.Functions[szCurFunction].Range[2] = nLine;
                    end
                    szCurFunction = nil;
                end
            end
 
            if szCurFunction then
                if self.Functions[szCurFunction] then
                    -- ����ջ������һ��
                    -- local szCurCallFunction = tbCallFunctionStack[#tbCallFunctionStack];
                    -- if tbLine.ShaderPath then
                    --     self.Functions[szCurFunction].ShaderPath = tbLine.ShaderPath;
                    -- end
                    if tbLine.CallFunctionName then
                        print("==>", tbLine.CallFunctionName, tbLine.ShaderPath);
                        local func = ShaderFunction:new(tbLine.CallFunctionName);
                        func.ShaderPath = tbLine.ShaderPath;
                        func.Range = tbLine.Func.Range;
                        func.LineInShaderFile = tbLine.LineInShaderFile;
                        table.insert(tbCallShaderFunctions[szCurFunction], func);
                        table.insert(self.Functions[szCurFunction].Callees, func);
                    end
                    if tbLine.bInstruction then
                        -- �������
                        local func = tbCallShaderFunctions[szCurFunction][#tbCallShaderFunctions[szCurFunction]];
                        if func then
                            func.InstructionNum = func.InstructionNum + 1;
                        end
                    end
                else
                    print("[Error]No current function name data. ", szCurFunction)
                end
            end
        end

        strLine = file:ReadNextLine();
    end
    file:Close();

    -- self:_UpdateShaderFunction(tbCallShaderFunctions);
end

function ShaderAssemblyParser:_ParseLine(szLine)
    local nLine, szShaderPath = self:_GetShaderPathFromLine(szLine);
    if not nLine then
        return;
    end
    local tbLine = {
        bLBrace = false,
        bRBrace = false,
        FuncName = nil,
        ShaderPath = szShaderPath,
        CallFunctionName = nil,
        CallFuncRange = {0, 0},
        CallInstructionNum = 0,
        bInstruction = true,
        LineInShaderFile = nLine,
    };

    tbLine.bLBrace = self:_IsValidLeftBrace(szLine);
    tbLine.bRBrace = self:_IsValidRightBrace(szLine);
    local szReturn, szFuncName = self:_GetShaderFunctionName(szLine, EnumFileType.EFT_ASSEMBLE_FILE);
    tbLine.FuncName = szFuncName;

    if type(nLine) == "number" then
        tbLine.bInstruction = false;
    end
    --[[
        �����������η���
        #line 3081 "E:\work\svn\code\Sword3\Source\KG3DEngineDX11\KG3DEngineE\Internal\Module\KG3D_MaterialSystem\Templates\CommonParam.h"
          mad r0.xyz, r3.xyzx, l(1.414214, 1.414214, 1.414214, 0.000000), l(-0.004000, -0.004000, -0.004000, 0.000000)
          max r0.xyz, r0.xyzx, l(0.000000, 0.000000, 0.000000, 0.000000)  // r0.x <- outColor.x; r0.y <- outColor.y; r0.z <- outColor.z
        
        #line 3082
    ]]
    -- ����Shader�ļ�����
    if szShaderPath then
        -- print("Path = ", szShaderPath);
        if not self.ShaderFunctions[szShaderPath] then
            ---@type ShaderFunction
            local tbFunctions = ShaderAssemblyParser:_ParseShaderCode(szShaderPath, EnumFileType.EFT_SHADER_FILE);
            self.ShaderFunctions[szShaderPath] = tbFunctions;
        end
        
        for k, v in pairs(self.ShaderFunctions[szShaderPath] or {}) do
            -- print(k, v)
            local nStartLine, nEndLine = unpack(v.Range);
            if nStartLine and nEndLine and nLine >= nStartLine and nLine <= nEndLine then
                -- print("\t", szCallFunction, nCallInstruction);
                -- print("\t", k, nLine, nStartLine, nEndLine, szShaderPath)
                -- szCallFunction = k;
                -- nCallInstruction = 0;
                tbLine.CallFunctionName = k;
                tbLine.Func = v;
                break;
            end
        end
    end

    return tbLine;
end

---@param fnAssemble ShaderFunction Assembley�ļ��еĺ���
---@param tbLine table
function ShaderAssemblyParser:_UpdateShaderFunction(tbCallShaderFunctions, fnAssemble, tbLine, tbCallFunctionStack)
    -- if not tbLine or not fnAssemble then
    --     return;
    -- end
    if not tbCallShaderFunctions then
        return;
    end
    local tbCallTree = {};
    for szAssembleyFunc, tbCallShaderFuncs in pairs(tbCallShaderFunctions) do
        local tbCallFunctionStack = {};
        tbCallTree[szAssembleyFunc] = {
            Callees = {},
            Functions = {
                --[Name] = ShaderFunction
            },
        };
        local tbFunctions = tbCallTree.Functions;
        for _, fnShader in ipairs(tbCallShaderFuncs) do
            table.insert(tbCallFunctionStack, fnShader.Name);
            if not tbFunctions[fnShader.Name] then
                tbFunctions[fnShader.Name] = fnShader;
            else

            end
        end
    end

    -- ջ���ǵ�ǰ����
    -- local fnCurShader = tbCallFunctionStack[#tbCallFunctionStack];
    -- if tbLine.Func then
    --     local szShaderFuncName = tbLine.CallFunctionName;
    --     local func = ShaderFunction:new(tbLine.CallFunctionName);
    --     func.ShaderPath = tbLine.ShaderPath;
    --     func.Range = tbLine.Func.Range;
    --     func.LineInShaderFile = tbLine.LineInShaderFile;

    --     table.insert(fnAssemble.Callees, szShaderFuncName);


    --     table.insert(tbCallFunctionStack, func);
    -- end

    -- if fnCurShader and tbLine.bInstruction then
    --     fnCurShader.InstructionNum = fnCurShader.InstructionNum + 1;
    -- end


end

---@function: ����һ��Shader���ļ�����ȡ�������ݣ��������к��������ں�Assemble��λ����
---@param szShaderPath string shader·��
---@param nType number 
function ShaderAssemblyParser:_ParseShaderCode(szShaderPath, eFileType)
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
        -- ȥ���ո�
        local szLine = (string.gsub(strLine, "^%s*(.-)%s*$", "%1"));
        local szReturn, szFuncName = self:_GetShaderFunctionName(szLine, eFileType);
        if szFuncName then
            -- print("Name = ", nLine, szReturn, szFuncName);
            szCurFunction = szFuncName;
            tbBrace = {};
            if not tbFunctions[szFuncName] then
                local func = ShaderFunction:new(szFuncName);
                func.Range[1] = nLine;
                func.ShaderPath = szShaderPath;
                tbFunctions[szFuncName] = func;
            end
            ---@type ShaderFunction
            local func = tbFunctions[szFuncName];
            if func then
                -- print("Function Start", nLine, szCurFunction)
            end
        end
        if self:_IsValidLeftBrace(szLine) then
            table.insert(tbBrace, nLine);
        end
        if self:_IsValidRightBrace(szLine) then
            table.remove(tbBrace);

            if szCurFunction and #tbBrace <= 0 then
                -- print("Function end.", nLine, szCurFunction, tbFunctions[szCurFunction]);
                if tbFunctions[szCurFunction] then
                    -- table.insert(tbFunctions[szCurFunction], nLine);
                    tbFunctions[szCurFunction].Range[2] = nLine;
                end
                szCurFunction = nil;
            end
        end
        
        strLine = file:ReadNextLine();
    end
    file:Close();

    return tbFunctions, tbLines;
end

---@param nType number
function ShaderAssemblyParser:_GetShaderFunctionName(szLine, eFileType)
    if not szLine then
        return;
    end
    
    local nStartPos, nEndPos = string.find(szLine, "//");
    if nStartPos == 1 and nEndPos == 2 then
        return;
    end
    
    local bAssemble = eFileType == EnumFileType.EFT_ASSEMBLE_FILE;
    -- Ĭ����shader��������ĺ�����ʽ
    local szPattern = FUNCTION_PATTERN_SHADER;
    if bAssemble then
        szPattern = FUNCTION_PATTERN_ASSEMBLY;
    end

    local szReturn, szName = string.match(szLine, szPattern);
    if ShaderKeyword[szReturn] then
        return;
    end

    -- log
    -- if szName == "bary_value" then
    --    print(szReturn, szLine) 
    -- end

    -- ���������������Assembly�ļ�����������������
    --     int     BlendShapePitch = 0;        // Offset: 33536, size:    4
    if bAssemble and szName then
        local nStartPos, nEndPos = string.find(szLine, "=");
		local nNextPos = string.find(szLine, "%w", nStartPos + 1);
		if nNextPos then
			return;
		end
    end
    return szReturn, szName;
end

function ShaderAssemblyParser:_GetInstructionNum(szLine)
    if not szLine then
        return 0;
    end
    local szPattern = ".*Approximately (%d+) instruction slots used";
    local szNum = string.match(szLine, szPattern);
    if not szNum then
        return 0;
    end
    print("[_GetInstructionNum]", szNum);
    return tonumber(szNum);
end

function ShaderAssemblyParser:_IsValidLeftBrace(szLine)
    local nStartPos = string.find(szLine, "{");
    if not nStartPos then
        return false;
    end
    -- ������ע������
    return true;
end

function ShaderAssemblyParser:_IsValidRightBrace(szLine)
    local nStartPos = string.find(szLine, "}");
    if not nStartPos then
        return false;
    end
    return true;
end

function ShaderAssemblyParser:_IsAllLineAnnotation(szLine)
    local nStartPos, nEndPos = string.find(szLine, "//");
    if nStartPos == 1 and nEndPos == 2 then
        return true;
    end
    return false;
end

function ShaderAssemblyParser:_GetShaderPathFromAssembly(szLine)
    if not szLine then
        return;
    end
    local szPattern = "%s*#line%s+([%d]+)%s+\"([%w\\/:%.-_]+)\"";
    local szLineNum, szPath = string.match(szLine, szPattern);
	return szLineNum, szPath;
end

function ShaderAssemblyParser:_GetShaderPathFromLine(szLine)
    -- �յ�
    if not szLine or string.len(szLine) <= 0 then
        return;
    end

    -- ע��
    if self:_IsAllLineAnnotation(szLine) then
        -- print(szLine);
        return;
    end
    
    -- #line ��ͷ
    local szPattern = "%s*#line%s+([%d]+)%s*";
    local szLineNum = string.match(szLine, szPattern);
    if szLineNum then
        -- ��·������ʽ
        -- #line 196 "h:\svn\BDCode\sword3-products\trunk\client\data\material\Shader_DX11_HD\Voxelization.h5"
        local szLineNumber, szShaderPath = self:_GetShaderPathFromAssembly(szLine);
        local nLine = tonumber(szLineNumber);
        if nLine and szShaderPath then
            -- print(nLine, szShaderPath);
            return nLine, szShaderPath;
        end

        -- ����·������ʽ
        -- #line 200
        nLine = tonumber(szLineNum);
        if nLine then
            return nLine;
        end
    end
    
    -- ʣ�µĶ���ָ����
    return szLine;
end

function ShaderAssemblyParser:_ParseAssemblyCode(tbFunctions, tbLines, tbFunctionMappingNames)
    local tbShaderFunctions = {
        --[[
            [szPath] = {
                [szFuncName] = {nStartLine, nEndLine}
            }
        ]]
    }

    for szFuncName, func in pairs(tbFunctions or {}) do
        local szShaderFunctionName = tbFunctionMappingNames[szFuncName];
        if not szShaderFunctionName then
            print("[Warning]no mapping name......................................................");
        end
        local nStart, nEnd = unpack(func.Range);
        local nInstruction = 0;
        print(szFuncName, nStart, nEnd, szFuncName == "MainVoxelizationWithGbufferGeometryShader");
        local szCallFunction = nil;
        local szShaderName = nil;
        local nCallInstruction = 0;
        -- ����#line�����У����õ�����Щ�������ã��Լ��������ָ�����Ƕ���
        for i = nStart, nEnd do
            -- ȥ���ո�
            local szLine = (string.gsub(tbLines[i], "^%s*(.-)%s*$", "%1"));
            local nLine, szShaderPath = self:_GetShaderPathFromLine(szLine);
            
            if szShaderPath then
                if not tbShaderFunctions[szShaderPath] then
                    local tbSubFunctions = ShaderAssemblyParser:_ParseShaderCode(szShaderPath, EnumFileType.EFT_SHADER_FILE);
                    tbShaderFunctions[szShaderPath] = tbSubFunctions;
                end
                
                for k, v in pairs(tbShaderFunctions[szShaderPath] or {}) do
                    local nCallStart, nCallEnd = unpack(v.Range);
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

function ShaderAssemblyParser:_GetAssemblyFunctionNameToShaderFunctionName(szShaderPath)
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
        -- ȥ���ո�
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

------------------------------------------------------------------------------------------------------
function TestShaderFunction()
    -- E:\work\svn\code\Sword3\Source\KG3DEngineDX11\KG3DEngineE\Internal\Module\KG3D_MaterialSystem\Templates\ModelBasePixelShader.fx5
    local szShaderPath = "E:\\work\\svn\\code\\Sword3\\Source\\KG3DEngineDX11\\KG3DEngineE\\Internal\\Module\\KG3D_MaterialSystem\\Templates\\ModelBasePixelShader.fx5"
    local tbFunctions = ShaderAssemblyParser:_ParseShaderCode(szShaderPath, EnumFileType.EFT_SHADER_FILE);
    for k, v in pairs(tbFunctions) do
        print(k, v.Range[1], v.Range[2]);
    end
end
-- TestShaderFunction();

function TestFunction()
    local tbFunctionMappingNames = {};
    local tbCompileNamesShaderPath = {
        "H:\\svn\\BDCode\\sword3-products\\trunk\\client\\data\\material\\Shader_DX11_HD\\MeshSharedTechnique.h",
    }
    for k, v in pairs(tbCompileNamesShaderPath) do
        local tbNames = ShaderAssemblyParser:_GetAssemblyFunctionNameToShaderFunctionName(v);
        for name1, name2 in pairs(tbNames or {}) do
            tbFunctionMappingNames[name1] = name2;
        end
    end
    for k, v in pairs(tbFunctionMappingNames) do
        print(k, "--->", v)
    end
    
    local szShaderPath = "h:\\svn\\BDCode\\sword3-products\\trunk\\client\\data\\material\\Shader_DX11_HD\\ModelBasePixelShader.fx5"
    -- local tbFunctions = ShaderAssemblyParser:_ParseShaderCode(szShaderPath, EnumFileType.EFT_SHADER_FILE);
    local szAssemblyPath = "F:\\study\\lua\\fragmentary\\mtl_sss_face_debug1.fxo";
    local tbFunctions, tbAllLines = ShaderAssemblyParser:_ParseShaderCode(szAssemblyPath, EnumFileType.EFT_ASSEMBLE_FILE);
    -- ShaderAssemblyParser:_Load(szAssemblyPath);
    print("Lines = ", #tbAllLines, #(tbFunctions or {}))
    print("-------------------------------------")
    -- for k, v in pairs(tbFunctions) do
    --     print(k);
    -- end
    ShaderAssemblyParser:_ParseAssemblyCode(tbFunctions, tbAllLines, tbFunctionMappingNames);
end

-- TestFunction();

function DoParse()
    local szAssemblyPath = "F:\\study\\lua\\fragmentary\\FlexWrapMeshShader_FS.cod";
    -- local tbFunctions, tbAllLines = ShaderAssemblyParser:_ParseShaderCode(szAssemblyPath, EnumFileType.EFT_ASSEMBLE_FILE);
    ShaderAssemblyParser:_Load(szAssemblyPath);
    print("Lines = ", #ShaderAssemblyParser.Lines)
    print("-------------------------------------")
    local nInstructionNum = 0;
    for k, v in pairs(ShaderAssemblyParser.Functions) do
        print(k, v.InstructionNum);
        for _, func in ipairs(v.Callees) do
            print("\t", func.Name, func.InstructionNum);
            nInstructionNum = nInstructionNum + func.InstructionNum;
        end
        print(k, "InstructionNum = ", nInstructionNum)
    end

    -- ShaderAssemblyParser:_ParseAssemblyCode(tbFunctions, tbAllLines, tbFunctionMappingNames);
;
end

-- DoParse();