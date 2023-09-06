

#pragma once

#include <vector>
#include <memory>
#include "shared_from.hpp"

#define USE_OSS

#ifndef USE_OSS
#include <iostream>
#define OUT std::cout
#else
#include <sstream>
extern std::ostringstream oss;
#define OUT oss
#endif

namespace ensc251 {
	// using namespace std;

	static unsigned count = 0;

	// Deriving from enable_shared_from_this<> in order to be able
	//    to get a copy of a shared pointer to the TreeNode object
	class TreeNode : public std::enable_shared_from_this<TreeNode>
	{
	private:
		typedef std::shared_ptr<TreeNode> TreeNodeSP;
		typedef std::vector<TreeNodeSP> TreeNodeSPVect;

	protected:
		TreeNodeSPVect childSPVector;
		typedef void (TreeNode::*action_func)(void);
		typedef void (TreeNode::*traverse_func)(void);

	public:
		virtual ~TreeNode() {
			// do not modify this insertion on OUT
//			OUT << "Destroying TreeNode with " << childSPVector.size() << " children."<< std::endl;
		}
		TreeNode() {};

		virtual std::shared_ptr<TreeNode> clone() const { return std::make_shared<TreeNode>(*this); };

		TreeNode(const TreeNode& nodeToCopy) {
			for (auto childSP: nodeToCopy.childSPVector) {
				add_childP(childSP->clone());
			}
		}

		friend void swap(TreeNode& first, TreeNode& second) // the swap function (should never fail!)
		{
			  // enable ADL (not necessary in our case, but good practice)
			using std::swap;

			swap(first.childSPVector, second.childSPVector);
		}

		TreeNode & operator = (TreeNode other) // note: argument passed by value, copy made!
		{
			swap(*this, other); // swap *this with other
			return *this; // by convention, always return *this
		}// swapped other (original *this) is destroyed, releasing the memory

		void add_childP(TreeNodeSP child){ childSPVector.push_back(child); }
		void add_children(const TreeNodeSPVect& childPV){ childSPVector.insert(childSPVector.end(), childPV.begin(), childPV.end()); }
		const TreeNodeSPVect& get_children() const { return childSPVector; }

		void traverse_children_post_order(traverse_func tf, action_func af)
		{
			for(auto childP : childSPVector) {
				(childP.get()->*tf)(); // traverse child's children using tf
			}
			(this->*af)();
		}

		void traverse_children_in_order(traverse_func tf, action_func af)
		{
			if(childSPVector.size() != 2) {
				OUT << "Error -- can only do inorder traversal on a node with 2 children" << std::endl;
			}
			else {
				(childSPVector[0].get()->*tf)();
				(this->*af)();
				(childSPVector[1].get()->*tf)();
			}
		}

		void traverse_children_pre_order(traverse_func tf, action_func af)
		{
			(this->*af)();
			for(auto childP : childSPVector) {
				(childP.get()->*tf)();
			}
		}

		void count_action()
		{
			count++;
		}

		void count_traverse()
		{
			traverse_children_post_order(&ensc251::TreeNode::count_traverse, &ensc251::TreeNode::count_action);
		}
		void reset_count() { count = 0; }
		unsigned get_count() const { return count; }

	};
}
