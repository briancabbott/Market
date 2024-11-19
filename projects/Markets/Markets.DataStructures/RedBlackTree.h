#pragma once

import std;

/// <summary>
/// Red-Black Tree:
///    - Extra Color information for rebalancing on insert.
/// 
///    - Requirements:
///       - Every node is either red or black.
///       
///       - All NIL nodes(figure 1) are considered black.
/// 
///       - A red node does not have a red child.
/// 
///       - Every path from a given node to any of its descendant 
///			NIL nodes goes through the same number of black nodes.
/// 
///       - (Conclusion)If a node N has exactly one child, the child 
///			must be red, because if it were black, its NIL descendants 
///			would sit at a different black depth than N's NIL child, 
///			violating requirement 4.
/// 
///    - Operations:
///       -  
/// 
///    - Balancing Rule:
///    - Non-branching Node:
///    - Path:
///    - Simple Path:
///    - Descendant:
///    - Adjacent: 
/// 
///    - Red-Black Balancing Rules:
/// 
/// </summary>
enum class RBColor {
	Red,
	Black
};

template <typename DataType>
class RBNode
{
public:
	RBNode& parent;
	RBNode& left;
	RBNode& right;
	RBColor color;
	DataType& data;
};

template <typename Key, typename DataType>
class RBTree {

private:
	RBNode<DataType>& root;

	RBNode<DataType>& put(RBNode<DataType>& node, Key key, DataType value);
	DataType& get(RBNode<DataType>& node, Key key);
	RBNode<DataType>& deleteMin(RBNode<DataType>& node);
	RBNode<DataType>& deleteMax(RBNode<DataType>& node);
	RBNode<DataType>& deleteNode(RBNode<DataType>& node, Key key);
	int height(RBNode<DataType>& node);
	
	RBNode<DataType>& minimum(RBNode<DataType>& node);
	RBNode<DataType>& maximum(RBNode<DataType>& node);
	RBNode<DataType>& ceiling(RBNode<DataType>& node, Key key);
	RBNode<DataType>& floor(RBNode<DataType>& node, Key key);
	Key select(RBNode<DataType>& node, int rank);
	int rank(Key key, RBNode<DataType>& node);


	RBNode<DataType>& rotateRight(RBNode<DataType>& node);
	RBNode<DataType>& rotateLeft(RBNode<DataType>& node);
	void flipColors();
	RBNode<DataType>& moveRedLeft(RBNode<DataType>& node);
	RBNode<DataType>& moveRedRight(RBNode<DataType>& node);
	RBNode<DataType>& balance(RBNode<DataType>& node);

	/***************************************************************************
	*  Check integrity of red-black tree data structure.
	***************************************************************************/
	bool checkIntegrity();
	bool isEmpty();

	bool isBST();
	bool isBST(RBNode<DataType>& node, Key min, Key max);
	bool isSizeConsistent();
	bool isSizeConsistent(RBNode<DataType>& node);
	bool isRankConsistent();
	bool isBalanced();
	bool isBalanced(RBNode<DataType>& node, int black);

	bool is23();
	bool is23(RBNode<DataType>& node);

	std::vector<Key> keys(RBNode<DataType>& node, std::queue<Key> queue, Key lo, Key hi);

public:
	RBTree();
	~RBTree();
	
	/***************************************************************************
	*  Node helper methods.
	***************************************************************************/
	bool isRed(RBNode<DataType>& node);

	/***************************************************************************
	*  Red-black tree insertion.
	***************************************************************************/
	void put(Key key, DataType value);
	DataType get(Key key);
	bool contains(Key key);
	void search(DataType data);
	void traverse();

	/***************************************************************************
	*  Red-black tree deletion.
	***************************************************************************/
	void deleteMin();
	void deleteMax();
	void deleteNode(Key key);

	/***************************************************************************
	*  Utility functions.
	***************************************************************************/
	int height();

	/***************************************************************************
	*  Ordered symbol table methods.
	***************************************************************************/
	RBNode<DataType>& minimum();
	RBNode<DataType>& maximum();
	Key ceiling(Key key);
	Key floor(Key key);
	Key select(int rank);
	int rank(Key key);

	int size();
	int size(Key lo, Key hi);

	/***************************************************************************
	*  Range count and range search.
	***************************************************************************/
	std::vector<Key> keys();
	std::vector<Key> keys(Key lo, Key hi);
};

