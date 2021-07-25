---------------------------------------------------------------------------------------
--Author		:pk
--Designer		:pk
--Date			:2021-07-24 14:23:43
---------------------------------------------------------------------------------------

local tbChain = {
    {"A", "B", "C", "D", "H"},
    {"E", "C", "D", "H"},
    {"A", "F", "G"}
}

function Chain2Graph(tbChain)
    local tbGraph = {};
    for _, tbOneChain in pairs(tbChain or {}) do
        for k, u in ipairs(tbOneChain) do
            if k == #tbOneChain then
                break;
            end
            if not tbGraph[u] then
                tbGraph[u] = {};
            end
            local v = tbOneChain[k+1];
            if not tbGraph[v] then
                tbGraph[v] = {};
            end

            local bExist = false;
            for _, node in pairs(tbGraph[u]) do
                if node == v then
                    bExist = true;
                    break;
                end
            end
            if not bExist then
                table.insert(tbGraph[u], v);
            end
        end
    end
    return tbGraph;
end

function PrintGraph(tbGraph)
    for u, tbV in pairs(tbGraph or {}) do
        local szGraph = u .. ": ";
        for k, v in ipairs(tbV) do
            szGraph = szGraph .. v .. ", "
        end
        print(szGraph);
    end
end

local tbGraph1 = Chain2Graph(tbChain);
PrintGraph(tbGraph1);

function Graph2Chain(tbGraph)
end

local tbNode = {
    Name = "",
    Parent = nil,
    Color = 0, --(0, white; 1- gray; 2- black)
    StartTime = 0,
    EndTime = 0,
}

function ConstructNodeGraph(tbGraph)
    local tbNodeGraph = {};
    tbNodeGraph.V = {};
    tbNodeGraph.E = {};
    for k, v in pairs(tbGraph) do
        tbNodeGraph.V[k] = {
            Name = k,
        };
    end

    for k, v in pairs(tbGraph) do
        if not tbNodeGraph.E[k] then
            tbNodeGraph.E[k] = {};
        end

        for _, v1 in pairs(v) do
            table.insert(tbNodeGraph.E[k], tbNodeGraph.V[v1]);
        end
    end
    return tbNodeGraph;
end

local tbNodeGraph = ConstructNodeGraph(tbGraph1);
for k, v in pairs(tbNodeGraph.E) do
    print(111, k, #v)
end

function DFS(G)
    for k, v in pairs(G.V or {}) do
        v.Parent = nil;
        v.Color = 0;
    end

    local nTime = 0;
    local DFS_VISIT;
    DFS_VISIT = function(G, u)
        nTime = nTime + 1;
        u.StartTime = nTime;
        u.Color = 1;
        print(110, u.Name)
        for _, v in ipairs(G.E[u.Name]) do
            if v.Color == 0 then
                v.Parent = u;
                print("\t", v.Name, v.Parent)
                DFS_VISIT(G, v);
            end
        end
        print(111)
        u.Color = 2;
        nTime = nTime + 1;
        u.EndTime = nTime;
    end

    for k, v in pairs(G.V or {}) do
        if v.Color == 0 then
            DFS_VISIT(G, v);
        end
    end
end

print("DFS: ")
DFS(tbNodeGraph);

for k, v in pairs(tbNodeGraph.V) do
    -- if not v.Parent then
        print(v.Name, v.Parent, v.Parent and v.Parent.Name)
    -- end
end