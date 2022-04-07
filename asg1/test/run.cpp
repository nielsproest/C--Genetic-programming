#include "../src/List.hpp"
#include "../src/Tree.hpp"
#include <cassert>

//Test general functionality
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
//Add and read alot of elements
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
//Test regular type-ness
void dlst_default() {
	DM852::List lst, c;
	lst.push_back("BLOOD");
	lst.push_back("SHED");

	c = lst;
	assert(c.begin()->data == "BLOOD");
	assert(c.begin()->next->data == "SHED");

	assert(&lst != &c);
	assert(lst == c);
	assert(!(lst != c));
}
void dlst_default2() {
	DM852::List lst;
	lst.push_back("BLOOD");
	lst.push_back("SHED");
	DM852::List c(lst);
	
	assert(lst == c);
}

//Test general functionality
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
//Add and read alot of elements
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
//Test regular type-ness
void bst_default() {
	DM852::Tree lst, c;
	lst.insert(0,"BLOOD");
	lst.insert(1,"SHED");

	c = lst;
	assert(c.begin()->value == "BLOOD");
	assert(c.begin()->next()->value == "SHED");

	assert(&lst != &c);
	assert(lst == c);
	assert(!(lst != c));
}
void bst_default2() {
	DM852::Tree lst;
	lst.insert(0,"BLOOD");
	lst.insert(1,"SHED");
	DM852::Tree c(lst);
	
	assert(lst == c);
}

int main() {
	std::cout << "Hello" << std::endl;

	//Double linked list
	dlst_test();
	dlst_stress();
	dlst_default();
	dlst_default2();
	//Scapegoat tree tests
	bst_test();
	bst_stress();
	bst_default();
	bst_default2();

	std::cout << "Bye" << std::endl;
	return 0;
}