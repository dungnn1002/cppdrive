#include "file_tree.h"

#include <dirent.h>

#include <cstring>
#include <iostream>
#include <sstream>

FileTree::FileTree() : name("") {}

// Constructor to initialize with a name
FileTree::FileTree(const std::string &n) : name(n) {}

// Function to add a child node
void FileTree::addChild(const std::string &childName) { children.emplace_back(childName); }

// Function to convert the tree structure to a string
std::string FileTree::toString() const {
    std::ostringstream oss;
    oss << name;
    for (const auto &child : children) {
        oss << " {" << child.toString() << "}";
    }
    return oss.str();
}

// Function to parse the tree structure from a string
FileTree FileTree::parseTree(const std::string &str) {
    size_t pos = 0;
    return parseTreeRecursive(str, pos);
}

// Helper function to recursively parse nodes from a string
FileTree FileTree::parseTreeRecursive(const std::string &str, size_t &pos) {
    std::string nodeName;
    // Read the name
    while (pos < str.size() && str[pos] != ' ' && str[pos] != '{' && str[pos] != '}') {
        nodeName += str[pos++];
    }
    FileTree node(nodeName);
    while (pos < str.size() && str[pos] != '}') {
        if (str[pos] == ' ') {
            pos++;
        } else if (str[pos] == '{') {
            pos++;   // Skip '{'
            node.children.push_back(parseTreeRecursive(str, pos));
        }
    }
    pos++;   // Skip '}'
    return node;
}

// Function to display the tree structure
void FileTree::display(int depth) const {
    // Display the current node
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";   // Adjust indentation based on depth
    }
    // Display children recursively
    for (const auto &child : children) {
        if (child.children.size() > 0) {
            std::cout << "\033[1;34m" << child.name << "\033[0m"
                      << "/" << std::endl;
        } else {
            std::cout << child.name << std::endl;
        }
    }
}

// Function to populate the tree structure with files and folders in a directory
void FileTree::populateFromDirectory(const std::string &path) {
    DIR *dir = opendir(path.c_str());
    if (dir == nullptr) {
        std::cerr << "Error opening directory: " << path << std::endl;
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Ignore entries for current directory (.) and parent directory (..)
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // Check if the entry is a directory
            if (entry->d_type == DT_DIR) {
                // Add a child folder node
                addChild(entry->d_name);
                // Recursively populate the child node
                children.back().populateFromDirectory(path + "/" + entry->d_name);
            } else {
                // Add a child file node
                addChild(entry->d_name);
            }
        }
    }

    closedir(dir);
}
