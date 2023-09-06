
#pragma once

#include <vector>
#include <memory>

//#define USE_OSS

#ifndef USE_OSS
#include <iostream>
#define OUT std::cout
#else
#include <sstream>

extern std::ostringstream oss;

#define OUT oss
#endif

namespace ensc251 {

	static unsigned count = 0; // Static variable to keep track of count

	class TreeNode
	{
	private:
		typedef std::shared_ptr<TreeNode> TreeNodeSP;

	typedef std::vector<TreeNodeSP> TreeNodeSPVect;

	protected:
		TreeNodeSPVect childSPVector; // Vector to store child TreeNode shared pointers

		typedef void (TreeNode::*action_func)(void); // Function pointer type for action functions

	typedef void (TreeNode::*traverse_func)(void); // Function pointer type for traverse functions

	public:
		TreeNode() {}

		virtual ~TreeNode()
		{
			// do not modify this insertion on OUT
			OUT << "Destroying TreeNode with " << childSPVector.size() << " children." << std::endl;
		}

		virtual std::shared_ptr<TreeNode> clone() const { return std::make_shared<TreeNode>(*this); } // Virtual function to clone the TreeNode

		TreeNode(const TreeNode& nodeToCopy)
		{
			// Copy constructor for TreeNode
			if (nodeToCopy.childSPVector.size()) {
				for (auto baby : nodeToCopy.childSPVector) {
					add_childP(baby->clone()); // Clone and add each child TreeNode to the current TreeNode
				}
			}
		}

		friend void swap(TreeNode& first, TreeNode& second)
		{
			// Swap function for TreeNode objects
			using std::swap;
			swap(first.childSPVector, second.childSPVector); // Swap the childSPVector between two TreeNode objects
		}

		TreeNode& operator=(TreeNode other)
		{
			// Assignment operator for TreeNode objects
		swap(*this, other); // Swap the current TreeNode with the other TreeNode
		return *this;
		}

		void add_childP(TreeNodeSP child) { childSPVector.push_back(child); } // Add a child TreeNode to the current TreeNode
		void add_children(const TreeNodeSPVect& childPV) { childSPVector.insert(childSPVector.end(), childPV.begin(), childPV.end()); } // Add multiple child TreeNodes to the current TreeNode
		const TreeNodeSPVect& get_children() const { return childSPVector; } // Get the vector of child TreeNodes

		void traverse_children_post_order(traverse_func tf, action_func af)
		{
			// Traverse the child TreeNodes in post-order and perform the given action function on each TreeNode
			for (auto childP : childSPVector) {
				(childP.get()->*tf)(); // Traverse the child TreeNodes in post-order
			}
			(this->*af)(); // Perform the action function on the current TreeNode
		}

		void traverse_children_in_order(traverse_func tf, action_func af)
		{
			// Traverse the child TreeNodes in in-order and perform the given action function on each TreeNode
			if (childSPVector.size() != 2) {
				OUT << "Error -- can only do inorder traversal on a node with 2 children" << std::endl; // Error message for in-order traversal on nodes with less than or more than 2 children
			}
		(childSPVector[0].get()->*tf)(); // Traverse the left child TreeNode in in-order
		(this->*af)(); // Perform the action function on the current TreeNode
		(childSPVector[1].get()->*tf)(); // Traverse the right child TreeNode in in-order
		}

		void traverse_children_pre_order(traverse_func tf, action_func af)
		{
			// Traverse the child TreeNodes in pre-order and perform the given action function on each TreeNode
			(this->*af)(); // Perform the action function on the current TreeNode
			for (auto childP : childSPVector) {
				(childP.get()->*tf)(); // Traverse the child TreeNodes in pre-order
			}
		}

		void count_action()
		{
			count++; // Increment the count variable
		}


		void count_traverse()
		{
			// Traverse the child TreeNodes in post-order and increment the count for each TreeNode
			traverse_children_post_order(&ensc251::TreeNode::count_traverse, &ensc251::TreeNode::count_action);
		}

		void reset_count() { count = 0; } // Reset the count variable to 0
		unsigned get_count() const { return count; } // Get the current value of the count variable
	};
}