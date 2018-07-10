#include <iostream>


class PtrClass {
public:
	PtrClass();
	void show(int);
	~PtrClass();
};

class SharedPtr {
public:
	int a;
	std::shared_ptr<SharedPtr> shrptr;

	SharedPtr();
	SharedPtr(const SharedPtr&);
	~SharedPtr();
};

PtrClass::PtrClass() {
	std::cout << "Constructor" << std::endl;
}

PtrClass::~PtrClass() {
	std::cout << "Destructor" << std::endl;
}

void PtrClass::show(int x) {
	std::cout << x << std::endl;
}



SharedPtr::SharedPtr() {
	std::cout << "Constructor for shared_ptr" << std::endl;
	a = 0;
}

SharedPtr::SharedPtr(const SharedPtr& item) {
	a = item.a;
	shrptr = item.shrptr;
}

SharedPtr::~SharedPtr() {
	std::cout << "Destructor for shared_ptr" << std::endl;
}

class WeakPtr {
public:
	int a;
	int id;
	std::weak_ptr<WeakPtr> wkptr;

	WeakPtr(int i) { std::cout << "Constructor for weak_ptr" << std::endl; a = 1; id = i; }
	WeakPtr(const WeakPtr& item) { a = item.a; wkptr = item.wkptr; }
	~WeakPtr() { std::cout << "Destructor for weak_ptr " << id << std::endl; }
};


int main() {
//	auto p = new PtrClass;
//	auto up = std::unique_ptr<PtrClass>(p);


//	SharedPtr item1, item2;
////	item1.shrptr = std::shared_ptr<SharedPtr>(new SharedPtr(item2));
//	
//	item1.shrptr = std::make_shared<SharedPtr>(item2);
//	item2.shrptr = std::make_shared<SharedPtr>(item1);

	WeakPtr item3(3), item4(4);

	item3.wkptr = std::make_shared<WeakPtr>(item4);

	item4.wkptr = std::make_shared<WeakPtr>(item3);


	//std::cout << it em3.wkptr.use_count() << std::endl;
	//std::cout << item4.wkptr.use_count() << std::endl;

//	auto _up = std::unique_ptr<PtrClass>(p);
	//(*up).show(5);
	//(*_up).show(6);

	

	system("pause");
	return 0;
}