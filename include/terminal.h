#ifndef TERMINAL_H
#define TERMINAL_H

#include "file_tree.h"

class Terminal {
   public:
    Terminal(const std::string &treeData);

    Terminal(FileTree root);

    void resetCurrentDirectory();

    void ls() const;

    int cd(const std::string &directory);

   private:
    FileTree root;
    FileTree currentDirectory;
};

#endif   // TERMINAL_H
