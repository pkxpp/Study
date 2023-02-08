#include <vector>
#include <unordered_map>
#include <forward_list>
#include <unordered_set>

struct KTiledNode {
	int32_t nID;
};

struct KTiledEdge {
	int32_t nID1;
	int32_t nID2;
	int nWeight;
};

class KTiledGraph {
public:
	KTiledGraph() {}
	~KTiledGraph() {}
	static void TestGraph();
private:
	typedef std::unordered_map<int32_t, KTiledNode> MAP_VERTEX;
	typedef std::unordered_map<int64_t, KTiledEdge> MAP_EDGE;
	typedef std::forward_list<int32_t> LIST_ADJACENCY;
	typedef std::unordered_map<int32_t, LIST_ADJACENCY> MAP_ADJACENCY;
	MAP_VERTEX m_mapVertices;
	MAP_EDGE m_mapEdges;
	MAP_ADJACENCY m_mapAdjacency;

public:
	typedef typename MAP_VERTEX::iterator MAP_VERTEX_ITERATOR;
	typedef typename MAP_VERTEX::const_iterator MAP_VERTEX_CONT_ITERATOR;
	typedef typename MAP_EDGE::iterator MAP_EDGE_ITERATOR;
	typedef typename MAP_EDGE::const_iterator MAP_EDGE_CONST_ITERATOR;
	typedef typename MAP_ADJACENCY::iterator MAP_ADJACENCY_ITERATOR;
	typedef typename MAP_ADJACENCY::const_iterator MAP_ADJACENCY_COST_ITERATOR;

	void InsertVetex(KTiledNode& node);
	std::pair<MAP_EDGE::iterator, bool> InsertEdge(int32_t nID1, int32_t ID2, KTiledEdge& edge);
	MAP_VERTEX_ITERATOR BeginVertices();
	MAP_VERTEX_CONT_ITERATOR BeginVertices() const;
	MAP_VERTEX_ITERATOR EndVertices();
	MAP_VERTEX_CONT_ITERATOR EndVertices() const;

	MAP_EDGE_ITERATOR BeginEdges();
	MAP_EDGE_CONST_ITERATOR BeginEdges() const;
	MAP_EDGE_ITERATOR EndEdges();
	MAP_EDGE_CONST_ITERATOR EndEdges() const;

	void _DFS(std::vector<std::forward_list<int32_t>>& OutBlockConnectedList, int32_t nID);
	void _DFSRecurve(std::vector<std::forward_list<int32_t>>& OutBlockConnectedList, 
		int32_t nID, 
		std::forward_list<int32_t>& PathFromRoot,
		std::unordered_set<int32_t>& Visited);
	void GetAllPathsBetweenNodes(
		std::vector<std::vector<int32_t>>& OutPaths,
		int32_t s,
		int32_t d, 
		std::unordered_set<int32_t>& visited, 
		std::vector<int32_t>& pathFromRoot,
		int& index);
};

typedef typename KTiledGraph::MAP_VERTEX_ITERATOR GRAPH_VERTEX_ITERATOR;
typedef typename KTiledGraph::MAP_VERTEX_CONT_ITERATOR GRAPH_VERTEX_CONT_ITERATOR;
typedef typename KTiledGraph::MAP_EDGE_ITERATOR GRAPH_EDGE_ITERATOR;
typedef typename KTiledGraph::MAP_EDGE_CONST_ITERATOR GRAPH_EDGE_CONST_ITERATOR;
typedef typename KTiledGraph::MAP_ADJACENCY_ITERATOR GRAPH_ADJACENCY_ITERATOR;
typedef typename KTiledGraph::MAP_ADJACENCY_COST_ITERATOR GRAPH_ADJACENCY_COST_ITERATOR;

//class DepthFirstPaths
//{
//private:
//    bool marked[];//记录是否被dfs访问过
//    int edgesTo[];//记录最后一个到当前节点的顶点
//    int s;//搜索的起始点
//
//public:
//    public DepthFirstPaths(Graph g, int s)
//    {
//        marked = new bool[g.GetVerticals()];
//        edgesTo = new int[g.GetVerticals()];
//        this.s = s;
//        dfs(g, s);
//    }
//
//    private void dfs(Graph g, int v)
//    {
//        marked[v] = true;
//        foreach(int w in g.GetAdjacency(v))
//        {
//            if (!marked[w])
//            {
//                edgesTo[w] = v;
//                dfs(g, w);
//            }
//        }
//    }
//
//    public bool HasPathTo(int v)
//    {
//        return marked[v];
//    }
//
//    public Stack<int> PathTo(int v)
//    {
//
//        if (!HasPathTo(v)) return null;
//        Stack<int> path = new Stack<int>();
//
//        for (int x = v; x != s; x = edgesTo[x])
//        {
//            path.Push(x);
//        }
//        path.Push(s);
//        return path;
//    }
//}