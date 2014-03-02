#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <queue>

//Testing...does this cause a merge conflict?
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

/*
Pseudo-algorithm:
pass the "next" pointer all the way down to the smallest element (leftmost node)
set the next pointer equal to the current node

*/

template <typename T>
class BinaryTree {
private:
	node<T>* root;
	void CreateInorderQueue(std::queue<node<T>*>& q, node<T>* cur) {
		if (cur != nullptr) {
			CreateInorderQueue(q, cur->left);
			q.push(cur);
			CreateInorderQueue(q, cur->right);
		}
	}
	
	void TransformToBackbone() {

		if (empty())
			return;

		//Get "sorted" queue from tree; makes it easier to create backbone. Not the most efficient way of doing this, since the tree is traversed twice.
		std::queue<node<T>*> q;
		CreateInorderQueue(q, root);

		//Initialize new root
		root = q.front();
		q.pop();
		root->left = nullptr;

		//Create the backbone
		node<T>* temp = root;
		while (!q.empty()) {
			temp->right = q.front();
			q.pop();
			temp = temp->right;
			temp->left = nullptr;
		}
	}

	bool RotateRight(node<T>*& parent, node<T>* child) {  //Rotates parent's left subtree "up"
		if (parent == nullptr || child == nullptr)
			return false;
		if (child != parent->left)						//This algorithm only works if child is a left subtree
			return false;

		parent->left = child->right;
		child->right = parent;
		parent = child;
		return true;
	}


public:
	BinaryTree() : root{ nullptr } {}
	BinaryTree(const BinaryTree<T>&) = delete;    //Disallow copying for now
	BinaryTree(BinaryTree&& b) : root{nullptr} {} //Trivial move constructor; makes no sense to use this.
	~BinaryTree() { clear(); }
	
	void balance() {
		if (empty())
			return;
		TransformToBackbone();
		//Add code here to actually balance the backbone
	}
	
	void clear() {
		if (empty())
			return;
	
		std::queue<node<T>*> q;
		CreateInorderQueue(q, root);
		while (!q.empty()) {
			delete q.front();
			q.pop();
		}
		
		root = nullptr;
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
		node<T>* prev{};
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
