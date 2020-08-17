/// "����������" (��������) �������, ��������� � ����� RBTree.h
///
////////////////////////////////////////////////////////////////////////////////

#include <stdexcept>        // std::invalid_argument
#include <fstream>
#include <ctime>

#include "RBTree.h"

namespace Tree 
{
//==============================================================================
// class RBTree
//==============================================================================

template <typename Element, typename Compar >
void RBTree<Element, Compar>::deleteNodeRB(NodeRB* nd)
{ // ���� ���������� ���� �� ����������, ������ ������ �� ������, �.�. � ���������� �������� ���
    if (nd == nullptr) return;
    delete nd; // �������� ����� � �����������
}


template <typename Element, typename Compar >
void RBTree<Element, Compar>::insert(const Element& key)
{
    NodeRB* newNodeRB = insertNewBstEl(key); // ������� ��� � �������� ������ ������
    //std::cout << key; getchar();
    //Print(); getchar();
    rebalance(newNodeRB); // ����������������
}

template<typename Element, typename Compar> // ������ ������ ��������
void RBTree<Element, Compar>::delete_case1(NodeRB * n, NodeRB *p) // ��������, �������� �� ������
{
	if (p) delete_case2(n, p); // ���� ��������� ������� - �� ������, �� ���� ������
}

template<typename Element, typename Compar> // ������ ������ ��������
void RBTree<Element, Compar>::delete_case2(NodeRB * n, NodeRB *p) // ���� - �������, ��� ��������� - ������
{
	NodeRB* s = p->_left == n ? p->_right : p->_left; // �������� ����� ���������� ��������

	if (s->_color == RED) // ���� ���� - �������
	{
		p->_color = RED; // ������ �������� � �������
		s->_color = BLACK; // ������ ����� � ������

		if (p->_left == n) rotLeft(p); // �����
		else rotRight(p); // � ������ ������ ����������������
	}

	delete_case3(n, p); // ����������, �.�. �������� ��-������ � ���������� ������� � ������ ����� ��� ��� ��������
}

template<typename Element, typename Compar> // ������ ������ ��������
void RBTree<Element, Compar>::delete_case3(NodeRB * n, NodeRB *p) // ��� ������
{
	NodeRB* s = p->_left == n ? p->_right : p->_left; // �������� ����� ���������� ��������
	bool flag = true;

	if (p->_color == BLACK && // ���� �������� - ������
		s->_color == BLACK) // � ���� - ������
	{
		if (s->_left && s->_left->_color == RED) flag = false; // � ���� �����
		if (s->_right && s->_right->_color == RED) flag = false; // ������

		if (flag)
		{
			s->_color = RED; // �������� ����� � ������� (������� ������ ����� ���� ������)
			delete_case1(p, p->_parent); // �������� �������� ��������
		}
		else
		{
			delete_case4(n, p);
		}
	}
	else
	{
		delete_case4(n, p);
	}
}

template<typename Element, typename Compar> // ��������� ������ ��������
void RBTree<Element, Compar>::delete_case4(NodeRB * n, NodeRB *p) // �������� - �������, ��� ��������� - ������
{
	NodeRB* s = p->_left == n ? p->_right : p->_left; // �������� ����� ���������� ��������
	bool flag = true;

	if (p->_color == RED && // ���� �������� - �������
		s->_color == BLACK) // � ���� - ������
	{
		if (s->_left && s->_left->_color == RED) flag = false; // � ���� �����
		if (s->_right && s->_right->_color == RED) flag = false; // ������

		if (flag)
		{
			s->_color = RED; // ���� ����� ������� (������� ������ ����� ���� ������)
			p->_color = BLACK; // � �������� - ������
		}
		else
		{
			delete_case5(n, p);
		}
	}
	else
	{
		delete_case5(n, p);
	}
}

template<typename Element, typename Compar> // ����� ������ ��������
void RBTree<Element, Compar>::delete_case5(NodeRB * n, NodeRB *p) // ����� ���� ����� �������, ��� ��������� - ������
{
	NodeRB* s = p->_left == n ? p->_right : p->_left; // �������� ����� ���������� ��������
	bool flag = true;

	//std::cout << "on case 5"; getchar();

	if (s->_color == BLACK) // ���� ���� - ������
	{
		if (p->_left == n) // ���� ��� ����� ������
		{
			if (s->_right && s->_right->_color == RED) flag = false; // � ������ ���� ������ ���� ������
			if (s->_left && s->_left->_color == BLACK) flag = false; // � ����� - �������

			if (flag)
			{
				s->_color = RED; // ������ ������ � ������� (������� ������ ����� ���� ������)
				s->_left->_color = BLACK; // � ����� ���� ������ ������, ��� ����� �� ����� 
				rotRight(s); // ������ �������� (���� ���� ����������� ��� ����� �����, �� ���� s)
			}
		}

		flag = true;
		
		if (p->_right == n) // ���� ��� ������ ������
		{
			if (s->_left && s->_left->_color == RED) flag = false; // ����������� ������� ��� ��� ������ ������
			if (s->_right && s->_right->_color == BLACK) flag = false;

			if (flag)
			{
				s->_color = RED; // ���� - �������
				s->_right->_color = BLACK; // ���� - ����� ��������
				rotLeft(s); // ����� �������� ��� ���������� ������� ������� �����
			}
		}
	}

	delete_case6(n, p);
}

template<typename Element, typename Compar> // ������ ������ ��������
void RBTree<Element, Compar>::delete_case6(NodeRB * n, NodeRB *p) // ������ ����� ����� - �������, ��� ��������� ������
{
	NodeRB* s = p->_left == n ? p->_right : p->_left; // �������� ����� ���������� ��������

	s->_color = p->_color; // ���� ����� ����� ����� ��������
	p->_color = BLACK; // �������� ������ ������

	if (p->_left == n) // ����� ������
	{
		s->_right->_color = BLACK; // ������ ���� ������
		rotLeft(p); // ����� �������� ������ ��������
	}
	else // ������ ������
	{
		s->_left->_color = BLACK; // ����� ���� ������
		rotRight(p); // ������ �������� ������ ��������
	}
}

template<typename Element, typename Compar> // ������ ������ ���� ������
void RBTree<Element, Compar>::rbtransplant(NodeRB * u, NodeRB * v)
{
	if (!u->_parent) _root = v;
	else if (u == u->_parent->_left) u->_parent->_left = v;
	else u->_parent->_right = v;

	if (v) v->_parent = u->_parent;
}

template<typename Element, typename Compar> // �������� ��������
void RBTree<Element, Compar>::remove(const Element & key)
{
	NodeRB* finded = find(key); // ������ ����

	//if (finded == nullptr) throw std::invalid_argument("Key not in tree!"); // ���� ���� ��� � ������
	if (finded == nullptr) return;
	
	NodeRB* y = finded; // ������ �� ��������� ����

	NodeRB* x = nullptr; // � ���������� ����� ������������ ��� ������ �� �������

	if (!finded->_left) // ���� � ���������� ���� ��� ������ �������
	{
		x = finded->_right; // �������� ��������� ����

		rbtransplant(finded, x); // ������ �������� (������� ����� ���� null)

		if (y->_color == BLACK) // ���� ��������� ���� - ������
		{
			if (x && x->_color == RED) { x->_color = BLACK; } // ���� ����� - �������, �� ������ ������������� ������� � ������
			else { delete_case1(x, finded->_parent); } // ����� ������� ��������� ������� ��� �������� ���������� ������ �� ������
		}

		y = nullptr; // ����������, ����� ���� ����� 0xDDDDDD
		delete y; // ������� ����
	}
	else if (!finded->_right) // ���� � ���������� ���� ��� ������� �������
	{
		x = finded->_left; // �������� ��������� ����

		rbtransplant(finded, x); // ����� �������� (������� ����� ���� null)

		if (y->_color == BLACK) // ���� ��������� ���� - ������
		{
			if (x && x->_color == RED) { x->_color = BLACK; } // ���� ����� - �������, �� ������ ������������� ������� � ������
			else { delete_case1(x, finded->_parent); } // ����� ������� ��������� ������� ��� �������� ���������� ������ �� ������
		}

		y = nullptr; // ����������, ����� ���� ����� 0xDDDDDD
		delete y; // ������� ����
	}
	else
	{
		y =  maximum(finded->_left); // ������� ������������ ������� �� ������ ���-������
		int yoc = y->_color; // �������� ���� ����� ����
		x = y->_left; // ������ �� ������ (������� ����� ���� null). ������� ������� �� ����������� ���.

		if (y->_parent == finded) { if (x) x->_parent = finded; } // ���� ����. ������� - ������ ������� ���������� ��������
		else 
		{
			rbtransplant(y, y->_right); // ������ ���� �� ������ ���� (�� ����� null)
			y->_left = finded->_left; // ������ ��
			y->_left->_parent = y; // ������ ������� � �������� ������ ���� ���������
		}

		rbtransplant(finded, y); // ������ ���������� ���� �� ����. ���� � ����� ���������
		y->_right = finded->_right; // ���������� ������
		y->_right->_parent = y; // �� �������� � ������� ������� (�.�. ������ ������� � ���� null)
		y->_color = finded->_color; // ����������

		if (yoc == BLACK) // ���� ��������� ���� - ������, ��, ��������, �������� ���� �� ������� ��-������
		{
			if (x && x->_color == RED) { x->_color = BLACK; } // ���� ������� ������� - �� ������������� ��� ��� ������������ � ���������� ������ ����� � ������ �� ������������� ����
			else { delete_case1(x, y); } // ������������� ������ ��������� ������������
		}

		x = nullptr; // ������� �������,
		delete x; // ��� ��� �������� �� ������� ������������ � ��������, �, ��������������, ���������� ������� �������
	}
}

template <typename Element, typename Compar> // ����� �������� � ������
typename RBTree<Element, Compar>::NodeRB* RBTree<Element, Compar>::find(const Element& key) const
{
	NodeRB* it = _root;

	while (it && it->_key != key) // ���� �� ������
	{
		if (_compar(it->_key, key)) it = it->_right;
		else it = it->_left;
	}

	return it;
}

template <typename Element, typename Compar >
typename RBTree<Element, Compar>::NodeRB* RBTree<Element, Compar>::insertNewBstEl(const Element& key)
{
	if (!_root) return _root = new NodeRB(key, nullptr, nullptr, nullptr, RED);

	NodeRB* it = _root; // ������� ��������
	NodeRB* prev = nullptr; // ���������� ���� ���������

	while (it)
	{
		//if (it->_key == key) throw std::invalid_argument("Key already in tree!"); // ���� ���� ��� � ������

		prev = it;

		if (_compar(it->_key, key)) // ���� �� ����� �����
		{
			if (it->_right) { it = it->_right; continue; }
			else { return it->_right = new NodeRB(key, nullptr, nullptr, prev, RED); }
		}
		else // ��� �� ������
		{
			if (it->_left) { it = it->_left; continue; }
			else { return it->_left = new NodeRB(key, nullptr, nullptr, prev, RED); }
		}
	}

	return it;
}


template <typename Element, typename Compar >
typename RBTree<Element, Compar>::NodeRB* RBTree<Element, Compar>::rebalanceDUG(NodeRB* nd)
{
    // ��������� � ���� ����� ��� ��������, ��� ���� ���� (� ��� ��� ������� ���� �� ��������)
    
	NodeRB* grandPa = nd->_parent->_parent; // ���

	if (!grandPa) return nd->_parent; // ������ ���� ���������

    NodeRB* uncle = nd->_parent->_parent->_left == nd->_parent 
		? grandPa->_right 
		: grandPa->_left; // ��� ������ ������ ����� ������ ���� � ��������.

    if (uncle && uncle->_color == RED) // ���� ���� ����� �� �������, ��� ��� ��� � ��� ����...
    { // ���� � ���� ������ � ������, � ������� ��� ���������������� �����
		nd->_parent->_color = BLACK;
		uncle->_color = BLACK;
		grandPa->_color = RED;
		 
        // ������ ����������� ������ "����-����-�������-����" � �-�-�-�, �� ���� �����������, ��� ���
        // � �������� � ��� ��������, ������� ��������� � ��������
		return grandPa;
    }

    // ���� ������, �������, �������� �� ���� "���������-������" � �������
    if (nd->_parent->_parent->_left == nd->_parent 
		&& nd->_parent->_right == nd) // ��� ������ ������ ����� ������ ����
    {                  
		rotLeft(nd->_parent); // CASE2 � ��������
		nd = nd->_left;
    }

	if (nd->_parent->_parent->_right == nd->_parent 
		&& nd->_parent->_left == nd) // ��� ������� ������ ����� ����� ����
	{
		rotRight(nd->_parent); // CASE2 � ��������
		nd = nd->_right;
	}

	nd->_parent->_color = BLACK;

	nd->_parent->_parent->_color = RED; // ���� � �������

	if (nd->_parent->_left == nd 
		&& nd->_parent->_parent->_left == nd->_parent) rotRight(nd->_parent->_parent);
	else rotLeft(nd->_parent->_parent); /* (n == n->parent->right) && (n->parent == g->right) */

    return nd;
}


template <typename Element, typename Compar >
void RBTree<Element, Compar>::rebalance(NodeRB* nd)
{
	while (nd->_parent && nd->_parent->_color == RED) // ���� ���� � ����� ����������� ��������, ���������
	{
		nd = rebalanceDUG(nd); // ��������� ���������������� ��������� "����, ����, �������" � ��������� ��������
    } 

	_root->_color = BLACK; // ������ ������ ������
}

template <typename Element, typename Compar>
void RBTree<Element, Compar>::rotLeft(typename RBTree<Element, Compar>::NodeRB* nd)
{
    NodeRB* y = nd->_right; // ������ �������, ������� ������ ����� ������ �������� "����"

    if (!y) throw std::invalid_argument("Can't rotate left since the right child is nil");

	y->_parent = nd->_parent; 

	if (nd->_parent)
	{
		if (nd->_parent->_left == nd) nd->_parent->_left = y;
		else nd->_parent->_right = y;
	}

	nd->_right = y->_left;

	if (y->_left) y->_left->_parent = nd;

	NodeRB* r = nd->_parent;
	nd->_parent = y; 
	y->_left = nd;
	if (!r) _root = y;
}

template <typename Element, typename Compar>
void RBTree<Element, Compar>::rotRight(typename RBTree<Element, Compar>::NodeRB* nd)
{
	NodeRB* y = nd->_left; // ����� �������, ������� ������ ����� ������� �������� "����"

	if (!y) throw std::invalid_argument("Can't rotate left since the right child is nil");

	y->_parent = nd->_parent;

	if (nd->_parent)
	{
		if (nd->_parent->_left == nd) nd->_parent->_left = y;
		else nd->_parent->_right = y;
	}

	nd->_left = y->_right;

	if (y->_right) y->_right->_parent = nd;

	NodeRB* r = nd->_parent;
	nd->_parent = y;
	y->_right = nd;
	if (!r) _root = y;
}

template <typename Element, typename Compar>
void RBTree<Element, Compar>::Print(std::ostream& os)
{
   	os << std::endl << "------------------------------" << std::endl;
   	PrintInternal(_root, os);
   	os << std::endl << "------------------------------" << std::endl;
}

template <typename Element, typename Compar>
void RBTree<Element, Compar>::PrintInternal(NodeRB* node, std::ostream& os)
{
    if (node)
    {
    	PrintInternal(node->_left, os);
    	os << node->_key << " ";
    	PrintInternal(node->_right, os);
	}
}


template <typename Element, typename Compar>
void RBTree<Element, Compar>::Scanner(const std::string filenameIn, const std::string filenameOut)
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
} // namespace Tree


