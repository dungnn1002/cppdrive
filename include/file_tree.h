#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>

class FileTree {
   public:
    std::string name;
    std::vector<FileTree> children;

    FileTree();

    // Constructor to initialize with a name
    FileTree(const std::string &n);

    // Function to add a child node
    void addChild(const std::string &childName);

    // Function to convert the tree structure to a string
    std::string toString() const;

    // Function to parse the tree structure from a string
    FileTree parseTree(const std::string &str);

    FileTree parseTreeRecursive(const std::string &str, size_t &pos);

    // Function to display the tree structure
    void display(int depth = 0) const;

    // Function to populate the tree structure with files and folders in a directory
    void populateFromDirectory(const std::string &path);
};

#endif   // TREE_H
