
module;

// global module fragment area. Put #include directives here 
#include <functional>

export module Markets.DataStructures;

import std;

class NodeAddress {
	int row;
	int column;
};

template <typename KeyType>
class IAddressableNode
{
public:
	// Maps to Tree Level
	virtual int GetRow() = 0;
	// Maps to 2^(row-1)
	virtual int GetColumn() = 0;
	// Maps to Node.Key
	virtual KeyType GetElement() = 0;

	int GetRowLength() {
		return std::pow(2, GetRow());
	}
};

template <typename KeyType, typename ValueType>
class BSTNode : public IAddressableNode<KeyType>
{
public:
	BSTNode();
	BSTNode(KeyType key, ValueType value);
	~BSTNode();

	KeyType key;
	ValueType value;
	BSTNode* parent;
	BSTNode* leftChild;
	BSTNode* rightChild;

	int GetRow() override;
	int GetColumn() override;
	KeyType GetElement() override;
};

template <typename KeyType, typename ValueType>
auto defaultTraversalCallbackFn = [](BSTNode<KeyType, ValueType>* node) {
	std::println("BSTNode.key: {}", node.key);
};


template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>::BSTNode() : BSTNode(nullptr, nullptr) {}

template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>::BSTNode(KeyType key, ValueType value)
{
	this.key = key;
	this.value = value;
	this->leftChild = nullptr;
	this->rightChild = nullptr;
}

template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>::~BSTNode()
{
	delete key;
	delete value;
	delete this->leftChild;
	delete this->rightChild;
}

template <typename KeyType, typename ValueType>
int BSTNode<KeyType, ValueType>::GetRow()
{
	int row = 1;
	BSTNode<KeyType, ValueType>* tmpParent = parent;
	while (tmpParent != nullptr) {
		tmpParent = tmpParent->parent;
		row++;
	}
	return row;
}

template <typename KeyType, typename ValueType>
int BSTNode<KeyType, ValueType>::GetColumn()
{
	auto isLeft = this == parent->leftChild;
	auto isRight = this == parent->rightChild;
	auto unadjustedIndex = GetRow() * parent->GetColumn();
	int column = unadjustedIndex;
	if (isLeft) {
		column -= 1;
	}
	else if (isRight) {
		column += 1;
	}
	else {
		// TODO: Throw exception: non-binary tree unsupported currently for addressing
	}
	return column;
}

template <typename KeyType, typename ValueType>
KeyType BSTNode<KeyType, ValueType>::GetElement()
{
	return key;
}

enum TraversalKind {
	PreOrder, PostOrder, InOrder, LevelOrder
};

enum RotationDirection {
	Left, Right
};

template <typename KeyType, typename ValueType>
class BSTree
{
private:
	BSTNode<KeyType, ValueType>* root;

	BSTNode<KeyType, ValueType>* InsertNode(BSTNode<KeyType, ValueType>* parent, BSTNode<KeyType, ValueType>* node, KeyType key, ValueType value);
	BSTNode<KeyType, ValueType>* RemoveNode(BSTNode<KeyType, ValueType>* node, KeyType key, ValueType value);
	BSTNode<KeyType, ValueType>* Search(BSTNode<KeyType, ValueType>* node, KeyType key);
	void Traverse(BSTNode<KeyType, ValueType>* node, TraversalKind traversalMethod, std::function<void(BSTNode<KeyType, ValueType>*)> traversalCallbackFn);


	BSTNode<KeyType, ValueType>* RotateLeft(BSTNode<KeyType, ValueType>* node);
	BSTNode<KeyType, ValueType>* RotateRight(BSTNode<KeyType, ValueType>* node);


	BSTNode<KeyType, ValueType>* GetSuccessor(BSTNode<KeyType, ValueType>* node);

public:
	BSTree();
	~BSTree();
	void InsertNode(KeyType key, ValueType value);
	void RemoveNode(KeyType key);

	BSTNode<KeyType, ValueType>* Search(KeyType obj);

	void Traverse(TraversalKind traversalMethod, std::function<void(BSTNode<KeyType, ValueType>*)> traversalCallbackFn);
	BSTNode<KeyType, ValueType>* Rotate(BSTNode<KeyType, ValueType>* node, RotationDirection direction);

	void Balance();

	int Height();

