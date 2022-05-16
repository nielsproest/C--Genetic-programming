#include <iostream>
#include <utility>
#include <cmath>
#include <cassert>
#include <iterator>
#include <functional>

/*
 * Swaps n's parent's pointer for r
 *	n's parent must be calculated before r
 *	so this must a macro.
 *	(inlines arent real)
 *	(jakob loves macro's :) )
 */
#define CSWAP(n,r) {						\
		if (n->parent != nullptr) {			\
			if (n->parent->left == n) {		\
					n->parent->left = r;	\
				} else {					\
					n->parent->right = r;	\
				}							\
			} else {						\
				this->root = r;				\
			}								\
		}

namespace DM852 {
template <typename Key ,typename Value ,typename Comp = std::less < Key >>
class Tree {
public:
	using value_type = std::pair<const Key,Value>;

	class Node {
	public:
		value_type data;

		//Parent pointers are bad m'kay
		Node* parent;
		Node* left;
		Node* right;

		Node(Key k, Value d) : data(k,d) {
			this->parent = nullptr;
			this->left = nullptr;
			this->right = nullptr;
		}

		const Key& key() {
			return this->data.first;
		}
		Value& value() {
			return this->data.second;
		}

		/*
		 * Runtime of O(n)
		 * Clones this node and its children
		 */
		Node* clone(Node* parent) {
			Node* me = new Node(this->key(), this->value());

			me->parent = parent;
			if (this->left != nullptr) {
				me->left = this->left->clone(me);
			}
			if (this->right != nullptr) {
				me->right = this->right->clone(me);
			}

			return me;
		}
	};

	class const_iterator {
	public:
		using value_type = Tree::value_type;
		using reference = const Tree::value_type&;
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = ptrdiff_t; //incorrect
		Node* c;
		Tree<Key, Value, Comp>* p;
		bool last_action_was_increment = false;

		void increment() {
			if (this->c == nullptr) {
				if (!this->last_action_was_increment) {
					this->c = p->head();
				}
				return;
			}

			if (this->c != nullptr) {
				this->c = p->next(this->c);
			}
			if (this->c == nullptr) {
				this->last_action_was_increment = true;
			}
		}
		void decrement() {
			if (this->c == nullptr) {
				if (this->last_action_was_increment) {
					this->c = p->tail();
				}
				return;
			}

			if (this->c != nullptr) {
				this->c = p->prev(this->c);
			}
			if (this->c == nullptr) {
				this->last_action_was_increment = false;
			}
		}

		//Iterator
		const_iterator() : c(nullptr), p(nullptr) {
			this->last_action_was_increment = true;
		}
		const_iterator(Node* start, Tree<Key, Value, Comp>* parent) : c(start), p(parent) {
			if (start == nullptr) {
				this->last_action_was_increment = true;
			}
		}

		bool operator==(const_iterator& other) {
			return this->p == other.p;
		}

		//Required for equality
		//	struct std::common_reference<DM852::Tree<>::iterator&, int&>
		operator int&() const {
			return *this;
		}

		//Pre-increment
		const_iterator& operator++() {
			this->increment();
			return *this;
		}
		//Pre-decrement
		const_iterator& operator--() {
			this->decrement();
			return *this;
		}
		//Post-increment
		const_iterator operator++(int) {
			const_iterator V(this->c, this->p);
			this->increment();
			return V;
		}
		//Post-decrement
		const_iterator operator--(int) {
			const_iterator V(this->c, this->p);
			this->decrement();
			return V;
		}
		//Dereference
		reference operator*() {
			assert(this->c != nullptr);
			return this->c->data;
		}
		reference operator*() const {
			assert(this->c != nullptr);
			return this->c->data;
		}
	};
	class iterator {
	public:
		using value_type = Tree::value_type;
		using reference = Tree::value_type&;
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = ptrdiff_t; //incorrect
		Node* c;
		Tree<Key, Value, Comp>* p;
		bool last_action_was_increment = false;

