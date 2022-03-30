
#include <iostream>

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
	~List() {
		this->clear();
	}
	int size() const {
		return this->n;
	}
	bool empty() const {
		return this->n == 0;
	}
	void push_back(const std::string &elem) {
		Node* p = new Node();
		p->data = elem;
		p->next = nullptr;
		p->prev = nullptr;

		if (this->head == nullptr) {
			this->head = p;
			this->tail = p;
			return;
		}

		Node* last = this->tail;
		p->prev = last;
		last->next = p;
		this->tail = p;

		this->n++;
	}
	Node* insert(Node* node, const std::string &elem) {
		Node* p = new Node();
		p->data = elem;
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
	const std::string &front() const {
		return this->tail->data;
	}
	const std::string &back() const {
		return this->head->data;
	}
	const Node *begin() const {
		return this->head;
	}
	const Node *end() const {
		return nullptr;
	}
	std::string &front() {
		return this->tail->data;
	}
	std::string &back() {
		return this->head->data;
	}
	Node *begin() {
		return this->head;
	}
	Node *end() {
		return nullptr;
	}
};
}

int main() {
	return 0;
}
