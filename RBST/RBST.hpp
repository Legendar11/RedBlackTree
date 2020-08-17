#include <fstream>
#include <iostream>
#include <ctime>
#include <string>

#include "RBST.h"

namespace Tree
{
template<typename Element, typename Compar>
RandomizedBinarySearchTree<Element, Compar>::RandomizedBinarySearchTree() : _root(nullptr)
{	
}

template<typename Element, typename Compar>
typename RandomizedBinarySearchTree<Element, Compar>::RBSTNode* 
	RandomizedBinarySearchTree<Element, Compar>::insert(const Element& X, RBSTNode *T)
{
	if (T == nullptr) return new RBSTNode(X, rand() % 100 + 1);
	
	if (_compar(X, T->_key))
	{
		T->_left = insert(X, T->_left);
		if (T->_left->_priority < T->_priority)
		{
			RBSTNode *L = T->_left;
			T->_left = L->_right;
			L->_right = T;
			return L;
		}
	}
	else
	{
		T->_right = insert(X, T->_right);
		if (T->_right->_priority < T->_priority)
		{
			RBSTNode *R = T->_right;
			T->_right = R->_left;
			R->_left = T;
			return R;
		}
	}
	return T;		
}

template<typename Element, typename Compar>
size_t RandomizedBinarySearchTree<Element, Compar>::countNodes(RBSTNode *r)
{
	if (_root == nullptr) return 0;
	
	int l = 1;
	l += countNodes(r->_left);
	l += countNodes(r->_right);
	return l;
}	

template<typename Element, typename Compar>
typename RandomizedBinarySearchTree<Element, Compar>::RBSTNode* 
	RandomizedBinarySearchTree<Element, Compar>::search(RBSTNode *r, const Element val)
{
	while (r)
	{
		int rval = r->_key;
		
		if (val == rval)
		{
			return r;
		}
		
		if (_compar(val, rval)) r = r->_left;
		else  r = r->_right;
	}
	return nullptr;
}

template<typename Element, typename Compar>
void RandomizedBinarySearchTree<Element, Compar>::Print(RBSTNode *r, std::ostream& os)
{
	if (r)
	{
		Print(r->_left, os);
		os << r->_key << "  ";
		Print(r->_right, os);
	}
}

template<typename Element, typename Compar>
size_t RandomizedBinarySearchTree<Element, Compar>::height(RBSTNode *node)
{
	size_t l, r, h = 0;
	if (node) 
	{
		l = height(node->_left);
		r = height(node->_right);
		h = ((l > r) ? l : r) + 1;
	}
	return h;
}

template<typename Element, typename Compar>
typename RandomizedBinarySearchTree<Element, Compar>::RBSTNode* 
	RandomizedBinarySearchTree<Element, Compar>::remove(RBSTNode* node, const Element key)
{
	if (!node) return nullptr;

	if (node->_key == key) 
	{
		RBSTNode* tmp; 

		if (!node->_right)
		{
			tmp = node->_left;
		}
		else 
		{
			RBSTNode* ptr = node->_right; 
			
			if (!ptr->_left) 
			{
				ptr->_left = node->_left; 
				tmp = ptr; 
			}
			else 
			{
				RBSTNode* pmin = ptr->_left;
				while (pmin->_left) 
				{
					ptr = pmin;
					pmin = ptr->_left; 
				}
					
				ptr->_left = pmin->_right;
				pmin->_left = node->_left; 
				pmin->_right = node->_right;
				tmp = pmin; 
			}
		}

		delete node; 
		return tmp; 
	}
	else 
	{
		if (_compar(key, node->_key))
		{
			node->_left = remove(node->_left, key);
		}
		else 
		{
			node->_right = remove(node->_right, key);
		}
	}
}

template <typename Element, typename Compar>
void RandomizedBinarySearchTree<Element, Compar>::Scanner(const std::string filenameIn, const std::string filenameOut)
{
	std::ifstream fin(filenameIn);
	
	const size_t maxR = 100000;
		
	Element value;
	std::string command;
	size_t start;
	clear(_root);
	
	std::ofstream fout(filenameOut);
	
	while(fin.peek() != -1)
	{
		fin >> command;
		
		if (command == "add")
		{
			fin >> value; 
			
			start = clock();
			insert(value);
			fout << "add " << value << " in " 
				<< (clock() - start) << " ms " << std::endl;
		}
		
		if (command == "addI")
		{
			fin >> value; 
			
			start = clock();
			for(Element i = 0; i < value; i++) insert(i);
			fout << "add " << value << " elements in " 
				<< (clock() - start) << " ms " << std::endl;
		}
		
		if (command == "addR")
		{
			fin >> value; 
			
			start = clock();
			for(Element i = 0; i < value; i++) insert(rand() % maxR);
			fout << "add " << value << " elements in " 
				<< (clock() - start) << " ms " << std::endl;
		}
		
		if (command ==  "remove")
		{
			fin >> value; 
			
			start = clock();
			remove(value);
			fout << "remove " << value << " in " 
				<< (clock() - start) << " ms " << std::endl;
		}
		
		if (command ==  "removeI")
		{
			fin >> value; 
			
			start = clock();
			for(Element i = 0; i < value; i++) remove(i);
			fout << "remove " << value << " elements in " 
				<< (clock() - start) << " ms " << std::endl;
		}
		
		if (command ==  "removeR")
		{
			fin >> value; 
			
			start = clock();
			for(Element i = 0; i < value; i++) remove(rand() % maxR);
			fout << "remove " << value << " elements in " 
				<< (clock() - start) << " ms " << std::endl;
		}
		
		if (command == "find")
		{
			fin >> value; 
		
			start = clock();
			fout << std::endl << "finded " 
				<< value << " = " << (find(value) ? "true" : "false") << std::endl;
			fout << "finded " << value << " in " 
				<< (clock() - start) << " ms " << std::endl;
		}
		
		if (command == "findR")
		{
			fin >> value; 
		
			start = clock();
			for(Element i = 0; i < value; i++) find(rand() % maxR);
			fout << "find " << value << " elements in " 
				<< (clock() - start) << " ms " << std::endl;
		}
		
		if (command == "print")
		{
			Print(fout);
		}
		
		if (command == "max")
		{
			fout << std::endl << "maximum = " << maximum()->_key << std::endl;
		}
		
		if (command == "min")
		{
			fout << std::endl << "minimum = " << minimum()->_key << std::endl;
		}
		
	}
	
	fin.close();
	fout.close();
}
}
