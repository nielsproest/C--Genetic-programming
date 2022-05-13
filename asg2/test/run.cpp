#include "../src/List.hpp"
#include "../src/Tree.hpp"
#include <cassert>
#include <iterator>

//Test general functionality
void dlst_test() {
	DM852::List<std::string> lst;

	assert(lst.empty());

	lst.push_back("a");
	assert(lst.size() == 1);
	lst.push_back("c");
	assert(lst.size() == 2);

	assert(!lst.empty());

	auto first = lst.begin();
	assert(*first == "a");
	assert(*(++first) == "c");

	lst.insert(first, "b");
	assert(lst.size() == 3);

	first = lst.begin();
	assert(*first == "a");
	assert(*(++first) == "b");
	assert(*(++first) == "c");

	/*lst.erase(first->next);
	assert(first->data == "a");
	assert(first->next->data == "c");

	lst.pop_back();
	assert(first->data == "a");
	assert(first->next == nullptr);*/

	lst.clear();
	assert(lst.empty());
}
//Add and read alot of elements
void dlst_stress() {
	DM852::List<std::string> lst;

	for (int i=0; i<1000; i++) {
		std::string s = std::to_string(i);
		lst.push_back(s);
	}

	auto first = lst.begin();
	for (int i=0; i<1000; i++) {
		std::string s = std::to_string(i);
		assert(*first == s);
		++first;
	}

	lst.clear();
	assert(lst.empty());
}
//Test regular type-ness
void dlst_default() {
	DM852::List<std::string> lst, c;
	lst.push_back("BLOOD");
	lst.push_back("SHED");

	c = lst;
	assert(*c.begin() == "BLOOD");
	assert(*++c.begin() == "SHED");

	assert(&lst != &c);
	assert(lst == c);
	assert(!(lst != c));
}
void dlst_default2() {
	DM852::List<std::string> lst;
	lst.push_back("BLOOD");
	lst.push_back("SHED");
	DM852::List<std::string> c(lst);

	assert(lst == c);
}

template <typename T>
void dlst_directionaltest(T i) requires std::bidirectional_iterator<T> {
	i++;
	std::cout << *i << std::endl;
}

void dlst_directional() {
	DM852::List<int> fun;
	fun.push_back(5);
	fun.push_back(6);
	dlst_directionaltest(fun.begin());

	DM852::List<std::string> fun2;
	fun2.push_back("hey");
	fun2.push_back("you");
	dlst_directionaltest(fun2.begin());
}

//Test general functionality
void bst_test() {
	DM852::Tree<int,std::string> lst;

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

	auto first = lst.begin();
	for (int i=0; i<3; i++) {
		char ch = (char)(i+97);
		std::string st = {ch};

		assert((*first).second == st);
		++first;
	}

	assert((*lst.find(1)).second == "b");

	lst.erase(0);
	assert((*++lst.begin()).second == "b");
	//assert((*++lst.begin()).second == "c");

	lst.clear();
	assert(lst.empty());
}
//Add and read alot of elements
void bst_stress() {
	DM852::Tree<int,std::string> lst;

	for (int i=0; i<10000; i++) {
		lst.insert(i, "");
	}

	auto first = lst.begin();
	for (int i=0; i<10000; i++) {
		assert((*first).first == i);
		++first;
	}

	lst.clear();
	assert(lst.empty());
}
//Test regular type-ness
void bst_default() {
	DM852::Tree<int,std::string> lst, c;
	lst.insert(0,"BLOOD");
	lst.insert(1,"SHED");

	c = lst;
	assert((*c.begin()).second == "BLOOD");
	assert((*++c.begin()).second == "SHED");

	assert(&lst != &c);
	assert(lst == c);
	assert(!(lst != c));
}
void bst_default2() {
	DM852::Tree<int,std::string> lst;
	lst.insert(0,"BLOOD");
	lst.insert(1,"SHED");
	DM852::Tree<int,std::string> c(lst);
	
	assert(lst == c);
}

template <typename T>
void bst_directionaltest(T i) requires std::bidirectional_iterator<T> {
	i++;
	auto d = *i;
	std::cout << d.first << std::endl;
}

void bst_directional() {
	DM852::Tree<int,int> t;
	t.insert(0,0);
	t.insert(1,0);
	bst_directionaltest(t.begin());

	auto m = t.end();
	auto n = --m;
	auto k = *n;
	std::cout << k.first << std::endl;
}

int main() {
	std::cout << "Hello" << std::endl;

	//Double linked list
	dlst_test();
	dlst_stress();
	dlst_default();
	dlst_default2();
	dlst_directional();
	//Scapegoat tree tests
	bst_test();
	bst_stress();
	bst_default();
	bst_default2();
	bst_directional();

	std::cout << "Bye" << std::endl;
	return 0;
}
