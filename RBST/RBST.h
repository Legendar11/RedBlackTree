#pragma once

#include <iostream>
#include <algorithm>

namespace Tree
{
/** \brief ������������������ �������� ������ ������. */
template<typename Element, typename Compar = std::less<Element>>
class RandomizedBinarySearchTree
{
protected:
	/** \brief ���� ����. */
	class RBSTNode
	{
	public:
		RBSTNode *_left, *_right; ///< ����� � ������ �������
		int _priority; ///< ��������� ���� - �� ���� ������� �������
		Element _key; ///< �������� ����������
	
		RBSTNode(Element value, 
			int priority = 10000,
			RBSTNode* left = nullptr, 
			RBSTNode* right = nullptr)
			: _key(value), _left(left), _right(right), _priority(priority)
		{
		}
	};
	
	RBSTNode *_root; ///< ������ ������
	Compar _compar;
	
public:
	
	RandomizedBinarySearchTree();

	/** \brief ������ �� ������. */
	const bool isEmpty() const { return _root == nullptr; }

	/** \brief ��������������� ����� ������� � ������. */
	void insert(const Element& X) { _root = insert(X, _root); }

	/** \brief ��������������� ����� ����� ����� � ������. */
	size_t countNodes() { return countNodes(_root); }

	/** \brief ������� ������. */
	RBSTNode* minimum()
	{
		RBSTNode* it = _root;
		while (it->_left) it = it->_left;
		return it;
	}

	/** \brief �������� ������. */
	RBSTNode* maximum()
	{
		RBSTNode* it = _root;
		while (it->_right) it = it->_right;
		return it;
	}

	/** \brief ��������������� ����� ������ � ������. */
	RBSTNode* find(const Element X) { return search(_root, X); }
	
	/** \brief ��������������� ����� ������ ������. */
	void Print(std::ostream& os = std::cout) { Print(_root, os); }

	/** \brief ��������������� ����� ������ ������. */
	size_t height() { return height(_root); }

	/** \brief ��������������� ����� ������� �� ������. */
	void remove(const Element value) { _root = remove(_root, value); }
	
	/** \brief ���� ������. */
	void Scanner(const std::string filenameIn, const std::string filenameOut);
	
protected:
	
	/** \brief ������� � ������. */
	RBSTNode *insert(const Element& X, RBSTNode *T);
	
	/** \brief ����� ����� ������. */
	size_t countNodes(RBSTNode *r);
	
	/** \brief ����� � ������. */
	RBSTNode* search(RBSTNode *r, const Element val);
	
	/** \brief ������ ������. */
	void Print(RBSTNode *r, std::ostream& os);
	
	/** \brief ������ ������. */
	size_t height(RBSTNode *node);
	
	/** \brief ������� �� ������. */
	RBSTNode* remove(RBSTNode* node, const Element key);
	
	/** \brief ������� ������. */
	void clear(RBSTNode* node)
	{
		if (node)
		{
			clear(node->_left);
			clear(node->_right);
			node = nullptr;
			delete node;
		}
	}
};
} // namespace Tree

#include "RBST.hpp"
