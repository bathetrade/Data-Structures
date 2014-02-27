#ifndef BINARY_TREE_H
#define BINARY_TREE_H

template <typename T>
struct node {
	T data;
	node<T>* next;

	node() : data{}, next{} {}
	~node() {}																					//Don't delete pointer because it can be shared (cringe)
	node(const T& el) : data{ el }, next{}	{}												    //Copy the data, next=nullptr
	node(T&& el) : data{ static_cast<T&&>(el) }, next{} {}										//Move the data
	node(const node<T>& n) : data{ n.data }, next{ n.next } {}									//Copy the data and the pointer (gross)
	node(node<T>&& n) : data{ static_cast<T&&>(n.data) }, next{ n.next }, n.next{ nullptr } {}	//Move the node to this node
	node<T>& operator=(const node<T>& n) {
		data = n.data;
		next = n.next;
		return *this;
	}
	node<T>& operator=(node<T>&& n) {
		data = static_cast<T&&>(n.data);
		next = n.next;
		n.next = nullptr;
	}
};

template <typename T>
class BinaryTree {
private:
	node<T>* root;
public:
	BinaryTree() : root{ nullptr } {}

};

#endif