		void increment() {
			if (this->c == nullptr) {
				if (!this->last_action_was_increment) {
					this->c = p->head();
				}
				return;
			}

			if (this->c != nullptr) {
				this->c = p->next(this->c);
			}
			if (this->c == nullptr) {
				this->last_action_was_increment = true;
			}
		}
		void decrement() {
			if (this->c == nullptr) {
				if (this->last_action_was_increment) {
					this->c = p->tail();
				}
				return;
			}

			if (this->c != nullptr) {
				this->c = p->prev(this->c);
			}
			if (this->c == nullptr) {
				this->last_action_was_increment = false;
			}
		}

		//Iterator
		iterator() : c(nullptr), p(nullptr) {
			this->last_action_was_increment = true;
		}
		iterator(Node* start, Tree<Key, Value, Comp>* parent) : c(start), p(parent) {
			if (start == nullptr) {
				this->last_action_was_increment = true;
			}
		}

		// implicit conversion
		operator const_iterator() const {
			return const_iterator(this->c,this->p);
		}
		// explicit conversion
		explicit operator const_iterator*() const {
			return const_iterator(this->c,this->p);
		}

		bool operator==(iterator& other) {
			return this->p == other.p;
		}

		//Required for equality
		//	struct std::common_reference<DM852::Tree<>::iterator&, int&>
		operator int&() const {
			return *this;
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
			assert(this->c != nullptr);
			return this->c->data;
		}
		reference operator*() const {
			assert(this->c != nullptr);
			return this->c->data;
		}
	};

private:
	//No touchy
	int n = 0;
	Node* root = nullptr;
	Comp cmp = std::less<Key>();

public:
	Tree() {}
	Tree(const Tree& other) {
		this->operator=(other);
	}
	Tree(Comp comp) {
		this->cmp = comp;
	}

	/*
	 * move constructor
	*/
	Tree(const Tree&& other) {
		this->root = other.root->clone(nullptr);
		this->n = other.n;
		this->cmp = other.cmp;
		other.clear();
	}

	/*
	 * Runtime of O(n)
	 * Delete this tree
	 */
	~Tree() {
		this->clear();
	}

	/*
	 * move assignment operator
	*/
	Tree& operator=(const Tree&& other) {
		this->root = other.root->clone(nullptr);
		this->n = other.n;
		this->cmp = other.cmp;
		other->clear();
		return *this;
	}

	/*
	 * Runtime of O(n)
	 * Copies the tree 
	 */
	Tree& operator=(const Tree& other) {
		this->clear();
		this->root = other.root->clone(nullptr);
		this->n = other.n;
		this->cmp = other.cmp;
		return *this;
	}
	/*
	 * Worstcase Runtime of O(n)
	 * Checks equality 
	 */
	bool equals(const Tree& other) {
		if (this->size() != other.size()) {
			return false;
		}

		auto f = this->begin();
		auto g = other.begin();

		auto fd = this->end();
		auto gd = other.end();

		while (
			f != fd &&
			g != gd &&
			this->c_equal((*f).first, (*g).first)// && (*f).second == (*g).second
		) {
			++f;
			++g;
		}

		return f == fd && g == gd;
	}
	bool operator==(const Tree& other) {
		return this->equals(other);
	}
	bool operator!=(const Tree& other) {
		return !this->equals(other);
	}


	bool c_less(const Key& a, const Key& b) {
		return this->cmp(a,b);
	}
	bool c_more(const Key& a, const Key& b) {
		return !this->cmp(a,b);
	}
	bool c_equal(const Key& a, const Key& b) {
		return !this->c_less(a,b) && !this->c_more(a,b);
	}
	bool c_less_or_equal(const Key& a, const Key& b) {
		return this->c_less(a,b) || this->c_equal(a,b);
	}
	bool c_more_or_equal(const Key& a, const Key& b) {
		return this->c_more(a,b) || this->c_equal(a,b);
	}

