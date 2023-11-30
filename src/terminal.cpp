#include "terminal.h"

#include <iostream>

Terminal::Terminal(const std::string &treeData) {
    root = root.parseTree(treeData);
    currentDirectory = root;
}

Terminal::Terminal(FileTree root) : root(root), currentDirectory(root) {}

void Terminal::ls() const { currentDirectory.display(); }

int Terminal::cd(const std::string &directory) {
    if (directory == "..") {
        if (currentDirectory.name == ".") {
            std::cout << "Already at root directory" << std::endl;
            return 0;
        }
        currentDirectory = root;
        return 1;
    }

    bool found = false;

    // Find the child directory with the given name
    for (const auto &child : currentDirectory.children) {
        if (child.name == directory && !child.children.empty()) {
            currentDirectory = child;   // Change to the child directory
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Directory not found: " << directory << std::endl;
    }

    return found;
}

void Terminal::resetCurrentDirectory() { currentDirectory = root; }