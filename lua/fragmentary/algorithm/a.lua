-----------------------------------------------------------------------------
--脚本文件：a.lua
--功能设计：
--功能开发：page(pk)
--开发时间：2022/10/08 10:17:43
--脚本功能：Strip Packing问题/2D rectangular packing problem
--修改记录：
-----------------------------------------------------------------------------
-- 参考：
-- [1]https://www.zhihu.com/question/402364344/answer/1319272200?utm_source=hot_content_share&utm_medium=all
-- [2]https://blog.csdn.net/zll_hust/article/details/105292856

function _IsIntersect(tbMin1, tbMax1, tbMin2, tbMax2)
	if tbMax2.x <= tbMin1.x or tbMax2.z <= tbMin1.z then
		return false;
	elseif tbMin2.x >= tbMax1.x or tbMin2.z >= tbMax1.z then
		return false;
    end
		
    return true;
end

function StripPacking2D(nWidth, nHeight, tbPackage)
    if nHeight > nWidth then
        nWidth, nHeight = nHeight, nWidth;
    end
    print("w, h = ", nWidth, nHeight)
    table.sort(tbPackage, function(a, b)
        if a[1] > b[1] then
            return true;
        end

        return a[2] > b[2];
    end);

    for k, v in ipairs(tbPackage) do
        print(v[1], v[2]);
    end

    -- 坐标系是nWidth, nHeight区域的中心点为原点, x轴向右，y轴向上
    -- 左上角开始摆放
    local nStartX, nStartY = -nWidth/2, nHeight/2;
    local nEndX, nEndY = nWidth/2, -nHeight / 2;
    local nMaxTimes = 1;
    local tbPosition = {};
    local tbLeftSpace = {{{nStartX, nStartY}, {nEndX, nEndY}}};
    for i = 1, nMaxTimes do
        for k, v in ipairs(tbPackage) do
            print("------------------------------", v[1], v[2])
            local tbRemoveSpaceIndex = {};
            for nIndex, tbSpace in ipairs(tbLeftSpace) do
                nStartX, nStartY = tbSpace[1][1], tbSpace[1][2];
                nEndX, nEndY = tbSpace[2][1], tbSpace[2][2];
                -- print("Package = ", v[1], v[2])
                local w, h = nEndX - nStartX, nStartY - nEndY;
                print("Range = ", nStartX, nStartY, nEndX, nEndY, w, h)
                if v[1] <= 0 or v[1] > nWidth or v[2] <= 0 or v[2] > nHeight then
                    print("[Error]The package size is wrong.");
                    return;
                end
                if v[1] <= w and v[2] <= h then
                    -- 计算位置和下一个可以摆放的空间
                    local x, y = nStartX + v[1] / 2, nStartY - v[2] / 2;
                    print("Package(" .. v[1] .. ", " .. v[2] .. ")--->(" .. x .. ", " .. y .. ")");
                    table.insert(tbPosition, {x, y});
                    table.insert(tbRemoveSpaceIndex, nIndex);

                    -- 右边的空间
                    print("right: ", nStartX + v[1], nStartY, nEndX, nEndY)
                    if nEndX - nStartX + v[1] > 0 then
                        table.insert(tbLeftSpace, {{nStartX + v[1], nStartY}, {nEndX, nEndY}});
                    end
                    -- 下边的空间
                    print("bottom: ", nStartX, nStartY - v[2], nEndX, nEndY)
                    if nStartY + v[2] - nEndY > 0 then
                        table.insert(tbLeftSpace, {{nStartX, nStartY - v[2]}, {nEndX, nEndY}});
                    end

                    --把剩下空间修正一下位置
                    for j = nIndex + 1, #tbLeftSpace do
                        local tbLeftTop = tbLeftSpace[j][1];
                        local tbRightBottom = tbLeftSpace[j][2];
                        -- 相交检测
                        if _IsIntersect({x = nStartX, z = nEndY}, {x = nEndX, z = nStartY}, {x = tbLeftTop[1], z = tbLeftTop[2]}, {x = tbRightBottom[1], z = tbLeftTop[1]}) then
                            print("Begin = ", tbLeftTop[1], tbLeftTop[2], tbRightBottom[1], tbRightBottom[2])
                            if nStartX < tbLeftTop[1] and tbLeftTop[1] < nEndX then
                                tbLeftTop[1] = nEndX;
                            end
                            if nStartX < tbRightBottom[1] and tbRightBottom[1] < nEndX then
                                tbRightBottom[1] = nStartX;
                            end

                            if nEndY < tbLeftTop[2] and tbLeftTop[2] < nStartY then
                                tbLeftTop[2] = nEndY;
                            end

                            if nEndY < tbRightBottom[2] and tbRightBottom[2] < nStartY then
                                tbRightBottom[2] = nStartY;
                            end
                            print("End = ", tbLeftTop[1], tbLeftTop[2], tbRightBottom[1], tbRightBottom[2])
                        end
                    end
                    break;
                end
            end
            print("#tbLeftSpace = ", #tbLeftSpace)
            for nIndex = #tbRemoveSpaceIndex, 1, -1 do
                table.remove(tbLeftSpace, tbRemoveSpaceIndex[nIndex]);
            end
            print("#tbLeftSpace = ", #tbLeftSpace)
        end
        if #tbPosition == #tbPackage then
            print("Find it.")
            break;
        end
    end
    print("Ret = ")
    for k, v in ipairs(tbPosition) do
        print(v[1], v[2]);
    end
    return tbPackage, tbPosition;
end

StripPacking2D(7, 8, {{3, 4}, {3, 3}, {5, 3}});