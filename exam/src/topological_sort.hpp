#ifndef GRAPH_TOPOLOGICAL_SORT_HPP
#define GRAPH_TOPOLOGICAL_SORT_HPP
#include "depth_first_search.hpp"
namespace graph {
namespace detail {

template<typename OIter>
struct TopoVisitor : DFSNullVisitor {
	TopoVisitor(OIter iter) : iter(iter) {}

	template<typename G, typename V>
	void startVertex(const V& v, const G&) {
		iter = v;
		++iter;
	}

	template<typename G, typename E>
	void treeEdge(const E& e, const G& g) {
		iter = target(e,g);
		++iter;
	}
	
private:
	OIter iter;
};

} // namespace detail
template<typename Graph, typename OutputIterator>
void topoSort(const Graph &g, OutputIterator oIter) {
	dfs(g,detail::TopoVisitor(oIter));
}


} // namespace graph
#endif // GRAPH_TOPOLOGICAL_SORT_HPP
