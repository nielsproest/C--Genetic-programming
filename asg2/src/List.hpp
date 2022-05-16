#include <iostream>
#include <cassert>
#include <iterator>

namespace DM852 {
template <typename T>
class List {
public:
	//Jakob wanted this
	typedef T value_type;

	//Internal node class
	class Node {
	public:
		//Stored value
		value_type data;
		//Next pointer
		Node* next;
		Node* prev;
		Node(value_type d) : data(d) {
			this->next = nullptr;
			this->prev = nullptr;
		}
	};

	//Must be BidirectionalIterator
	class const_iterator {
	public:
		using value_type = List::value_type;
		using reference = const List::value_type&;
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = ptrdiff_t; //incorrect
		Node* c;
		const List<value_type>* p;
		bool last_action_was_increment = false;

		void increment() {
			if (this->c == nullptr) {
				if (!this->last_action_was_increment) {
					this->c = ((List<value_type>*)p)->r_head();
				}
				return;
			}

			if (this->c != nullptr) {
				this->c = this->c->next;
			}
			if (this->c == nullptr) {
				this->last_action_was_increment = false;
			}
		}
		void decrement() {
			if (this->c == nullptr) {
				if (this->last_action_was_increment) {
					this->c = ((List<value_type>*)p)->r_tail();
				}
				return;
			}

			if (this->c != nullptr) {
				this->c = this->c->prev;
			}
			if (this->c == nullptr) {
				this->last_action_was_increment = false;
			}
		}

		//Iterator
		const_iterator() : c(nullptr), p(nullptr) {
			this->last_action_was_increment = true;
		}
		const_iterator(Node* start, const List<T>* parent) : c(start), p(parent) {
			if (start == nullptr) {
				this->last_action_was_increment = true;
			}
		}

