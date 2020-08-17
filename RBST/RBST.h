#pragma once

#include <iostream>
#include <algorithm>

namespace Tree
{
/** \brief Рандромизированное бинарное дерево поиска. */
template<typename Element, typename Compar = std::less<Element>>
class RandomizedBinarySearchTree
{
protected:
	/** \brief Узел РБДП. */
	class RBSTNode
	{
	public:
		RBSTNode *_left, *_right; ///< левый и правый потомок
		int _priority; ///< приоритет узла - от него зависит вставка
		Element _key; ///< хранимая информация
	
		RBSTNode(Element value, 
			int priority = 10000,
			RBSTNode* left = nullptr, 
			RBSTNode* right = nullptr)
			: _key(value), _left(left), _right(right), _priority(priority)
		{
		}
	};
	
	RBSTNode *_root; ///< корень дерева
	Compar _compar;
	
public:
	
	RandomizedBinarySearchTree();

	/** \brief Пустое ли дерево. */
	const bool isEmpty() const { return _root == nullptr; }

	/** \brief Вспомогательный метод вставки в дерево. */
	void insert(const Element& X) { _root = insert(X, _root); }

	/** \brief Вспомогательный метод числа узлов в дереве. */
	size_t countNodes() { return countNodes(_root); }

	/** \brief Минимум дерева. */
	RBSTNode* minimum()
	{
		RBSTNode* it = _root;
		while (it->_left) it = it->_left;
		return it;
	}

	/** \brief Максимум дерева. */
	RBSTNode* maximum()
	{
		RBSTNode* it = _root;
		while (it->_right) it = it->_right;
		return it;
	}

	/** \brief Вспомогательный метод поиска в дереве. */
	RBSTNode* find(const Element X) { return search(_root, X); }
	
	/** \brief Вспомогательный метод печати дерева. */
	void Print(std::ostream& os = std::cout) { Print(_root, os); }

	/** \brief Вспомогательный метод высоты дерева. */
	size_t height() { return height(_root); }

	/** \brief Вспомогательный метод изъятия из дерева. */
	void remove(const Element value) { _root = remove(_root, value); }
	
	/** \brief Тест дерева. */
	void Scanner(const std::string filenameIn, const std::string filenameOut);
	
protected:
	
	/** \brief Вставка в дерево. */
	RBSTNode *insert(const Element& X, RBSTNode *T);
	
	/** \brief Число узлов дерева. */
	size_t countNodes(RBSTNode *r);
	
	/** \brief Поиск в дереве. */
	RBSTNode* search(RBSTNode *r, const Element val);
	
	/** \brief Печать дерева. */
	void Print(RBSTNode *r, std::ostream& os);
	
	/** \brief Высота дерева. */
	size_t height(RBSTNode *node);
	
	/** \brief Изъятие из дерева. */
	RBSTNode* remove(RBSTNode* node, const Element key);
	
	/** \brief Очистка дерева. */
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
