#include "pch.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>

/*
The most basic entity structure is Node. Node is declared inside protected part of BinaryTree class as struct.
Each node has value, link to parent, left and right node. In public section of BinaryTree class we define Position.
He's data members consist of Node pointer to a Tree. Access to node's value is provided with overloaded
dereferencing operatr ("*"). BinaryTree is declared as friend. Functions parent(), left(), right() are used to
accessed apropriate member of a Node structure. List declaration is used to represent list of nodes.
There are some update methods inside BinaryTree. addRoot(), expandExternal() and removeAboveExternal()
BinaryTree private part consist of Node pointer (Node* root) and number of nodes (int n).
*/

template <typename T>
class BinaryTree {

protected:	//only BinaryTree and derived class can access Node
	struct Node {
		T value;	//element
		Node* parent;	//link to parent node
		Node* left;		//link to left node
		Node* right;	//lin to right node
		Node(T val=0, Node* prnt = NULL, Node* lft = NULL, Node* rgt = NULL)	//constructor
			:value(val), parent(prnt), left(lft), right(rgt) {};
	};

public:
	class Position {
	public:
		Position(Node* x = NULL);	//constructor
		T& operator*();		//access element
		Position parent() const;	//get parent
		Position left() const;	//get left chid
		Position right() const;	//get right child
		bool isRoot() const;	//check if node is a root
		bool isExternal() const;	//check if node is external
		friend class BinaryTree<T>; //BinaryTree can acces private members
	private:
		Node* v; //pointer node
	};
	typedef std::list<Position> PositionList;	 //list of positions

	BinaryTree();
	BinaryTree(const BinaryTree& T);
	BinaryTree& operator=(BinaryTree T);
	~BinaryTree();
	Position getRoot() const; //get the root
	PositionList positions() const; //get list of nodes
	void addRoot();		//add root to empty tree
	void expandEternal(const Position& p);	//expand external Node
	Position removeAboveExternal(const Position& p); //remove p and parent
	int depth(Position p);
	int height(Position p);
	void preorderPrint(Position p);
	void postorderPrint(Position p);
	void inorderPrint(Position p);
	void externalLeft(Position p, int& i);

protected:
	void preorder(Node* v, PositionList& pl) const;
	int size() const;	//check number of nodes
	bool isEmpty() const;	//check if tree is empty
	Node* clone(Node*  p) const;
	void emptyTree(Position p);

private:
	Node* root;	//root pointer to first node
	int n;	//number of nodes 
};


int main()
{

	BinaryTree<int> T;
	T.addRoot();	//create's root node 

	BinaryTree<int>::Position p = T.getRoot();
	*p = 313;
	T.expandEternal(p);
	*(p.left()) = 5;
	*(p.right()) = 2;
	T.expandEternal(p.left());
	*(p.left().left()) = 6;
	*(p.left().right()) = 9;
	T.expandEternal(p.right());
	*(p.right().left()) = 3;
	*(p.right().right()) = 7;

	int x = T.depth(p.right());
	T.preorderPrint(p);
	std::cout << " ";
	T.postorderPrint(p);
	std::cout << " ";
	T.inorderPrint(p);
	p = T.getRoot();
	x = 0; 
	T.externalLeft(p, x); 
	std::cout << "\n number of left external elements: "; 
	std::cout << x; 
	p = T.getRoot(); 
	x = T.height(p); 
	std::cout << "\n height: " << x; 
	x = T.depth(p.right().left()); 
	std::cout << "\n depth: " << x; 

 
	BinaryTree<int> L = T;
	std::cout << "\n"; 
	L.display(); 
	std::cout << "\n"; 
	T.display(); 
	p = L.getRoot(); 
	*p = 1; 
	std::cout << "\n"; 
	std::cout << *T.getRoot(); 
	std::cout << "\n";
	std::cout << *L.getRoot();

	return 0;
}
/*
Position methods
*/
template <typename T>
BinaryTree<T>::Position::Position(Node* x) {	//construct position node
	this->v = x;
}

template <typename T>
T& BinaryTree<T>::Position::operator*() {	//access value
	return v->value;
}

template <typename T>
typename BinaryTree<T>::Position BinaryTree<T>::Position::left() const {	//construct position on left link
	return Position(v->left);
}

template <typename T>
typename BinaryTree<T>::Position BinaryTree<T>::Position::right() const { //construct position on right link
	return Position(v->right);
}

template <typename T>
typename BinaryTree<T>::Position BinaryTree<T>::Position::parent() const { //construct position on parent
	return Position(v->parent);
}
/*
no param
check if node is external
*/
template <typename T>
bool BinaryTree<T>::Position::isExternal() const {
	return (v->left == NULL && v->right == NULL);
}
/*
BinaryTree methods declarations
*/
template <typename T>
BinaryTree<T>::BinaryTree()
	:root(NULL), n(0) {};

