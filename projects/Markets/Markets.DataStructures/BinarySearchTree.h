#pragma once

template <typename KeyType, typename ValueType>
class BSTNode
{
public:
	KeyType key;
	ValueType value;
	BSTNode* leftChild;
	BSTNode* leftChild;
};


template <typename KeyType, typename ValueType>
class BSTree
{ 
private: 
	BSTNode* root;
public:
	BSTree();
	~BSTree();

	void InsertNode(KeyType key);
	void InsertNode(BSTNode<KeyType, ValueType>* node, KeyType key);

	void RemoveNode(KeyType key);
	BSTNode<KeyType, ValueType>* RemoveNode(BSTNode<KeyType, ValueType>* node, KeyType key);

	void Search(KeyType key);
	BSTNode<KeyType, ValueType>* Search(BSTNode<KeyType, ValueType>* node, KeyType key);
	
	bool IsEmpty();

	BSTNode<KeyType, ValueType>* GetSuccessor(BSTNode<KeyType, ValueType>* node);
};


template <typename KeyType, typename ValueType>
class BSTReverseIterator
{
public:
	BSTReverseIterator(BSTNode<KeyType, ValueType>* root);
	~BSTReverseIterator();

	virtual BSTNode<KeyType, ValueType>* curr();
	virtual BSTNode<KeyType, ValueType>* prev();
	virtual bool isEnd();
};

template <typename KeyType, typename ValueType>
class BSTForwardIterator
{
public:
	virtual BSTNode<KeyType, ValueType>* curr();
	virtual BSTNode<KeyType, ValueType>* next();
	bool isEnd();
};

///
/// Implementation
/// 


template <typename KeyType, typename ValueType>
BSTree<KeyType, ValueType>::BSTree() 
{
	root = nullptr;
}


template <typename KeyType, typename ValueType>
BSTree<KeyType, ValueType>::~BSTree() 
{
	delete root;
}


template <typename KeyType, typename ValueType>
void BSTree<KeyType, ValueType>::InsertNode(KeyType key) 
{
	BSTNode* newNode = new BSTNode();
	newNode->key = key;
	newNode->leftChild = nullptr;
	newNode->rightChild = nullptr;

	if (root == nullptr) {
		root = newNode;
		return;
	}
	else {
		InsertNode(newNode, key);
	}
}

template <typename KeyType, typename ValueType>
void BSTree<KeyType, ValueType>::InsertNode(BSTNode<KeyType, ValueType>* node, KeyType key)
{
	if (node->key == key) {
		return;
	}
	if (node->key > key) {
		// Right insertion
		node->rightChild = InsertNode(node->rightChild, key);
	} 
	else {
		// left insertion
		node->leftChild = InsertNode(node->leftChild, key);
	}
}

template <typename KeyType, typename ValueType>
void BSTree<KeyType, ValueType>::RemoveNode(KeyType key) 
{
	if (root == nullptr) {
		return;
	}
	else {
		RemoveNode(root, key);
	}
}

template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>* BSTree<KeyType, ValueType>::RemoveNode(BSTNode<KeyType, ValueType>* node, KeyType key)
{
	if (node->key < key) {
		node->rightChild = RemoveNode(node->rightChild, key);
	}
	else if (node->key > key) {
		node->leftChild = RemoveNode(node->leftChild, key);
	}
	else {
		// Remove node
		if (node->leftChild == nullptr) {
			BSTNode<KeyType, ValueType>* temp = node->rightChild;
			delete node;
			return temp;
		}
		if (node->rightChild == nullptr) {
			BSTNode<KeyType, ValueType>* temp = node->leftChild;
			delete node;
			return temp;
		}

		BSTNode<KeyType, ValueType>* successor = GetSuccessor(root);
		node->key = successor->key;
		node->right = RemoveNode(node->right, successor->key);
	}
	return node;
}

template <typename KeyType, typename ValueType>
void BSTree<KeyType, ValueType>::Search(KeyType key)
{
	if (root == nullptr) {
		return;
	}
	else {
		Search(root, key);
	}
}

template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>* BSTree<KeyType, ValueType>::Search(BSTNode<KeyType, ValueType>* node, KeyType key)
{
	if (node->key < key) {
		Search(node->rightChild, key);
	}
	else if (node->key > key) {
		Search(node->leftChild, key);
	}
	else {
		return node;
	}
}


template <typename KeyType, typename ValueType>
bool BSTree<KeyType, ValueType>::IsEmpty()
{
	return root == nullptr;
}

template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>* GetSuccessor(BSTNode<KeyType, ValueType>* node) {
	node = node->rightChild;
	while (node != NULL && node->leftChild != NULL) {
		node = node->leftChild;
	}
	return node;
}

// TODO: -- Traversal methods...
auto inorderTraversal = []() {};

auto preorderTraversal = []() {};

auto postorderTraversal = []() {};