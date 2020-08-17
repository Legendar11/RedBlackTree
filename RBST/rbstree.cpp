#include "RBST.h"

int main(int argc, char** argv)
{
	Tree::RandomizedBinarySearchTree<int> tree;
	
	tree.Scanner(argv[1], argv[2]);
    
    std::cout << "ok!";
    
	return 0;
}