	bool IsEmpty();
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
void BSTree<KeyType, ValueType>::InsertNode(KeyType key, ValueType value)
{
	BSTNode<KeyType, ValueType>* newNode = new BSTNode<KeyType, ValueType>();
	newNode->key = key;
	newNode->value = value;
	newNode->leftChild = nullptr;
	newNode->rightChild = nullptr;

	if (root == nullptr) {
		root = newNode;
		return;
	}
	else {
		InsertNode(newNode, key, value);
	}
}

template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>* BSTree<KeyType, ValueType>::InsertNode(BSTNode<KeyType, ValueType>* parent, BSTNode<KeyType, ValueType>* node, KeyType key, ValueType value)
{
	if (node == nullptr) {
		auto newNode = new BSTNode(key, value);
		newNode->parent = parent;
		return newNode;
	}
	if (node->key > key) {
		// Right insertion
		node->rightChild = InsertNode(node, node->rightChild, key, value);
	}
	else {
		// left insertion
		node->leftChild = InsertNode(node, node->leftChild, key, value);
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
BSTNode<KeyType, ValueType>* BSTree<KeyType, ValueType>::RemoveNode(BSTNode<KeyType, ValueType>* node, KeyType key, ValueType value)
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
BSTNode<KeyType, ValueType>* BSTree<KeyType, ValueType>::Search(KeyType key)
{
	if (root == nullptr) {
		return;
	}
	else {
		return Search(root, key);
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
void BSTree<KeyType, ValueType>::Traverse(TraversalKind traversalMethod, std::function<void(BSTNode<KeyType, ValueType>*)> traversalCallbackFn) {
	if (traversalCallbackFn == NULL) {
		traversalCallbackFn = defaultTraversalCallbackFn;
	}
	Traverse(root, traversalMethod, traversalCallbackFn);
}

template <typename KeyType, typename ValueType>
void BSTree<KeyType, ValueType>::Traverse(BSTNode<KeyType, ValueType>* node, TraversalKind traversalMethod, std::function<void(BSTNode<KeyType, ValueType>*)> traversalCallbackFn) {
	if (traversalMethod == PreOrder) {
		traversalCallbackFn(node);
		Traverse(node->leftChild, traversalMethod, traversalCallbackFn);
		Traverse(node->rightChild, traversalMethod, traversalCallbackFn);
	}
	else if (traversalMethod == PostOrder) {
		Traverse(node->leftChild, traversalMethod, traversalCallbackFn);
		Traverse(node->rightChild, traversalMethod, traversalCallbackFn);
		traversalCallbackFn(node);
	}
	else if (traversalMethod == InOrder) {
		Traverse(node->leftChild, traversalMethod, traversalCallbackFn);
		traversalCallbackFn(node);
		Traverse(node->rightChild, traversalMethod, traversalCallbackFn);
	}
	else if (traversalMethod == LevelOrder) {
		auto nodes = new std::queue<BSTNode<KeyType, ValueType>*>();
		auto queue = new std::queue<BSTNode<KeyType, ValueType>*>();
		queue.enqueue(root);
		while (!queue.isEmpty()) {
			auto x = queue.dequeue();
			if (x == nullptr)
				continue;

			nodes.enqueue(x);
			queue.enqueue(x.leftChild);
			queue.enqueue(x.rightChild);
		}
		for (auto n : nodes) {
			traversalCallbackFn(n);
		}
	}
}

template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>* BSTree<KeyType, ValueType>::Rotate(BSTNode<KeyType, ValueType>* node, RotationDirection direction) {
	if (direction == Left) {
		return RotateLeft(node);
	}
	else if (direction == Right) {
		return RotateRight(node);
	}
	else {
		// TODO: Error, throw exception?
	}
}


template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>* BSTree<KeyType, ValueType>::RotateLeft(BSTNode<KeyType, ValueType>* node) {
	auto y = node->rightChild;
	auto T2 = y->leftChild;

	// Perform rotation
	y->left = node;
	node->right = T2;

	return y;
}
template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>* BSTree<KeyType, ValueType>::RotateRight(BSTNode<KeyType, ValueType>* node) {
	auto x = node->leftChild;
	auto T2 = x->rightChild;

	// Perform rotation
	x->rightChild = node;
	node->leftChild = T2;

	return x;
}


template <typename KeyType, typename ValueType>
bool BSTree<KeyType, ValueType>::IsEmpty()
{
	return root == nullptr;
}

template <typename KeyType, typename ValueType>
BSTNode<KeyType, ValueType>* BSTree<KeyType, ValueType>::GetSuccessor(BSTNode<KeyType, ValueType>* node) {
	node = node->rightChild;
	while (node != nullptr && node->leftChild != nullptr) {
		node = node->leftChild;
	}
	return node;
}

// TODO: -- Traversal methods...
auto inorderTraversal = []() {};

auto preorderTraversal = []() {};

auto postorderTraversal = []() {};

