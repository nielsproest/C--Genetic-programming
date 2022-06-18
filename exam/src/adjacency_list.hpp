#ifndef GRAPH_ADJACENCY_LIST_HPP
#define GRAPH_ADJACENCY_LIST_HPP
#include "tags.hpp"
#include "traits.hpp"
#include "properties.hpp"
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <cassert>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include <type_traits>
namespace graph {

template<typename DirectedCategoryT,
         typename VertexPropT = NoProp,
         typename EdgePropT = NoProp>
struct AdjacencyList {
public:
	struct EdgeDescriptor;
private:
	using OutEdge = std::size_t;
	using OutEdgeList = std::vector<OutEdge>;
	using InEdgeList = OutEdgeList;

	struct StoredVertexSimple {
		OutEdgeList eOut;
	};
	struct StoredVertexComplex : StoredVertexSimple {
		InEdgeList eIn;
	};

	using StoredVertexT = std::conditional_t<
		std::same_as<DirectedCategoryT, graph::tags::Directed>, 
		StoredVertexSimple, //If directed
		StoredVertexComplex //If bidirected (technically otherwise)
	>;
	struct StoredVertexE : StoredVertexT {
		VertexPropT prop;
	};
	using StoredVertex = std::conditional_t<
		std::same_as<VertexPropT, NoProp>, 
		StoredVertexT, //If no prop
		StoredVertexE
	>;

	struct StoredEdgeT {
		std::size_t src, tar;
	};
	struct StoredEdgeE : StoredEdgeT {
		EdgePropT prop;
	};
	using StoredEdge = std::conditional_t<
		std::same_as<EdgePropT, NoProp>, 
		StoredEdgeT, //If no prop
		StoredEdgeE
	>;

	using VList = std::vector<StoredVertex>;
	using EList = std::vector<StoredEdge>;
public: // Graph
	using DirectedCategory = DirectedCategoryT;
	using VertexDescriptor = std::size_t;

	struct EdgeDescriptor {
		EdgeDescriptor() = default;
		EdgeDescriptor(std::size_t src, std::size_t tar,
		               std::size_t storedEdgeIdx)
			: src(src), tar(tar), storedEdgeIdx(storedEdgeIdx) {}
	public:
		std::size_t src, tar;
		std::size_t storedEdgeIdx;
	public:
		friend bool operator==(const EdgeDescriptor &a,
		                       const EdgeDescriptor &b) {
			return a.storedEdgeIdx == b.storedEdgeIdx;
		}
	};
public: // PropertyGraph
	using VertexProp = VertexPropT;
	using EdgeProp = EdgePropT;
public: // VertexListGraph
	struct VertexRange {
		// the iterator is simply a counter that returns its value when
		// dereferenced
		using iterator = boost::counting_iterator<VertexDescriptor>;
	public:
		VertexRange(std::size_t n) : n(n) {}
		iterator begin() const { return iterator(0); }
		iterator end()   const { return iterator(n); }
	private:
		std::size_t n;
	};
public: // EdgeListGraph
	struct EdgeRange {
		// We want to adapt the edge list,
		// so it dereferences to EdgeDescriptor instead of StoredEdge
		using EListIterator = typename EList::const_iterator;

		struct iterator : boost::iterator_adaptor<
				iterator, // because we use CRTP
				EListIterator, // the iterator we adapt
				// we want to convert the StoredEdge into an EdgeDescriptor:
				EdgeDescriptor,
				// we can use RA as the underlying iterator supports it:
				std::random_access_iterator_tag,
				// when we dereference we return by value, not by reference
				EdgeDescriptor
			> {
			using Base = boost::iterator_adaptor<
				iterator, EListIterator, EdgeDescriptor,
				std::random_access_iterator_tag, EdgeDescriptor>;
		public:
			iterator() = default;
			iterator(EListIterator i, EListIterator first) : Base(i), first(first) {}
		private:
			// let the Boost machinery use our methods:
			friend class boost::iterator_core_access;

			EdgeDescriptor dereference() const {
				// get our current position stored in the
				// boost::iterator_adaptor base class
				const EListIterator &i = this->base_reference();
				return EdgeDescriptor{i->src, i->tar,
					static_cast<std::size_t>(i - first)};
			}
		private:
			EListIterator first;
		};
	public:
		EdgeRange(const AdjacencyList &g) : g(&g) {}

		iterator begin() const {
			return iterator(g->eList.begin(), g->eList.begin());
		}

