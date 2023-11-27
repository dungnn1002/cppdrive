#include <dirent.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>

class Tree {
   public:
    std::string name;
    std::vector<Tree> children;

    // Constructor to initialize with a name
    Tree(const std::string &n) : name(n) {}

    // Function to add a child node
    void addChild(const std::string &childName) { children.emplace_back(childName); }

    // Function to convert the tree structure to a string
    std::string toString() const {
        std::ostringstream oss;
        oss << name;
        for (const auto &child : children) {
            oss << " {" << child.toString() << "}";
        }
        return oss.str();
    }

    static Tree parseTreeRecursive(const std::string &str, size_t &pos) {
        std::string name;
        // Read the name
        while (pos < str.size() && str[pos] != ' ' && str[pos] != '{' && str[pos] != '}') {
            name += str[pos++];
        }
        Tree node(name);
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

    static Tree parseTree(const std::string &str) {
        size_t pos = 0;
        return parseTreeRecursive(str, pos);
    }

    // Function to display the tree structure
    void display(int depth = 0) const {
        // Display the current node
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";   // Adjust indentation based on depth
        }
        std::cout << "├── " << name << std::endl;

        // Display children recursively
        for (const auto &child : children) {
            child.display(depth + 1);
        }
    }

    // Function to populate the tree structure with files and folders in a
    // directory
    void populateFromDirectory(const std::string &path) {
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
};

int main() {
    std::string directoryPath = "/tmp/cppdrive/";

    Tree root(".");

    root.populateFromDirectory(directoryPath);
    root.display();
    std::string treeString = root.toString();
    Tree newRoot = Tree::parseTree(treeString);
    std::cout << "Tree structure after parsing:\n";
    newRoot.display();

    return 0;
}