require "../io/tabfile"
require "../io/fs"


function _ProcessAllLines(lines, fNewFile)
    if not lines then
        return;
    end

    if fNewFile then
        fNewFile:Write(lines[1] .. "\n");
    end

    -- 第一行是表头
    local nRowCount = #lines
    for i = 2, nRowCount do
        if string.find(lines[i], "RASTER_TECHNIQUE 4") then
            print(i, lines[i])
        else
            fNewFile:Write(lines[i] .. "\n");
        end
    end

end


function DeleteVirtualGeometryShaderErrors()
    -- local tabfile = LTabFile:new("tools/ShaderList.txt");
    -- local row = tabfile:GetRowCount();
    -- print ("row = ", row)

    local file = FS.Open("tools/ShaderList.txt")
    if not file then
        return;
    end
    local szContent = file:Read();
    if type(szContent) ~= "string" then
        LOG_E("[LTabFile:ctor] load file failed:", self.m_szFilePath)
        file:Close();
        return false
    end
    local lines = string.split(szContent, "\r\n")

    if #lines < 1 then
        file:Close();
        return true
    end
    print("lines = ", #lines)

    local newFile = FS.Open("tools/ShaderList_new.txt", "wt")
    if not newFile then
        return;
    end

    _ProcessAllLines(lines, newFile);


    file:Close();
    newFile:Close();
end

DeleteVirtualGeometryShaderErrors();