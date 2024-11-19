#pragma once


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
enum class Color {
	Red,
	Black
};

template <typename DataType>
class RedBlackTree {

private:
	RedBlackTreeNode<DataType> root;

public:
	RedBlackTree();
	void insertNode(DataType data);
	void deleteNode(DataType data);
	void traverse();
	void search(DataType data);
	void rotate();
	void colorFlip();
	
	put
	get
	contains
	remove
	
	minimum
	maximum
	ceiling
	floor

	rank
	select
	size
	keys
};

template <typename DataType>
class RedBlackTreeNode
{
public:
	RedBlackTreeNode& parent;
	RedBlackTreeNode& left;
	RedBlackTreeNode& right;
	DataType data;
	Color color;
};