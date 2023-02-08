#include "KTiledGraph.h"
#include <iostream>
#include <string>

void KTiledGraph::TestGraph()
{
	printf("--------------\n");
	KTiledGraph G;
	KTiledNode a{1};
	KTiledNode b{ 2 };
	KTiledNode c{ 3 };
	KTiledNode d{ 4 };
	KTiledNode e{ 5 };
	KTiledNode f{ 6 };
	KTiledNode g{ 7 };
	KTiledNode h{ 8 };
	KTiledEdge e12{ 1, 2, 12 };
	KTiledEdge e13{ 1, 3, 13 };
	KTiledEdge e17{ 1, 7, 17 };
	KTiledEdge e25{ 2, 5, 25 };
	KTiledEdge e57{ 5, 7, 57 };
	G.InsertVetex(a);
	G.InsertVetex(b);
	G.InsertVetex(c);
	G.InsertVetex(d);
	G.InsertVetex(e);
	G.InsertVetex(f);
	G.InsertVetex(g);
	G.InsertVetex(h);
	G.InsertEdge(1, 2, e12);
	G.InsertEdge(1, 3, e13);
	G.InsertEdge(1, 7, e17);
	G.InsertEdge(2, 5, e25);
	G.InsertEdge(5, 7, e57);

	for (auto it = G.BeginEdges(); it != G.EndEdges(); ++it) {
		int nID2 = (it->first & 0xFFFFFFFF);
		int nID1 = (it->first >> 32 & 0xFFFFFFFF);
		std::cout << "Connection " << nID1 << " to " << nID2
			<< " with value:" << it->second.nWeight << std::endl;
	}

	//breadth_first_iterator<graph_type> bfs(graph, graph.find_vertex(1));
	//// Iterate through the graph and print the strings
	//while (!bfs.end()) {
	//	std::cout << bfs.next()->second << std::endl
	//}
	std::vector<std::forward_list<int32_t>> OutBlockConnectedList;
	G._DFS(OutBlockConnectedList, 1);
	for (auto itList = OutBlockConnectedList.begin(); itList != OutBlockConnectedList.end(); ++itList)
	{
		std::string strList = "";
		for (auto it = itList->begin(); it != itList->end(); ++it)
		{
			strList += std::to_string(*it) + "->";
		}
		std::cout << strList << std::endl;
	}

	//bool visited[1204];
	//int path[1024];
	//int path_index = 0;
	//G.printAllPathsUtil(1, 5, visited, path, path_index);
	std::vector<std::vector<int32_t>> OutPaths;
	std::unordered_set<int32_t> visited;
	std::vector<int32_t> pathFromRoot;
	int nIndex = 0;
	G.GetAllPathsBetweenNodes(OutPaths, 1, 5, visited, pathFromRoot, nIndex);
	for (auto itList = OutPaths.begin(); itList != OutPaths.end(); ++itList)
	{
		std::string strList = "";
		for (auto it = itList->begin(); it != itList->end(); ++it)
		{
			strList += std::to_string(*it) + "-->";
		}
		std::cout << strList << std::endl;
	}
}

void KTiledGraph::InsertVetex(KTiledNode& node)
{
	auto pair = m_mapVertices.emplace(node.nID, node);
	m_mapAdjacency.emplace(node.nID, LIST_ADJACENCY());
	//return pair;
}

std::pair<std::unordered_map<int64_t, KTiledEdge>::iterator, bool> KTiledGraph::InsertEdge(int32_t nID1, int32_t nID2, KTiledEdge& edge) {
	// Check if vertices are in the graph
	if (m_mapVertices.count(nID1) == 0 || m_mapVertices.count(nID2) == 0) {
		return std::make_pair(m_mapEdges.end(), false);
	}

	// Try to place new edge
	int64_t nEdgeKey = ((int64_t)nID1 << 32) + nID2;
	auto pair = m_mapEdges.emplace(nEdgeKey, edge);

	// Create adjacency entries if the edge was added
	if (pair.second) {
		m_mapAdjacency.at(nID1).push_front(nID2);
		if (nID1 != nID2) m_mapAdjacency.at(nID2).push_front(nID1);
	}

	return pair;
}

