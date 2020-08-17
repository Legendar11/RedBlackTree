#include <cstdlib>
#include <iostream>

#include "RBTree.h"

int main(int argc, char** argv)
{
    Tree::RBTree<int> tree;
	
   	tree.Scanner(argv[1], argv[2]);
    
    std::cout << "ok!";
    
    return 0;
}