		//Required for equality
		//	struct std::common_reference<DM852::List<int>::iterator&, int&>
		operator int&() const {
			return this;
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
		using value_type = List::value_type;
		using reference = List::value_type&;
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = ptrdiff_t; //incorrect
		Node* c;
		const List<value_type>* p;
		bool last_action_was_increment = false;

		void increment() {
			if (this->c == nullptr) {
				if (!this->last_action_was_increment) {
					this->c = ((List<value_type>*)p)->r_head();
				}
				return;
			}

			if (this->c != nullptr) {
				this->c = this->c->next;
			}
			if (this->c == nullptr) {
				this->last_action_was_increment = false;
			}
		}
		void decrement() {
			if (this->c == nullptr) {
				if (this->last_action_was_increment) {
					this->c = ((List<value_type>*)p)->r_tail();
				}
				return;
			}

			if (this->c != nullptr) {
				this->c = this->c->prev;
			}
			if (this->c == nullptr) {
				this->last_action_was_increment = false;
			}
		}

		//Iterator
		iterator() : c(nullptr), p(nullptr) {
			this->last_action_was_increment = true;
		}
		iterator(Node* start, const List<value_type>* parent) : c(start), p(parent) {
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

		//Required for equality
		//	struct std::common_reference<DM852::List<int>::iterator&, int&>
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
	//My precious 
	Node* head = nullptr;
	Node* tail = nullptr;
	int n = 0;

public:
	List() {}
	List(const List& other) {
		this->operator=(other);
	}

	/*
	 * move constructor
	*/
	List(const List&& other) {
		this->copy(other);
		other.clear();
	}
	/*
	 * Runtime of O(n)
	 * Delete this list
	 */
	~List() {
		this->clear();
	}

	/*
	 * move assignment operator
	*/
	List& operator=(const List&& other) {
		List& rtn = this->copy(other);
		other.clear();
		return rtn;
	}

	/*
	 * Runtime of O(n)
	 * Expects a valid list
	 */
	List& operator=(const List& other) {
		return this->copy(other);
	}
	List& copy(const List& other) {
		auto start = other.begin();
		this->clear();

		for (int i=0; i < other.size(); i++) {
			this->push_back(*start);
			++start;
		}

		return *this;
	}
	/*
	 * Worst case Runtime of O(n)
	 * Expects a valid list
	 */
	bool operator==(const List& other) {
		return this->equals(other);
	}
	bool operator!=(const List& other) {
		return !this->equals(other);
	}

	bool equals(const List& other) {
		if (this->size() != other.size()) {
			return false;
		}

		auto f = this->begin();
		auto g = other.begin();
		int i = this->size();

		while (
			--i > 0 &&
			*f == *g
		) {
			++f;
			++g;
		}

		return *f == *g;
	}

	/*
	 * For the iterator
	 * (please dont use it)
	 */
	Node* r_head() {
		return this->head;
	}
	Node* r_tail() {
		return this->tail;
	}

	/*
	 * Returns size
	 */
	int size() const {
		return this->n;
	}
	/*
	 * Checks if empty
	 */
	bool empty() const {
		return this->n == 0;
	}
	/*
	 * Adds an element to the end
	 */
	void push_back(value_type &elem) {
		Node* p = new Node(elem);

		this->n++;
		if (this->head == nullptr) {
			this->head = p;
			this->tail = p;
			return;
		}

		Node* last = this->tail;
		p->prev = last;
		last->next = p;
		this->tail = p;
	}
	void push_back(const value_type &elem) {
		return this->push_back((value_type&) elem);
	}
	/*
	 * Adds an element behind a node
	 */
	iterator insert(const_iterator pos, value_type &&elem) {
		Node* p = new Node(elem);
		Node* node = pos.c;
		p->next = node;
		p->prev = node->prev;

		//If not head element
		if (node->prev != nullptr) {
			node->prev->next = p;
		}
		node->prev = p;

		//If head element
		if (this->head == node) {
			this->head = p;
		}

		this->n++;
		return iterator(p, this);
	}
	iterator insert(const_iterator pos, const value_type &elem) {
		return this->insert(pos, (value_type&&)elem);
	}
	/*
	 * Runtime of O(n)
	 * Deletes all nodes
	 */
	void clear() {
		Node* old = this->head;

		while (old != nullptr) {
			Node* next = old->next;
			delete old;
			old = next;
		}

		this->n = 0;
		this->head = nullptr;
		this->tail = nullptr;
	}
	/*
	 * Deletes last nodes
	 */
	void pop_back() {
		Node* last = this->tail;
		
		if (last == nullptr) {
			return;
		} else if (last == this->head) {
			this->head = nullptr;
			this->tail = nullptr;
		} else {
			last->prev->next = nullptr;
			this->tail = last->prev;
		}

		delete last;
		this->n--;
	}
	/*
	 * Deletes a nodes
	 */
	void erase(const_iterator pos) {
		Node* node = pos.c;
		pos++;

		if (node->next != nullptr) {
			node->next->prev = node->prev;
		} else {
			this->tail = node->prev;
		}
		if (node->prev != nullptr) {
			node->prev->next = node->next;
		} else {
			this->head = node->next;
		}
		delete node;
		this->n--;
	}
	/*
	 * Returns the data from the first element
	 */
	const value_type &front() const {
		assert(this->head != nullptr);
		return this->head->data;
	}
	value_type &front() {
		assert(this->tail != nullptr);
		return this->tail->data;
	}
	/*
	 * Returns the data from the last element
	 */
	const value_type &back() const {
		assert(this->tail != nullptr);
		return this->tail->data;
	}
	value_type &back() {
		assert(this->head != nullptr);
		return this->head->data;
	}
	/*
	 * Returns the first element
	 */
	iterator begin() {
		return iterator(this->head, this);
	}
	const_iterator begin() const {
		return const_iterator(this->head, this);
	}
	/*
	 * Returns nullptr
	 */
	iterator end() {
		return ++iterator(this->tail, this);
	}
	const_iterator end() const {
		return ++const_iterator(this->tail, this);
	}
};
}
