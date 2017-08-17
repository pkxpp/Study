#include "stdafx.h"
#include <iostream>
#include<cmath>
#include "BinaryTree.h"
using namespace std;

// insert accepts a TreeNode pointer and a pointer to a node. The function inserts the node into 
// the tree pointed to by the TreeNode pointer. This function is called recursively. 
void BinaryTree::insert(TreeNode *nodePtr, TreeNode *newNode)
{
	if (nodePtr == NULL)
		nodePtr = newNode;                  // Insert the node.
	else if (newNode->value < nodePtr->value)
		insert(nodePtr->left, newNode);     // Search the left branch
	else
		insert(nodePtr->right, newNode);    // Search the right branch
}


// insertNode creates a new node to hold num as its value, and passes it to the insert function. 
void BinaryTree::insertNode(int num)
{
	TreeNode *newNode;      // Pointer to a new node.

	// Create a new node and store num in it.
	newNode = new TreeNode;
	newNode->value = num;
	newNode->left = newNode->right = NULL;

	// Insert the node.
	insert(root, newNode);
}


// destroySubTree is called by the destructor. It deletes all nodes in the tree.   
void BinaryTree::destroySubTree(TreeNode *nodePtr)
{
	if (nodePtr)
	{
		if (nodePtr->left)
			destroySubTree(nodePtr->left);
		if (nodePtr->right)
			destroySubTree(nodePtr->right);
		delete nodePtr;
	}
}


// searchNode determines if a value is present in the tree. 
// If so, the function returns true. Otherwise, it returns false.  
bool BinaryTree::searchNode(int num)
{
	TreeNode *nodePtr = root;

	while (nodePtr)
	{
		if (nodePtr->value == num)
			return true;
		else if (num < nodePtr->value)
			nodePtr = nodePtr->left;
		else
			nodePtr = nodePtr->right;
	}
	return false;
}


// remove calls deleteNode to delete the node whose value member is the same as num.
void BinaryTree::remove(int num)
{
	deleteNode(num, root);
}


// deleteNode deletes the node whose value member is the same as num.
void BinaryTree::deleteNode(int num, TreeNode *nodePtr)
{
	if (num < nodePtr->value)
		deleteNode(num, nodePtr->left);
	else if (num > nodePtr->value)
		deleteNode(num, nodePtr->right);
	else
		makeDeletion(nodePtr);
}


// makeDeletion takes a reference to a pointer to the node that is to be deleted. 
// The node is removed and the branches of the tree below the node are reattached. 
void BinaryTree::makeDeletion(TreeNode *nodePtr)
{
	// Define a temporary pointer to use in reattaching
	// the left subtree.
	TreeNode *tempNodePtr;

	if (nodePtr == NULL)
		cout << "Cannot delete empty node.\n";
	else if (nodePtr->right == NULL)
	{
		tempNodePtr = nodePtr;
		nodePtr = nodePtr->left;   // Reattach the left child
		delete tempNodePtr;
	}
	else if (nodePtr->left == NULL)
	{
		tempNodePtr = nodePtr;
		nodePtr = nodePtr->right;  // Reattach the right child
		delete tempNodePtr;
	}
	// If the node has two children.
	else
	{
		// Move one node the right.
		tempNodePtr = nodePtr->right;
		// Go to the end left node.
		while (tempNodePtr->left)
			tempNodePtr = tempNodePtr->left;
		// Reattach the left subtree.
		tempNodePtr->left = nodePtr->left;
		tempNodePtr = nodePtr;
		// Reattach the right subtree.
		nodePtr = nodePtr->right;
		delete tempNodePtr;
	}
}


// The displayInOrder member function displays the values 
// in the subtree pointed to by nodePtr, via inorder traversal. 
void BinaryTree::displayInOrder(TreeNode *nodePtr) const
{
	if (nodePtr)
	{
		displayInOrder(nodePtr->left);
		cout << nodePtr->value << endl;
		displayInOrder(nodePtr->right);
	}
}


// The displayPreOrder member function displays the values  
// in the subtree pointed to by nodePtr, via preorder traversal. 
void BinaryTree::displayPreOrder(TreeNode *nodePtr) const
{
	if (nodePtr)
	{
		cout << nodePtr->value << endl;
		displayPreOrder(nodePtr->left);
		displayPreOrder(nodePtr->right);
	}
}


// The displayPostOrder member function displays the values 
// in the subtree pointed to by nodePtr, via postorder traversal.
void BinaryTree::displayPostOrder(TreeNode *nodePtr) const
{
	if (nodePtr)
	{
		displayPostOrder(nodePtr->left);
		displayPostOrder(nodePtr->right);
		cout << nodePtr->value << endl;
	}
}

// InOrder to find
int BinaryTree::getLevel(TreeNode *nodePtr, int val, int level)
{
	if (nodePtr)
	{
		level = getLevel(nodePtr->left, val, level + 1);
		if (nodePtr->value == val){
			return level;
		}
		level = getLevel(nodePtr->right, val, level + 1);

	}
	
	return 0;
}

int BinaryTree::getLevelTree(TreeNode *node, int val, int lev)
{
	if (node == NULL)
	{
		return 0;
	}
	if (node->value == val)
	{
		return lev;
	}
	int downlev = getLevelTree(node->left, val, lev + 1);
	return downlev;
}
//int BinaryTree::getLevel(int val)
//{
//	return getLevelTree(root, val, 1);
//}

int BinaryTree::countNodes(TreeNode *root)
{

	if (root == NULL)
	{
		return 0;
	}
	else
	{
		int count = 1;
		count += countNodes(root->left);
		count += countNodes(root->right);
		return count;
	}

}
void BinaryTree::count()
{
	cout<< countNodes(root);
}