	/*
	 * Runtime of O(log n)
	 * Navigates to the next node
	 */
	Node *next(Node* work) {
		if (work->right != nullptr) {
			return InorderSuccessor(work->right);
		} else {
			Node* x = work;
			while (x != nullptr && this->c_less_or_equal(x->key(), work->key())) {
				x = x->parent;
			}
			return x;
		}
	}
	/*
	 * Runtime of O(log n)
	 * Navigates to the previous node
	 */
	Node *prev(Node* work) {
		if (work->left != nullptr) {
			return InorderPredecessor(work->left);
		} else {
			Node* x = work;
			while (x != nullptr && this->c_more_or_equal(x->key(), work->key())) {
				x = x->parent;
			}
			return x;
		}
	}

	/*
	 * Return size of tree
	 */
	int size() const {
		return this->n;
	}
	/*
	 * Returns if tree is empty
	 */
	bool empty() const {
		return this->n == 0;
	}
	/*
	 * Runs in O(log n) amortized cost (per Galperin and Rivest)
	 * Inserts an element
	 */
	std::pair<iterator, bool> insert(const Key &key, const Value &value) {
		Node* x = this->root;
		Node* y = nullptr;
		int depth=0;

		Key& k = (Key&)key;

		//Search
		while (x != nullptr) {
			y = x;
			if (this->c_less(k, x->key())) {
				x = x->left;
			} else if (this->c_more(k, x->key())) {
				x = x->right;
			} else { //equal
				y->data.second = value;
				return std::make_pair(iterator(y, this), false);
			}
			depth++;
		}

		//Insert
		this->n++;
		Node* z = new Node(k, value);

		if (y == nullptr) {
			this->root = z;
			z->parent = nullptr;
		} else if (k < y->key()) {
			y->left = z;
			z->parent = y;
		} else {
			y->right = z;
			z->parent = y;
		}

		return std::make_pair(iterator(z, this), true);
	}
	std::pair<iterator, bool> insert(Key &&key, Value &&value) {
		return this->insert((const Key&)key, (const Value&)value);
	}

	/*
	 * Runs in O(log n)
	 * Finds an element
	 */
	iterator find(const Key &key) {
		Node* x = this->root;

		//Standard search
		while (x != nullptr && key != x->key()) {
			if (this->c_less((Key&)key, x->key())) {
				x = x->left;
			} else if (this->c_more((Key&)key, x->key())) {
				x = x->right;
			} else {
				break;
			}
		}

		if (x->key() != key) {
			return this->end();
		} else {
			return iterator(x, this);
		}
	}
	const_iterator find(const Key &key) const {
		iterator f = this->find(key);
		return f;
	}

	/*
	 * Runs in O(n)
	 * Removes all elements
	 */
	void clear() {
		auto remove = [] (Node* x) { delete x; };
		this->PostorderTraversal(this->root, remove);
		this->n = 0;
		this->root = nullptr;
	}
	/*
	 * Runs in O(log n) amortized cost 
	 * Removes an element
	 */
	void erase(const Key &key) {
		bool found = false;
		this->root = this->DeleteNode(this->root, key, &found);
		if (found) {
			this->n--;
		}
	}
	void erase(const_iterator pos) {
		bool useless;
		Node* x = pos.c;
		CSWAP(x, this->DeleteNode(x, x->key, &useless));
		this->n--;
	}

	/*
	 * Runs in O(log n)
	 * Returns the smallest element's value
	 * Expects a non-empty tree
	 */
	const value_type &front() const {
		auto x = this->begin();
		assert(x != nullptr);
		return x->value;
	}
	value_type &front() {
		auto x = this->begin();
		assert(x != nullptr);
		return x->value;
	}
	/*
	 * Runs in O(log n)
	 * Returns the biggest element's value
	 * Expects a non-empty tree
	 */
	const value_type &back() const {
		Node* x = this->root;
		while (x != nullptr && x->right != nullptr) {
			x = x->right;
		}
		assert(x != nullptr);
		return x->value;
	}
	value_type &back() {
		Node* x = this->root;
		while (x != nullptr && x->right != nullptr) {
			x = x->right;
		}
		assert(x != nullptr);
		return x->value;
	}
	/*
	 * Runs in O(log n)
	 * Returns the smallest element
	 */
	const_iterator begin() const {
		Tree* fuck = (Tree*)this;
		return const_iterator(fuck->head(), fuck);
	}
	iterator begin() {
		return iterator(this->head(), this);
	}

