#include <iostream>
#include <utility>
#include <cmath>

#define SCAPEGOAT

//Swaps n's parent's pointer for r
//	n's parent must be calculated before r
//	so this must a macro.
//	(inlines arent real)
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
class Tree {
public:
	class Node {
	public:
		const int key;
		std::string value;
		//Parent pointers are bad m'kay
		Node* parent;
		Node* left;
		Node* right;

		Node(int k, std::string d) : value(d), key(k) {
			this->parent = nullptr;
			this->left = nullptr;
			this->right = nullptr;
		}

		const Node *next() const {
			if (this->right != nullptr) {
				return InorderSuccessor(this->right);
			} else {
				const Node* x = this;
				while (x != nullptr && x->key <= this->key) {
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
				while (x != nullptr && x->key <= this->key) {
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
				while (x != nullptr && x->key >= this->key) {
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
				while (x != nullptr && x->key >= this->key) {
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
#ifdef SCAPEGOAT
	int maxsize;
	double alpha;
#endif

public:
	Tree() {
		this->n = 0;
		this->root = nullptr;
#ifdef SCAPEGOAT
		this->maxsize = 0;
		this->alpha = 0.65;
#endif
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
		int depth=0;

		//Search
		while (x != nullptr) {
			y = x;
			if (key < x->key) {
				x = x->left;
			} else if (key > x->key) {
				x = x->right;
			} else { //equal
				break;
			}
			depth++;
		}

		//Insert
		this->n++;
		if (y != nullptr && key == y->key) {
			y->value = value;
			return std::make_pair(y, false);
		}

		Node* z = new Node(key, value);

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

		this->ScapegoatAdd(z, depth);
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

		return x;
	}
	void clear() {
		auto remove = [] (Node* x) { delete x; };
		this->PostorderTraversal(this->root, remove);
		this->n = 0;
		this->root = nullptr;
#ifdef SCAPEGOAT
		this->maxsize = 0;
#endif
	}
	void erase(int key) {
		this->root = this->DeleteNode(this->root, key);
		this->n--;
		this->ScapegoatRemove();
	}
	void erase(Node* node) {
		CSWAP(node, this->DeleteNode(node, node->key));
		this->n--;
		this->ScapegoatRemove();
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
	std::string &front() {
		return this->begin()->value;
	}
	std::string &back() {
		Node* x = this->root;
		while (x->right != nullptr) {
			x = x->right;
		}
		return x->value;
	}
	Node *begin() {
		Node* x = this->root;
		while (x->left != nullptr) {
			x = x->left;
		}
		return x;
	}
	Node *end() {
		return nullptr;
	}
private:
#ifdef SCAPEGOAT
	Node* Flatten(Node* x, Node* y) {
		if (x == nullptr) {
			return y;
		}
		x->right = this->Flatten(x->right, y);
		return this->Flatten(x->left, x);
	}
	Node* BuildTree(int n, Node* x) {
		if (n == 0) {
			x->left = nullptr;
			return x;
		}
		double val = (((double)n)-1.0)/2.0; //Ensure its double calculations
		Node* r = this->BuildTree(std::ceil(val), x);
		Node* s = this->BuildTree(std::floor(val), r->right); 
		r->right = s->left;
		s->left = r;

		if (r->right != nullptr) {
			r->right->parent = r;
		}
		if (s->left != nullptr) {
			s->left->parent = s;
		}

		return s;
	}
	Node* RebuildTree(Node* root, int n) {
		Node* w = new Node(-1, "");
		Node* z = this->Flatten(root, w);
		this->BuildTree(n,z);
		Node* wleft = w->left;
		delete w;
		return wleft;
	}
	//Calculations
	double AlphaHeightBalance(int s) {
		return std::floor(std::log2(s) / std::log2(1.0/this->alpha));
	}
	int NodeSize(Node* x) {
		if (x == nullptr) {
			return 0;
		} else {
			return this->NodeSize(x->right) + this->NodeSize(x->left) + 1;
		}
	}

	//Binary tree operations
	Node* NodeBrother(Node* parent, Node* x) {
		if (parent == nullptr) {
			return nullptr;
		}

		return parent->right == x ? parent->left : parent->right;
	}
#endif
	void ScapegoatAdd(Node* y, int depth) {
#ifdef SCAPEGOAT
		this->maxsize = std::max(this->n, this->maxsize);

		Node* scapegoat = y;
		if (depth > this->AlphaHeightBalance(this->n + 1)) {
			int tsize = 1;
			int rsize = 0;
			int lsize = 0;

			do {
				Node* oldgoat = scapegoat;
				scapegoat = scapegoat->parent;

				lsize = tsize;
				//This isn't a comparison (even though its expensive)
				rsize = this->NodeSize(
					this->NodeBrother(
						scapegoat,
						oldgoat
					)
				);

				tsize = rsize + lsize + 1;
			} while (scapegoat->parent != nullptr && lsize <= this->alpha*(tsize-1) && rsize <= this->alpha*(tsize-1));

			Node* parent_backup = scapegoat->parent;
			Node* rebuild = this->RebuildTree(scapegoat, tsize);

			if (parent_backup == nullptr) {
				this->root = rebuild;
			} else if (parent_backup->left == scapegoat) {
				parent_backup->left = rebuild;
			} else {
				parent_backup->right = rebuild;
			}
			rebuild->parent = parent_backup;
		}
#endif
	}
	void ScapegoatRemove() {
#ifdef SCAPEGOAT
		if (this->n < this->alpha * this->maxsize) {
			this->root = this->RebuildTree(this->root, this->n);
			this->maxsize = this->n;
		}
#endif
	}
	//Swaps n1 and n2
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