GRAPH_EDGE_ITERATOR KTiledGraph::BeginEdges() { return m_mapEdges.begin(); }
GRAPH_EDGE_CONST_ITERATOR KTiledGraph::BeginEdges() const { return m_mapEdges.begin(); }

GRAPH_EDGE_ITERATOR KTiledGraph::EndEdges() { return m_mapEdges.end(); }
GRAPH_EDGE_CONST_ITERATOR KTiledGraph::EndEdges() const { return m_mapEdges.end(); }

GRAPH_VERTEX_ITERATOR KTiledGraph::BeginVertices() { return m_mapVertices.begin(); }
GRAPH_VERTEX_CONT_ITERATOR KTiledGraph::BeginVertices() const { return m_mapVertices.begin(); }
GRAPH_VERTEX_ITERATOR KTiledGraph::EndVertices() { return m_mapVertices.end(); }
GRAPH_VERTEX_CONT_ITERATOR KTiledGraph::EndVertices() const { return m_mapVertices.end(); }

void KTiledGraph::_DFS(std::vector<std::forward_list<int32_t>>& OutBlockConnectedList, int32_t nID)
{
	
	std::forward_list<int32_t> path;
	std::unordered_set<int32_t> Visited;
	_DFSRecurve(OutBlockConnectedList, nID, path, Visited);
}

///@function DFS查找所有路径
void KTiledGraph::_DFSRecurve(
	std::vector<std::forward_list<int32_t>>& OutBlockConnectedList,
	int32_t nID, 
	std::forward_list<int32_t>& PathFromRoot, 
	std::unordered_set<int32_t>& Visited)
{
	auto path = PathFromRoot;
	path.push_front(nID);
	Visited.emplace(nID);
	auto itAdjacency = m_mapAdjacency.find(nID);
	if (itAdjacency == m_mapAdjacency.end() || itAdjacency->second.empty())
	{
		//path.pop_front();
		OutBlockConnectedList.push_back(path);
		return;
	}


	bool bDeep = false;
	if (itAdjacency != m_mapAdjacency.end())
	{
		for (auto it = itAdjacency->second.begin(); it != itAdjacency->second.end(); ++it)
		{
			if (Visited.count(*it) == 0)
			{
				bDeep = true;
				_DFSRecurve(OutBlockConnectedList, *it, path, Visited);
			}
				
		}
		if (!bDeep)
		{
			//path.pop_front();
			OutBlockConnectedList.push_back(path);
			return;
		}
	}
	//path.pop_front();
}

// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of vertices in current path.
// path[] stores actual vertices and path_index is current
// index in path[]
void KTiledGraph::GetAllPathsBetweenNodes(
	std::vector<std::vector<int32_t>>& OutPaths,
	int32_t s,
	int32_t d,
	std::unordered_set<int32_t>& visited,
	std::vector<int32_t>& pathFromRoot,
	int& index)
{
	std::vector<int32_t> path = pathFromRoot;
	// Mark the current node and store it in path[]
	visited.emplace(s);
	//path[index] = s;
	path.push_back(s);
	//index++;

	// If current vertex is same as destination, then print
	// current path[]
	if (s == d) {
		//for (int i = 0; i < index; i++)
		//	std::cout << path[i] << " ";
		//std::cout << std::endl;
		OutPaths.push_back(path);
	}
	else // If current vertex is not destination
	{
		// Recur for all the vertices adjacent to current
		// vertex
		//std::list<int>::iterator i;
		for (auto it = m_mapAdjacency[s].begin(); it != m_mapAdjacency[s].end(); ++it)
			if (visited.count(*it) == 0)
				GetAllPathsBetweenNodes(OutPaths, *it, d, visited, path,
					index);
	}

	// Remove current vertex from path[] and mark it as
	// unvisited
	//index--;
	//visited[s] = false;
	visited.erase(s);
}