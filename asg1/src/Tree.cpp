#include <iostream>
#include <utility>

namespace DM852 {
class Tree {
	class Node {
	public:
		const int key;
		std::string value;
		Node* parent;
		Node* left;
		Node* right;

		Node(int k) : key(k) {}

		const Node *next() const {
			if (this->right != nullptr) {
				return InorderSuccessor(this->right);
			} else {
				const Node* x = this;
				while (x->key <= this->key) {
					x = x->parent;
				}
				return x;
			}
		} 
		Node *next() {
			if (this->right != nullptr) {
				return InorderSuccessor(this->right);
			} else {
				Node* x = this;
				while (x->key <= this->key) {
					x = x->parent;
				}
				return x;
			}
		}
		const Node *prev() const {
			if (this->left != nullptr) {
				return InorderPredecessor(this->left);
			} else {
				const Node* x = this;
				while (x->key >= this->key) {
					x = x->parent;
				}
				return x;
			}
		}
		Node *prev() {
			if (this->left != nullptr) {
				return InorderPredecessor(this->left);
			} else {
				Node* x = this;
				while (x->key >= this->key) {
					x = x->parent;
				}
				return x;
			}
		}
	};

private:
	//No touchy
	int n;
	Node* root;

public:
	Tree() {
		this->n = 0;
		this->root = nullptr;
	}
	~Tree() {
		this->clear();
	}

	int size() const {
		return this->n;
	}
	bool empty() const {
		return this->n == 0;
	}
	std::pair<Node*, bool> insert(int key, const std::string &value) {
		Node* x = this->root;
		Node* y = nullptr;

		//Search
		while (x != nullptr) {
			y = x;
			if (key < x->key) {
				x = x->left;
			} else if (key > x->key) {
				x = x->right;
			} else {
				break;
			}
		}

		//Insert
		this->n++;
		if (key == y->key) {
			y->value = value;
			return std::make_pair(y, false);
		}

		Node* z = new Node(key);
		z->value = value;

		if (y == nullptr) {
			this->root = z;
			z->parent = nullptr;
		} else if (key < y->key) {
			y->left = z;
			z->parent = y;
		} else {
			y->right = z;
			z->parent = y;
		}

		return std::make_pair(z, true);
	}
	const Node *find(int key) const {
		Node* x = this->root;

		//Standard search
		while (x != nullptr && key != x->key) {
			if (key < x->key) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		if (x == nullptr) {
			return nullptr;
		} else {
			return x;
		}
	}
	void clear() {
		auto remove = [] (Node* x) { delete x; };
		this->PostorderTraversal(this->root, remove);
		this->n = 0;
		this->root = nullptr;
	}
	void erase(int key) {
		this->root = this->DeleteNode(this->root, key);
		this->n--;
	}
	void erase(Node* node) {
		this->pswap(node, this->DeleteNode(this->root, node->key));
		this->n--;
	}
	const std::string &front() const {
		return this->begin()->value;
	}
	const std::string &back() const {
		Node* x = this->root;
		while (x->right != nullptr) {
			x = x->right;
		}
		return x->value;
	}
	const Node *begin() const {
		Node* x = this->root;
		while (x->left != nullptr) {
			x = x->left;
		}
		return x;
	}
	const Node *end() const {
		return nullptr;
	}
private:
	//Swaps n's parent's pointer for r
	void pswap(Node* n, Node* r) {
		if (n->parent != nullptr) {
			if (n->parent->left == n) {
				n->parent->left = r;
			} else {
				n->parent->right = r;
			}
		} else {
			this->root = r;
		}
		r->parent = n->parent;
	}
	//Swaps n1 and n2
	void swap(Node* n1, Node* n2) {
		Node* n1left = n1->left;
		Node* n1right = n1->right;
		Node* n2left = n2->left;
		Node* n2right = n2->right;

		n1->left = n2left;
		n1->right = n2right;
		n2->left = n1left;
		n2->right = n2right;

		this->pswap(n1, n2);
		this->pswap(n2, n1);
	}
	//Big and heavy function
	Node* DeleteNode(Node* x, int searchKey) {
		if (x == nullptr) {
			return nullptr;
		}

		if (x->key < searchKey) {
			x->right = this->DeleteNode(x->right, searchKey);
		} else if (x->key > searchKey) {
			x->left = this->DeleteNode(x->left, searchKey);
		} else {
			if (x->right == nullptr && x->left == nullptr) {
				delete x;
				return nullptr;
			} else if (x->right == nullptr) {
				Node* tmp = x->left;
				delete x;
				return tmp;
			} else if (x->left == nullptr) {
				Node* tmp = x->right;
				delete x;
				return tmp;
			} else {
				Node* min_but_bigger = this->InorderSuccessor(x->right);
				/*x->key = min_but_bigger->key;
				x->value = min_but_bigger->value;*/
				this->swap(min_but_bigger, x);
				x->right = this->DeleteNode(x->right, x->key);
			}
		}

		return x;
	}
	static Node* InorderSuccessor(Node* x) {
		Node* min = x;
		while (min->left != nullptr) {
			min = min->left;
		}
		return min;
	}
	static Node* InorderPredecessor(Node* x) {
		Node* max = x;
		while (max->right != nullptr) {
			max = max->right;
		}
		return max;
	}

	//In-order Traversal
	template<typename K>
	void InorderTraversal(Node* x, K&& func) {
		if (x == nullptr) {
			return;
		}

		this->InorderTraversal(x->left, func);
		func(x);
		this->InorderTraversal(x->right, func);
	}
	//Post-order traversal of tree
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

int main() {
	return 0;
}