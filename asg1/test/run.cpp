#include "../src/List.hpp"
#include "../src/Tree.hpp"
#include <cassert>

void dlst_test() {
	DM852::List lst;

	assert(lst.empty());

	lst.push_back("a");
	assert(lst.size() == 1);
	lst.push_back("c");
	assert(lst.size() == 2);

	assert(!lst.empty());

	auto first = lst.begin();
	assert(first->data == "a");
	assert(first->next->data == "c");

	lst.insert(first->next, "b");
	assert(lst.size() == 3);
	assert(first->data == "a");
	assert(first->next->data == "b");
	assert(first->next->next->data == "c");

	lst.erase(first->next);
	assert(first->data == "a");
	assert(first->next->data == "c");

	lst.pop_back();
	assert(first->data == "a");
	assert(first->next == nullptr);

	lst.clear();
	assert(lst.empty());
}
void dlst_stress() {
	DM852::List lst;

	for (int i=0; i<1000; i++) {
		std::string s = std::to_string(i);
		lst.push_back(s);
	}

	auto* first = lst.begin();
	for (int i=0; i<1000; i++) {
		std::string s = std::to_string(i);
		assert(first->data == s);
		first = first->next;
	}

	lst.clear();
	assert(lst.empty());
}

void bst_test() {
	DM852::Tree lst;

	assert(lst.size() == 0);
	assert(lst.empty());
	lst.insert(0, "a");

	assert(lst.size() == 1);
	assert(!lst.empty());

	for (int i=1; i<3; i++) {
		char ch = (char)(i+97);
		std::string st = {ch};
		lst.insert(i, st);
	}
	assert(lst.size() == 3);

	auto* first = lst.begin();
	for (int i=0; i<3; i++) {
		char ch = (char)(i+97);
		std::string st = {ch};

		assert(first->value == st);
		first = first->next();
	}

	assert(lst.find(1)->value == "b");

	lst.erase(0);
	assert(lst.begin()->value == "b");
	assert(lst.begin()->right->value == "c");

	lst.clear();
	assert(lst.empty());
}
void bst_stress() {
	DM852::Tree lst;

	for (int i=0; i<10000; i++) {
		lst.insert(i, "");
	}

	auto* first = lst.begin();
	for (int i=0; i<10000; i++) {
		assert(first->key == i);
		first = first->next();
	}

	lst.clear();
	assert(lst.empty());
}

int main() {
	std::cout << "Hello" << std::endl;
	dlst_test();
	dlst_stress();
	bst_test();
	bst_stress();
	std::cout << "Bye" << std::endl;
	return 0;
}