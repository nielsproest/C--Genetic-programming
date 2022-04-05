#include <iostream>
#include <cassert>

/*
TODO: It must be a Regular type, i.e., be DefaultConstructible, Copyable, and EqualityComparable.

It must additionally have the following member functions. 
Some of them have pre-conditions, 
which you must document as a comment at the function declaration in the header file. 
Similarly, if a member func- tion does not run in constant time (ignoring memory allocation), 
you must document the computational complexity. 
If you have considered an alternative implementation to achieve a different computational complexity, 
rite a short note about it as well.
*/

namespace DM852 {
class List {
public:
	//Internal node class
	class Node {
	public:
		//Stored value
		std::string data;
		//Next pointer
		Node* next;
		Node* prev;
		Node(std::string d) : data(d) {
			this->next = nullptr;
			this->prev = nullptr;
		}
	};

private:
	//My precious 
	Node* head;
	Node* tail;
	int n;

public:
	List() {
		this->head = nullptr;
		this->tail = nullptr;
		this->n = 0;
	}
	/*
	 * Runtime of O(n)
	 * Delete this list
	 */
	~List() {
		this->clear();
	}

	/*
	 * Runtime of O(n)
	 * Expects a valid list
	 */
	List& operator=(const List& other) {
		const Node* start = other.begin();
		this->clear();

		while (start != nullptr) {
			this->push_back(start->data);
			start = start->next;
		}

		return *this;
	}
	/*
	 * Worst case Runtime of O(n)
	 * Expects a valid list
	 */
	bool operator==(const List& other) {
		if (this->size() != other.size()) {
			return false;
		}

		const Node* f = this->begin();
		const Node* g = other.begin();

		while (
			f != nullptr && 
			g != nullptr &&
			f->data == g->data
		) {
			f = f->next;
			g = g->next;
		}

		return f == nullptr && g == nullptr;
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
	void push_back(const std::string &elem) {
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
	/*
	 * Adds an element behind a node
	 */
	Node* insert(Node* node, const std::string &elem) {
		Node* p = new Node(elem);
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
		return p;
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
	void erase(Node* node) {
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
	const std::string &front() const {
		assert(this->head != nullptr);
		return this->head->data;
	}
	std::string &front() {
		assert(this->tail != nullptr);
		return this->tail->data;
	}
	/*
	 * Returns the data from the last element
	 */
	const std::string &back() const {
		assert(this->tail != nullptr);
		return this->tail->data;
	}
	std::string &back() {
		assert(this->head != nullptr);
		return this->head->data;
	}
	/*
	 * Returns the first element
	 */
	const Node *begin() const {
		return this->head;
	}
	Node *begin() {
		return this->head;
	}
	/*
	 * Returns nullptr
	 */
	const Node *end() const {
		return nullptr;
	}
	Node *end() {
		return nullptr;
	}
};
}
