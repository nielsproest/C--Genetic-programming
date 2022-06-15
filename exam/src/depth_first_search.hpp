#ifndef GRAPH_DEPTH_FIRST_SEARCH_HPP
#define GRAPH_DEPTH_FIRST_SEARCH_HPP
#include "traits.hpp"
namespace graph {

struct DFSNullVisitor {
	template<typename G, typename V>
	void initVertex(const V&, const G&) { }	

	template<typename G, typename V>
	void startVertex(const V&, const G&) { }	

	template<typename G, typename V>
	void discoverVertex(const V&, const G&) { }	

	template<typename G, typename V>
	void finishVertex(const V&, const G&) { }	

	template<typename G, typename E>
	void examineEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void treeEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void backEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void forwardOrCrossEdge(const E&, const G&) { }	

	template<typename G, typename E>
	void finishEdge(const E&, const G&) { }	
};

namespace detail {

enum struct DFSColour {
	White, Grey, Black
};

template<typename Graph, typename Visitor>
void dfsVisit(const Graph &g, Visitor visitor, typename Traits<Graph>::VertexDescriptor u,
		std::vector<DFSColour> &colour) {
	visitor.discoverVertex(u,g);
	colour.at(u) = DFSColour::Grey;
	for (auto ed : outEdges(u,g)) {
		auto v = target(ed,g);
		visitor.examineEdge(ed, g);
		if (colour.at(v) == DFSColour::White) {
			visitor.treeEdge(ed,g);
			dfsVisit(g,visitor,v,colour);
		} else if (colour.at(v) == DFSColour::Grey) {
			visitor.backEdge(ed, g);
		} else if (colour.at(v) == DFSColour::Black) {
			visitor.forwardOrCrossEdge(ed, g);
		}
		visitor.finishEdge(ed,g);
	}
	colour.at(u) = DFSColour::Black;
	visitor.finishVertex(u,g);
}

} // namespace detail
template<typename Graph, typename Visitor>
void dfs(const Graph &g, Visitor visitor) {
	std::vector<detail::DFSColour> colour;
	for (auto u : vertices(g)) {
		colour.push_back(detail::DFSColour::White);
		visitor.initVertex(u,g);
	}
	for (auto u : vertices(g)) {
		if (colour.at(u) == detail::DFSColour::White) {
			visitor.startVertex(u,g);
			detail::dfsVisit(g,visitor,u,colour);
		}
	}
}

} // namespace graph
#endif // GRAPH_DEPTH_FIRST_SEARCH_HPP
