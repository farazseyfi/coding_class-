
#pragma once

#include "TreeNode.hpp"

#include <iomanip>

#include <string>

//#include <string_view>


class DirectoryEntry : public ensc251::TreeNode
{
private:
    std::string m_name;

public:
    DirectoryEntry(std::string_view name) : m_name{name} {}


  friend void swap(DirectoryEntry& a, DirectoryEntry& b) noexcept
    {
        using std::swap;

       swap(static_cast<ensc251::TreeNode&>(a), static_cast<ensc251::TreeNode&>(b));
        swap(a.m_name, b.m_name);
    }

       void set_name(const std::string_view& name)
    {
        m_name = name;
    }

  const std::string& get_name() { return m_name; }

  virtual void print_action() = 0;
};
// DirectoryEntry class represents an entry in a directory tree.

class File : public DirectoryEntry
{

  public:
    File(std::string_view name) : DirectoryEntry(name) {}

   std::shared_ptr<TreeNode> clone() const override { return std::make_shared<File>(*this); };
    // Overrides the clone function of the base class.
    // Creates a new shared pointer to a File object with the same properties as the original object.

    void print_action() override
{
        // Do not modify insertion on OUT in this member function!
        OUT << std::setw(20) << this->get_name() + "\tF" << std::endl;
    }
    // Overrides the print_action function of the base class.
    // Defines the action performed when printing a File object.

};
// File class represents a file in a directory tree.

class Directory : public DirectoryEntry
{

  public:
    Directory(std::string_view dir_name) : DirectoryEntry(dir_name) {}

    std::shared_ptr<TreeNode> clone() const override { return std::make_shared<Directory>(*this); };
    // Overrides the clone function of the base class.
    // Creates a new shared pointer to a Directory object with the same properties as the original object.

    void print_action() override
    {
        // Do not modify insertion on OUT in this member function!
        OUT << std::setw(20) << this->get_name() + "\t|" << " ";
        OUT << this->childSPVector.size() << std::endl;
    }
    // Overrides the print_action function of the base class.
    // Defines the action performed when printing a Directory object.

      void print_traverse()
    {
        if (childSPVector.size() > 2)
        {
            traverse_children_pre_order(
                static_cast<traverse_func>(&Directory::print_traverse),
                static_cast<action_func>(&DirectoryEntry::print_action));
        }
        else if (childSPVector.size() < 2)
        {
            traverse_children_post_order(
                static_cast<traverse_func>(&Directory::print_traverse),
                static_cast<action_func>(&DirectoryEntry::print_action));
        }
        else
        {
            traverse_children_in_order(
                static_cast<traverse_func>(&Directory::print_traverse),
                static_cast<action_func>(&DirectoryEntry::print_action));
        }
    }
    // Defines the traversal and printing logic for a Directory object.
};
// Directory class represents a directory in a directory tree.