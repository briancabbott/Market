export module Markets.DataStructures;

export class IAddressableNode;
export class BSTNode;
export class BSTree;

export class BSTForwardIterator;
export class BSTReverseIterator;

export enum RotationDirection;
export enum TraversalKind;
export void defaultTraversalCallbackFn();
//template <typename KeyType, typename ValueType>
//auto defaultTraversalCallbackFn = [](BSTNode<KeyType, ValueType>* node) {
//	std::println("BSTNode.key: {}", node.key);
//};

//export void inorderTraversal();