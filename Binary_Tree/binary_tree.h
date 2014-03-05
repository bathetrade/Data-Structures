#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <queue>
#include <cmath>  //log2()

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
	void CreateInorderQueue(std::queue<node<T>*>& q, node<T>* cur) {
		if (cur != nullptr) {
			CreateInorderQueue(q, cur->left);
			q.push(cur);
			CreateInorderQueue(q, cur->right);
		}
	}
	
	void BalanceDSW() {
		size_t nodes = EfficientTransformToBackbone();
		if (nodes < 3)
			return;

		//Find the number of nodes m in the "closest" (such that m < nodes) perfectly balanced binary tree
		size_t m = pow(2, static_cast<size_t>(log2(nodes + 1))) - 1;
		size_t numPrerotations = nodes - m;
		
		//Create a fake root that is one higher than the actual root.
		//This is to prevent us from having to handle the root separately.
		//Also, it allows us to set the new root when we're done balancing.
		node<T> fake;
		node<T>* fakeRoot = &fake;
		fakeRoot->right = root;
		node<T>* tempGr = fakeRoot;
		node<T>* tempCh;

		//Do "pre-rotations" in order to decrease the the height of the backbone to m. This is somewhat of an edge case to deal with "extra nodes" (relative to m).
		//Also, this ensures that the last level of the tree has its leaves as far to the left as possible.
		for (int i = 0; i < numPrerotations; ++i) {
			tempCh = tempGr->right->right;
			RotateLeft(tempGr->right, tempCh);
			tempGr = tempGr->right;
		}

		//Do the actual balancing.
		while (m > 1) {
			m /= 2;
			tempGr = fakeRoot;
			for (int i = 0; i < m; ++i) {
				tempCh = tempGr->right->right;
				RotateLeft(tempGr->right, tempCh);
				tempGr = tempGr->right;
			}
		}

		root = fakeRoot->right;
	}

	size_t EfficientTransformToBackbone() {
		
		if (empty())
			return 0;
		
		size_t numNodes = 1;
		//First, eliminate the tree's left subtree.
		while (root->left != nullptr) {
			RotateRight(root, root->left);
		}
		
		//Then, transform the remainder of the tree into a backbone.
		node<T>* temp = root;
		while (temp->right != nullptr) {				//Iterate over the backbone
			node<T>* nextNodeLeftSubtree = temp->right->left;
			while (nextNodeLeftSubtree != nullptr) {		//Transform any "left-over" subtrees
				RotateRight(temp->right, nextNodeLeftSubtree);
				nextNodeLeftSubtree = temp->right->left;
			}

			++numNodes;
			temp = temp->right;
		}

		return numNodes;
	}
	
	void TransformToBackbone() {

		if (empty())
			return;

		//Get "sorted" queue from tree; makes it easier to create backbone.
		//Inefficient in space, but easy to understand.
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
		if (child != parent->left)		//This algorithm only works if child is a left subtree
			return false;

		parent->left = child->right;
		child->right = parent;
		parent = child;
		return true;
	}

	bool RotateLeft(node<T>*& parent, node<T>* child) {
		if (parent == nullptr || child == nullptr)
			return false;
		if (child != parent->right)		//Only works if child is parent's right subtree
			return false;
		
		parent->right = child->left;
		child->left   = parent;
		parent        = child;
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
		BalanceDSW();
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