		iterator end() const {
			return iterator(g->eList.end(), g->eList.begin());
		}
	private:
		const AdjacencyList *g;
	};
public:
	//Default constructable
	AdjacencyList() = default;
	AdjacencyList(std::size_t n) : vList(n) {}
	//Copy constructable
	AdjacencyList(const AdjacencyList& other) {
		std::copy(other.vList.begin(), other.vList.end(), this->vList);
		std::copy(other.eList.begin(), other.eList.end(), this->eList);
	}
	//Copy assignable
	AdjacencyList operator=(const AdjacencyList& other) {
		std::copy(other.vList.begin(), other.vList.end(), this->vList);
		std::copy(other.eList.begin(), other.eList.end(), this->eList);
	}
	//Move constructable
	AdjacencyList(AdjacencyList&& other) {
		std::move(other.vList.begin(), other.vList.end(), this->vList);
		std::move(other.eList.begin(), other.eList.end(), this->eList);
	}
	//Move assignable
	AdjacencyList operator=(AdjacencyList&& other) {
		std::move(other.vList.begin(), other.vList.end(), this->vList);
		std::move(other.eList.begin(), other.eList.end(), this->eList);
	}
private:
	VList vList;
	EList eList;
public:
	struct OutEdgeRange {
	public:
		std::size_t s;
		const AdjacencyList* parent;

		OutEdgeRange() {}
		OutEdgeRange(std::size_t s, const AdjacencyList* parent) : s(s), parent(parent) {}
		struct iterator;
		iterator begin() {
			return iterator(&this->parent->vList.at(this->s).eOut, this);
		}
		iterator end() {
			return ++iterator(&this->parent->vList.at(this->s).eOut, this, 0);
		}
		EdgeDescriptor create(std::size_t elst_idx) {
			VertexDescriptor other = this->parent->eList.at(elst_idx).tar;
			return EdgeDescriptor(this->s, other, elst_idx);
		}

		struct iterator {
			using value_type = EdgeDescriptor;
			using reference = value_type;
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = ptrdiff_t; //incorrect
	
			const OutEdgeList* p; //The list we iterate over
			OutEdgeRange* r; //The parent
			std::size_t c; //The index

			void increment() {
				this->c = std::min(((std::size_t)c)+1, this->p->size()); //Goes 1 above
			}
			void decrement() {
				this->c = std::max(((std::size_t)c)-1, std::size_t(0));
			}

			//Iterator
			iterator() : c(std::size_t(0)), p(nullptr) {}
			iterator(const OutEdgeList* p, OutEdgeRange* re) : r(re), c(std::size_t(0)), p(p) {}
			iterator(const OutEdgeList* p, OutEdgeRange* re, int) : r(re), c(std::size_t(p->size()-1)), p(p) {}

			//Required for equality
			//	struct std::common_reference<DM852::List<int>::iterator&, int&>
			operator int&() const {
				return (int&)c;
			}

			//Pre-increment
			iterator& operator++() {
				this->increment();
				return *this;
			}
			//Pre-decrement
			iterator& operator--() {
				this->decrement();
				return *this;
			}
			//Post-increment
			iterator operator++(int) {
				iterator V(this->c, this->p);
				this->increment();
				return V;
			}
			//Post-decrement
			iterator operator--(int) {
				iterator V(this->c, this->p);
				this->decrement();
				return V;
			}
			//Dereference
			reference operator*() {
				std::size_t e = this->p->at(this->c);
				return r->create(e);
			}
			reference operator*() const {
				std::size_t e = this->p->at(this->c);
				return r->create(e);
			}
		};
	};
	//InEdgeRange is basically the same thing, with source and target switched
	struct InEdgeRange : OutEdgeRange {
		InEdgeRange() {}
		InEdgeRange(std::size_t s, const AdjacencyList* parent) : OutEdgeRange(s, parent) {}
		using iterator = OutEdgeRange::iterator;
		iterator begin() {
			return iterator(&this->parent->vList.at(this->s).eIn, this);
		}
		iterator end() {
			return ++iterator(&this->parent->vList.at(this->s).eIn, this, 0);
		}
		EdgeDescriptor create(std::size_t elst_idx) {
			VertexDescriptor other = this->parent->eList.at(elst_idx).src;
			return EdgeDescriptor(other, this->s, elst_idx);
		}
	};
public: // Graph
	friend VertexDescriptor source(EdgeDescriptor e, const AdjacencyList &g) {
		return e.src;
	}

	friend VertexDescriptor target(EdgeDescriptor e, const AdjacencyList &g) {
		return e.tar;
	}
public: // VertexListGraph
	friend std::size_t numVertices(const AdjacencyList &g) {
		return g.vList.size();
	}

	friend VertexRange vertices(const AdjacencyList &g) {
		return VertexRange(numVertices(g));
	}
public: // EdgeListGraph
	friend std::size_t numEdges(const AdjacencyList &g) {
		return g.eList.size();
	}

