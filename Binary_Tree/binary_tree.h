#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <queue>

template <typename T>
struct node {
	T data;
	node<T>* left;
	node<T>* right;

	node() : data{}, left{}, right{} {}
	~node() {}											//Don't delete pointer because it can be shared (cringe)
	node(const T& el) : data{ el }, left{}, right{}	{}						//Copy the data, next=nullptr
	node(T&& el) : data{ static_cast<T&&>(el) }, left{}, right{} {}					//Move the data
	node(const node<T>& n) : data{ n.data }, left{n.left}, right{n.right} {}			//Copy the data and the pointer (gross)
	node(node<T>&& n) : data{ static_cast<T&&>(n.data) }, left{n.left}, right{n.right} {
		n.left  = nullptr;
		n.right = nullptr;
	}
	node<T>& operator=(const node<T>& n) {
		data  = n.data;
		left  = n.left;
		right = n.right;
		return *this;
	}
	node<T>& operator=(node<T>&& n) {
		data    = static_cast<T&&>(n.data);	//Could also use std::move()
		left    = n.left;
		right   = n.right;
		
		n.left  = nullptr;
		n.right = nullptr;
	}
};

template <typename T>
class BinaryTree {
private:
	node<T>* root;
public:
	BinaryTree() : root{ nullptr } {}
	BinaryTree(const BinaryTree<T>&) = delete;    //Disallow copying for now
	BinaryTree(BinaryTree&& b) : root{nullptr} {} //Trivial move constructor; makes no sense to use this.
	~BinaryTree() { clear(); }
	
	void clear() {
		if (empty())
			return;
	
		std::queue<node<T>*> q;
		node<T>* temp{};
		q.push(root);
		while (!q.empty()) {
			temp = q.front();
			q.pop();
			if (temp->left != nullptr)
				q.push(temp->left);
			if (temp->right != nullptr)
				q.push(temp->right);
			delete temp;
		}
	}
	
	bool empty() const { return root==nullptr; }
	
	void inorderPrint(const node<T>* n) const {
		
		if (n != nullptr) {
			inorderPrint(n->left);
			std::cout << n->data << " ";
			inorderPrint(n->right);
		}
		
		return;
	}

	bool insert(const T& el) {
		
		if (empty()) {
			root = new node<T>(el);
			return true;
		}
		
		//Find a leaf node
		node<T>* temp = root;
		node<T>* prev;
		while (temp) {
			prev = temp;
			if (el < temp->data)
				temp = temp->left;
			else if ( el > temp->data)
				temp = temp->right;
			else return false;		//No duplicate elements
		}
		
		//Create a new node
		if (el < prev->data)
			prev->left = new node<T>(el);
		else prev->right   = new node<T>(el);
		
		return true;
	}

	bool insert(T&& el) {
		
		if (empty()) {
			root = new node<T>(el);
			return true;
		}
		
		//Find a leaf node
		node<T>* temp = root;
		node<T>* prev;
		while (temp) {
			prev = temp;
			if (el < temp->data)
				temp = temp->left;
			else if ( el > temp->data)
				temp = temp->right;
			else return false;		//No duplicate elements
		}
		
		//Create a new node
		if (el < prev->data)
			prev->left = new node<T>(std::move(el));
		else prev->right   = new node<T>(std::move(el));
		
		return true;
	}
	
	void print() const  {
		if (empty())
			std::cout << "Empty tree\n";
		else inorderPrint(root);
	}

	bool remove(const T& el) {
		return true;
	}
	
	bool remove(T&& el) {
		return true;
	}
	
	bool search(const T& el) {
		node<T>* temp = root;
		while (temp) {
			if (el < temp->data)
				temp = temp->left;
			else if (el > temp->data)
				temp = temp->right;
			else return true;
		}
		return false;
	}

	void operator=(const BinaryTree<T>&) = delete;
	void operator=(BinaryTree<T>&&)      = delete;
};

#endif
