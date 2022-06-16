#include "../src/adjacency_list.hpp"
#include "../src/concepts.hpp"
#include "../src/depth_first_search.hpp"
#include "../src/topological_sort.hpp"
#include <iostream>

template<typename Graph>
void graph_test(Graph& e) requires graph::Graph<Graph> {
	std::cout << "graph_test" << std::endl;
}
template<typename Graph>
void mutgraph_test(Graph& e) requires graph::MutableGraph<Graph> {
	std::cout << "mutgraph_test" << std::endl;
	auto v1 = addVertex(e);

	auto v2 = addVertex(e);
	addEdge(v1,v2,10,e); //Technically property graph, meh
	auto v3 = addVertex(e);
	addEdge(v2,v3,20,e);
	auto v4 = addVertex(e);
	addEdge(v3,v4,30,e);
	auto v5 = addVertex(e);
	addEdge(v4,v5,40,e);
	auto v6 = addVertex(e);
	addEdge(v5,v6,50,e);
}
template<typename Graph>
void incgraph_test(Graph& e) requires graph::IncidenceGraph<Graph> {
	std::cout << "incgraph_test" << std::endl;
	//TODO: Some invalid parameter Range R void in Range
	for (auto v : vertices(e)) {
		std::cout << "Vertex: " << v << std::endl;
		std::cout << "\toutDegree: " << outDegree(v,e) << std::endl;
		std::cout << "\toutEdge: " << std::endl;
		for (auto ed : outEdges(v,e)) {
			std::cout << "\t\t" << target(ed,e) << std::endl;
		}
	}
}

template<typename Graph>
void bigraph_test(Graph& e) requires graph::BidirectionalGraph<Graph> {
	std::cout << "bigraph_test" << std::endl;
	for (auto v : vertices(e)) {
		std::cout << "Vertex: " << v << std::endl;
		std::cout << "\tinDegree: " << inDegree(v,e) << std::endl;
		std::cout << "\tinEdge: " << std::endl;
		for (auto ed : inEdges(v,e)) {
			std::cout << "\t\t" << source(ed,e) << std::endl;
		}
	}
}

template<typename Graph>
void propgraph_test(Graph& e) requires graph::PropertyGraph<Graph> {
	//TODO: Cannot have const access operator
	std::cout << "propgraph_test" << std::endl;
	for (auto v : edges(e)) {
		std::cout << "edge: " << source(v,e) << std::endl;
		std::cout << "edge: " << e[v] << std::endl;
	}
}

template<typename Graph>
void mutpropgraph_test(Graph& e) requires graph::MutablePropertyGraph<Graph> {
	std::cout << "mutpropgraph_test" << std::endl;
	for (auto v : edges(e)) {
		std::cout << "edge: " << source(v,e) << std::endl;
		std::cout << "edge: " << e[v] << std::endl;
	}
}

template<typename Graph>
void dfsgraph_test(Graph& e) {
	std::cout << "dfsgraph_test" << std::endl;
	dfs(e,graph::DFSNullVisitor());
}

template<typename Graph>
void topograph_test(Graph& g) {
	std::cout << "topograph_test" << std::endl;
	std::vector<typename graph::Traits<Graph>::VertexDescriptor> vs;
	vs.reserve(numVertices(g));
	graph::topoSort(g,std::back_inserter(vs));
	std::reverse(vs.begin(), vs.end());
}

void directed() {
	graph::AdjacencyList<graph::tags::Directed, int, int> e;

	graph_test(e);
	mutgraph_test(e);
	incgraph_test(e);
	propgraph_test(e);
	mutpropgraph_test(e);
	dfsgraph_test(e);
	topograph_test(e);
}
void bidirection() {
	graph::AdjacencyList<graph::tags::Bidirectional, int, int> e;
	graph_test(e);
	mutgraph_test(e);
	incgraph_test(e);
	bigraph_test(e);
	propgraph_test(e);
	mutpropgraph_test(e);
	dfsgraph_test(e);
	topograph_test(e);
}

int main() {
	directed();
	bidirection();
}