	/*
	 * Returns nullptr
	 */
	const_iterator end() const {
		Tree* fuck = (Tree*)this;
		return ++const_iterator(nullptr, fuck);
	}
	iterator end() {
		return ++iterator(nullptr, this);
	}

	Node* head() {
		Node* tmp = this->root;
		while (tmp->left != nullptr) {
			tmp = tmp->left;
		}
		return tmp;
	}
	Node* tail() {
		Node* tmp = this->root;
		while (tmp->right != nullptr) {
			tmp = tmp->right;
		}
		return tmp;
	}
private:
	//Binary tree operations
	/*
	 * Swaps n1 and n2
	 */
	void swap(Node* n1, Node* n2) {
		Node* n1left = n1->left;
		Node* n1right = n1->right;
		Node* n1parent = n1->parent;
		Node* n2left = n2->left;
		Node* n2right = n2->right;
		Node* n2parent = n2->parent;

		n1->left = n2left;
		n1->right = n2right;
		n2->left = n1left;
		n2->right = n1right;

		CSWAP(n1, n2);
		CSWAP(n2, n1);

		//Maintaining a parent pointer is a pain
		n1->parent = n2parent;
		n2->parent = n1parent;
	}
	/*
	 * Runs in O(log n)
	 * Deletes said node
	 */
	Node* DeleteNode(Node* x, int searchKey, bool* found) {
		if (x == nullptr) {
			return nullptr;
		}

		if (this->c_less(x->key(), searchKey)) {
			x->right = this->DeleteNode(x->right, searchKey, found);
		} else if (this->c_more(x->key(), searchKey)) {
			x->left = this->DeleteNode(x->left, searchKey, found);
		} else {
			*found = true;
			if (x->right == nullptr && x->left == nullptr) {
				if (x->parent != nullptr) {
					if (x->parent->left == x) {
						x->parent->left = nullptr;
					} else {
						x->parent->right = nullptr;
					}
				} else {
					this->root = nullptr;
				}

				delete x;
				return nullptr;
			} else if (x->right == nullptr) {
				Node* tmp = x->left;
				tmp->parent = x->parent;
				delete x;
				return tmp;
			} else if (x->left == nullptr) {
				Node* tmp = x->right;
				tmp->parent = x->parent;
				delete x;
				return tmp;
			} else {
				Node* min_but_bigger = this->InorderSuccessor(x->right);
				this->swap(min_but_bigger, x);
				x->right = this->DeleteNode(x->right, x->key(), found);
			}
		}

		return x;
	}
	/*
	 * Runs in O(log n)
	 * Finds the smallest value from this node
	 */
	static Node* InorderSuccessor(Node* x) {
		Node* min = x;
		while (min->left != nullptr) {
			min = min->left;
		}
		return min;
	}
	/*
	 * Runs in O(log n)
	 * Finds the biggest value from this node
	 */
	static Node* InorderPredecessor(Node* x) {
		Node* max = x;
		while (max->right != nullptr) {
			max = max->right;
		}
		return max;
	}

	/*
	 * Runs in O(n)
	 * In-order Traversal
	 */
	template<typename K>
	void InorderTraversal(Node* x, K&& func) {
		if (x == nullptr) {
			return;
		}

		this->InorderTraversal(x->left, func);
		func(x);
		this->InorderTraversal(x->right, func);
	}
	/*
	 * Runs in O(n)
	 * Post-order Traversal
	 */
	template<typename K>
	void PostorderTraversal(Node* x, K&& func) {
		if (x == nullptr) {
			return;
		}
		
		this->PostorderTraversal(x->left, func);
		this->PostorderTraversal(x->right, func);
		func(x);
	}
};
}