template <typename T>
BinaryTree<T>::~BinaryTree() {	//destroy whole tree
	emptyTree(getRoot());
}
template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& T)	//copy constrcutor
	:root(NULL), n(0) {
	root = clone(T.getRoot().v); 
}
template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree T) {	//assignment operator
	if (this != &T) {
		if (root != NULL) emptyTree(root); 
		root = clone(T.getRoot().v); 
	}
	return *this; 
}
/*
1 param: position
visit left side, then right side
clone Tree
*/
template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::clone(Node* p) const{	//clone with postorder
	if (p == NULL) {
		return p;
	}
	return new Node(p->value, p->parent, clone(p->left), clone(p->right));
}
/*
param position
clean whole tree with recursive calls
no return value
*/
template <typename T>
void BinaryTree<T>::emptyTree(Position p) {
	if (p.v != NULL) {
		emptyTree(p.left());
		emptyTree(p.right());
		delete p.v;
	}
	root = NULL;
}
/*
no param
check if node is root
return bool
*/
template <typename T>
bool BinaryTree<T>::Position::isRoot() const {
	return (v->parent == NULL);
}
/*
no param
number of nodes
return int 
*/
template <typename T>
int BinaryTree<T>::size() const {
	return n;
}

/*
no param
checks if tree is empty
return bool
*/
template <typename T>
bool BinaryTree<T>::isEmpty() const {
	return (n == 0);
}
/*
no param
construct Position on root
return Position
*/
template <typename T>
typename BinaryTree<T>::Position BinaryTree<T>::getRoot() const {
	return Position(root);
}
/*
no param
add's root, set n to 1
no ret value
*/
template <typename T>
void BinaryTree<T>::addRoot() {
	root = new Node;
	n = 1;
}
/*
1 param: const referance to position
creates two siblings
no ret value
*/
template <typename T>
void BinaryTree<T>::expandEternal(const Position& p) {	//expand external node
	Node* v = p.v;			//set pointer to external position
	v->left = new Node;		//create left node
	v->left->parent = v;	//set left's parent
	v->right = new Node;	//create right node
	v->right->parent = v;	//set right's parent
	n += 2;			//increment number of nodes by two
}
/*
1 param: const referance to Position
delete Position Node and he's parent
return new Position
*/
template <typename T>
typename BinaryTree<T>::Position BinaryTree<T>::removeAboveExternal(const Position& p) {	//remove p and p's parent
	Node* position = p.v;
	Node* parent = position->parent;
	Node* sibling = (position == parent->left) ? parent->right : parent->left;
	if (parent == root) {	//if p is child of root
		root = sibling;		//sibling is new root
		sibling->parent = NULL;		//root dont have parents
	}
	else {	//if p is not child of the node
		Node* gparent = parent->parent;		//take he's grandparent
		if (gparent->left == parent) {	//if parent node is left child
			gparent->left = sibling;	//sibling is now left child
		}
		else {	//else if parent node is right child
			gparent->right = sibling;	//sibling is now right child
		}
		sibling->parent = gparent; //and sibling's parent is granparent of node that will be deleted
	}
	delete position; //delete p node
	delete parent;	//delete he's parent
	n -= 2;		//reduce number of nodes
	return Position(sibling);
}
/*
2 params: pointer to Node, referance to List
fill list with nodes
reursive calls through whole list
no return value
*/
template <typename T>
void BinaryTree<T>::preorder(Node* v, PositionList& L) const {	//list of all node
	L.push_back(Position(v)); //add this node
	if (v->left != NULL) {
		preorder(v->left, L);	//recursive call for left side of the tree 
	}
	if (v->right != NULL) {
		preorder(v->right, L); //recursivve call for right call of the tree
	}
}
/*
no param
creates list, fill it with nodes
return PositionList
*/
template <typename T>
typename BinaryTree<T>::PositionList BinaryTree<T>::positions() const {	//list of all node
	PositionList L;
	preorder(root, L);	//traverse the tree and stores nodes inside list
	return PositionList(L);	//return resulting list
}
/*
1 param: Position
compute depth of Node
return int
*/
template <typename T>
int BinaryTree<T>::depth(Position p) {
	if (p.isRoot()) {
		return 0;
	}
	else {
		return 1 + depth(p.parent());
	}
}

template <typename T>
void BinaryTree<T>::preorderPrint(Position p) {	//root is visited first and then the subtrees as children
	std::cout << *p << " ";						//are visited recursively
	if (!p.isExternal()) {
		preorderPrint(p.left());
		preorderPrint(p.right());
	}
}

template <typename T>
void BinaryTree<T>::postorderPrint(Position p) {	//visits children of tree first, then visit the root
	if (!p.isExternal()) {
		postorderPrint(p.left());
		postorderPrint(p.right());
	}
	std::cout << *p << " ";
}
template <typename T>
void BinaryTree<T>::inorderPrint(Position p) {	//with inorder traversal, node is visited between recursive calls
	if (!p.isExternal()) {
		inorderPrint(p.left());
	}
	std::cout << *p << " ";		//node visit
	if (!p.isExternal()) {
		inorderPrint(p.right());
	}
}
/*
Algorithm for counting the number of left external nodes in a
binary tree, using the Binary tree ADT.
*/
template <typename T>
void BinaryTree<T>::externalLeft(Position p, int& i) {
	if (p.v == NULL) return;

	if (p.isExternal() && p.parent().left().v == p.v) {
		i++;
	}
	externalLeft(p.left(), i);
	externalLeft(p.right(), i);
}
/*
return height of tree
argument position
test if its NULL,
*/
template <typename T>
int BinaryTree<T>::height(Position p) {
	if (p.v == NULL) {
		return -1;
	}
	return 1 + std::max(height(p.left()), height(p.right()));
}