	friend EdgeRange edges(const AdjacencyList &g) {
		return EdgeRange(g);
	}
public: // Other
	friend std::size_t getIndex(VertexDescriptor v, const AdjacencyList &g) {
		return v;
	}
public: // IncidenceGraph
	friend OutEdgeRange outEdges(VertexDescriptor v, const AdjacencyList &g) {
		const AdjacencyList* _g = &g;
		return OutEdgeRange(v,_g);
	}
	friend std::size_t outDegree(VertexDescriptor v, const AdjacencyList &g) {
		return g.vList.at(v).eOut.size();
	}
public: // BidirectionalGraph
	friend InEdgeRange inEdges(VertexDescriptor& v, const AdjacencyList &g) requires std::same_as<DirectedCategory, graph::tags::Bidirectional> {
		const AdjacencyList* _g = &g;
		return InEdgeRange(v,_g);
	}
	friend std::size_t inDegree(VertexDescriptor& v, const AdjacencyList &g) requires std::same_as<DirectedCategory, graph::tags::Bidirectional> {
		return g.vList.at(v).eIn.size();
	}
public: // MutableGraph
	friend VertexDescriptor addVertex(AdjacencyList &g) requires std::default_initializable<VertexProp> {
		//Create an edge
		StoredVertex e;
		//Append to vertex list
		g.vList.push_back(e);
		//Return index
		return g.vList.size()-1;
	}
	friend EdgeDescriptor addEdge(const VertexDescriptor u, const VertexDescriptor v, AdjacencyList &g) requires std::same_as<DirectedCategory, graph::tags::Directed> && std::default_initializable<EdgeProp> {
		//Create an edge
		StoredEdge e;
		e.src = u;
		e.tar = v;

		//Append to edge list
		g.eList.push_back(e);
		auto idx = g.eList.size()-1;

		//Add edge to u's outedges
		std::vector<std::size_t>& ul = g.vList.at(u).eOut;
		ul.push_back(idx);

		//Return descriptor
		return EdgeDescriptor(u,v,idx);
	}
	friend EdgeDescriptor addEdge(const VertexDescriptor u, const VertexDescriptor v, AdjacencyList &g) requires std::same_as<DirectedCategory, graph::tags::Bidirectional> && std::default_initializable<EdgeProp> {
		//Create an edge
		StoredEdge e;
		e.src = u;
		e.tar = v;

		//Append to edge list
		g.eList.push_back(e);
		auto idx = g.eList.size()-1;

		//Add edge to u's outedges
		std::vector<std::size_t>& ul = g.vList.at(u).eOut;
		ul.push_back(idx);

		//Also add to v's inedges
		std::vector<std::size_t>& vl = g.vList.at(v).eIn;
		vl.push_back(idx);

		//Return descriptor
		return EdgeDescriptor(u,v,idx);
	}
public: // MutablePropertyGraph
	friend VertexDescriptor addVertex(const VertexProp &prop, AdjacencyList &g) requires (!std::same_as<VertexProp, NoProp>) {
		//Create an edge
		StoredVertex e;
		e.prop = prop;
		//Append to vertex list
		g.vList.push_back(e);
		//Return index
		return g.vList.size()-1;
	}
	friend EdgeDescriptor addEdge(const VertexDescriptor u, const VertexDescriptor v, const EdgeProp &prop, AdjacencyList &g) requires (!std::same_as<EdgeProp, NoProp>) && std::same_as<DirectedCategory, graph::tags::Directed> {
		//Create an edge
		StoredEdge e;
		e.src = u;
		e.tar = v;
		e.prop = prop;

		//Append to edge list
		g.eList.push_back(e);
		auto idx = g.eList.size()-1;

		//Add edge to u's outedges
		std::vector<std::size_t>& ul = g.vList.at(u).eOut;
		ul.push_back(idx);

		//Return descriptor
		return EdgeDescriptor(u,v,idx);
	}
	friend EdgeDescriptor addEdge(const VertexDescriptor u, const VertexDescriptor v, const EdgeProp &prop, AdjacencyList &g) requires (!std::same_as<EdgeProp, NoProp>) && std::same_as<DirectedCategory, graph::tags::Bidirectional> {
		//Create an edge
		StoredEdge e;
		e.src = u;
		e.tar = v;
		e.prop = prop;

		//Append to edge list
		g.eList.push_back(e);
		auto idx = g.eList.size()-1;

		//Add edge to u's outedges
		std::vector<std::size_t>& ul = g.vList.at(u).eOut;
		ul.push_back(idx);

		//Also add to v's inedges
		std::vector<std::size_t>& vl = g.vList.at(v).eIn;
		vl.push_back(idx);

		//Return descriptor
		return EdgeDescriptor(u,v,idx);
	}
public: // PropertyGraph
	VertexProp& operator[](VertexDescriptor e) requires (!std::same_as<VertexProp, NoProp>) {
		return vList.at(e).prop;
	}
	EdgeProp& operator[](EdgeDescriptor e) requires (!std::same_as<EdgeProp, NoProp>) {
		return eList.at(e.src).prop;
	}
	const VertexProp& operator[](const VertexDescriptor& e) const requires (!std::same_as<VertexProp, NoProp>) {
		return vList.at(e).prop;
	}
	const EdgeProp& operator[](const EdgeDescriptor& e) const requires (!std::same_as<EdgeProp, NoProp>) {
		return eList.at(source(e)).prop;
	}
};
} // namespace graph
#endif // GRAPH_ADJACENCY_LIST_HPP

