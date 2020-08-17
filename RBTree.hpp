/// "Реализация" (шаблонов) методов, описанных в файле RBTree.h
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
{ // если переданный узел не существует, просто ничего не делаем, т.к. в вызывающем проверок нет
    if (nd == nullptr) return;
    delete nd; // потомков убьет в деструкторе
}


template <typename Element, typename Compar >
void RBTree<Element, Compar>::insert(const Element& key)
{
    NodeRB* newNodeRB = insertNewBstEl(key); // вставка как в бинарное дерево поиска
    //std::cout << key; getchar();
    //Print(); getchar();
    rebalance(newNodeRB); // перебалансировка
}

template<typename Element, typename Compar> // первый случай удаления
void RBTree<Element, Compar>::delete_case1(NodeRB * n, NodeRB *p) // проверка, удалялся ли корень
{
	if (p) delete_case2(n, p); // если удаляемый элемент - не корень, то идем дальше
}

template<typename Element, typename Compar> // второй случай удаления
void RBTree<Element, Compar>::delete_case2(NodeRB * n, NodeRB *p) // брат - красный, все остальные - черные
{
	NodeRB* s = p->_left == n ? p->_right : p->_left; // получаем брата удаляемого элемента

	if (s->_color == RED) // если брат - красный
	{
		p->_color = RED; // красим родителя в красный
		s->_color = BLACK; // красим брата в черный

		if (p->_left == n) rotLeft(p); // левый
		else rotRight(p); // и правый случаи перебалансировки
	}

	delete_case3(n, p); // продолжаем, т.к. свойство КЧ-дерева о количестве красных и черных узлов все еще нарушено
}

template<typename Element, typename Compar> // третий случай удаления
void RBTree<Element, Compar>::delete_case3(NodeRB * n, NodeRB *p) // все черные
{
	NodeRB* s = p->_left == n ? p->_right : p->_left; // получаем брата удаляемого элемента
	bool flag = true;

	if (p->_color == BLACK && // если родитель - черный
		s->_color == BLACK) // и брат - черный
	{
		if (s->_left && s->_left->_color == RED) flag = false; // и дети брата
		if (s->_right && s->_right->_color == RED) flag = false; // черные

		if (flag)
		{
			s->_color = RED; // покраска брата в красный (красный должен иметь двух черных)
			delete_case1(p, p->_parent); // проверка покраски родителя
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

template<typename Element, typename Compar> // четвертый случай удаления
void RBTree<Element, Compar>::delete_case4(NodeRB * n, NodeRB *p) // родитель - красный, все остальные - черные
{
	NodeRB* s = p->_left == n ? p->_right : p->_left; // получаем брата удаляемого элемента
	bool flag = true;

	if (p->_color == RED && // если ордитель - красный
		s->_color == BLACK) // а брат - черный
	{
		if (s->_left && s->_left->_color == RED) flag = false; // и дети брата
		if (s->_right && s->_right->_color == RED) flag = false; // черные

		if (flag)
		{
			s->_color = RED; // брат будет красным (красный должен иметь двух черных)
			p->_color = BLACK; // а родитель - черным
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

template<typename Element, typename Compar> // пятый случай удаления
void RBTree<Element, Compar>::delete_case5(NodeRB * n, NodeRB *p) // левое дитя брата красное, все остальные - черные
{
	NodeRB* s = p->_left == n ? p->_right : p->_left; // получаем брата удаляемого элемента
	bool flag = true;

	//std::cout << "on case 5"; getchar();

	if (s->_color == BLACK) // если брат - черный
	{
		if (p->_left == n) // если это левый случай
		{
			if (s->_right && s->_right->_color == RED) flag = false; // и правое дитя должно быть черным
			if (s->_left && s->_left->_color == BLACK) flag = false; // а левое - красным

			if (flag)
			{
				s->_color = RED; // красим братца в красное (красный должен иметь двух черных)
				s->_left->_color = BLACK; // а левое дитя станет черным, как уголь на снегу 
				rotRight(s); // правое вращение (дабы дитя возвысилось над своим отцом, то есть s)
			}
		}

		flag = true;
		
		if (p->_right == n) // если это правый случай
		{
			if (s->_left && s->_left->_color == RED) flag = false; // аналогичные условия как для левого случая
			if (s->_right && s->_right->_color == BLACK) flag = false;

			if (flag)
			{
				s->_color = RED; // брат - красный
				s->_right->_color = BLACK; // дитя - цвета гуталина
				rotLeft(s); // левое вращение для возвышения правого потомка брата
			}
		}
	}

	delete_case6(n, p);
}

template<typename Element, typename Compar> // шестой случай удаления
void RBTree<Element, Compar>::delete_case6(NodeRB * n, NodeRB *p) // правый поток брата - красный, все остальные черные
{
	NodeRB* s = p->_left == n ? p->_right : p->_left; // получаем брата удаляемого элемента

	s->_color = p->_color; // цвет брата равен цвету родителя
	p->_color = BLACK; // родитель отныне черный

	if (p->_left == n) // левый случай
	{
		s->_right->_color = BLACK; // правый брат черный
		rotLeft(p); // левое вращение вокруг родителя
	}
	else // правый случай
	{
		s->_left->_color = BLACK; // левый брат черный
		rotRight(p); // правое вращение вокруг родителя
	}
}

template<typename Element, typename Compar> // замена одного узла другим
void RBTree<Element, Compar>::rbtransplant(NodeRB * u, NodeRB * v)
{
	if (!u->_parent) _root = v;
	else if (u == u->_parent->_left) u->_parent->_left = v;
	else u->_parent->_right = v;

	if (v) v->_parent = u->_parent;
}

template<typename Element, typename Compar> // удаление элемента
void RBTree<Element, Compar>::remove(const Element & key)
{
	NodeRB* finded = find(key); // поиска узла

	//if (finded == nullptr) throw std::invalid_argument("Key not in tree!"); // если узла нет в дереве
	if (finded == nullptr) return;
	
	NodeRB* y = finded; // ссылка на найденный узел

	NodeRB* x = nullptr; // в дальнейшем будем использовать для ссылки на потомка

	if (!finded->_left) // если у найденного узла нет левого потомка
	{
		x = finded->_right; // заменяем найденный узел

		rbtransplant(finded, x); // правым потомком (который может быть null)

		if (y->_color == BLACK) // если найденный узел - черный
		{
			if (x && x->_color == RED) { x->_color = BLACK; } // если поток - красный, то просто перекрашиваем потомка в черный
			else { delete_case1(x, finded->_parent); } // иначе смотрим различные условия для проверки сохранения свойст КЧ дерева
		}

		y = nullptr; // необходимо, иначе узел будет 0xDDDDDD
		delete y; // удаляем узел
	}
	else if (!finded->_right) // если у найденного узла нет правого потомка
	{
		x = finded->_left; // заменяем найденный узел

		rbtransplant(finded, x); // левым потомком (который может быть null)

		if (y->_color == BLACK) // если найденный узел - черный
		{
			if (x && x->_color == RED) { x->_color = BLACK; } // если поток - красный, то просто перекрашиваем потомка в черный
			else { delete_case1(x, finded->_parent); } // иначе смотрим различные условия для проверки сохранения свойст КЧ дерева
		}

		y = nullptr; // необходимо, иначе узел будет 0xDDDDDD
		delete y; // удаляем узел
	}
	else
	{
		y =  maximum(finded->_left); // возьмем максимальный элемент из левого под-дерева
		int yoc = y->_color; // запомним цвет этого узла
		x = y->_left; // ссылка на потока (который может быть null). Правого потомка по определению нет.

		if (y->_parent == finded) { if (x) x->_parent = finded; } // если макс. элемент - прямой потомок найденного элемента
		else 
		{
			rbtransplant(y, y->_right); // замена узла на правый узел (он будет null)
			y->_left = finded->_left; // ссылки на
			y->_left->_parent = y; // левого потомка и родителя должны быть обновлены
		}

		rbtransplant(finded, y); // замена найденного узла на макс. узел в левом поддереве
		y->_right = finded->_right; // обновление ссылок
		y->_right->_parent = y; // на родителя и правого потомка (т.к. правый потомок у есть null)
		y->_color = finded->_color; // перекраска

		if (yoc == BLACK) // если удаляемый узел - черный, то, возможно, нарушено одно из свойств КЧ-дерева
		{
			if (x && x->_color == RED) { x->_color = BLACK; } // если потомок красный - то перекрашиваем его для корректности и количестве черных узлов в дереве по определенному пути
			else { delete_case1(x, y); } // рассматриваем случаи наркшения балансировки
		}

		x = nullptr; // удаляем потомка,
		delete x; // так как значение из потомка перекочевало в родителя, и, соответственно, необходимо удалить потомка
	}
}

template <typename Element, typename Compar> // поиск значения в дереве
typename RBTree<Element, Compar>::NodeRB* RBTree<Element, Compar>::find(const Element& key) const
{
	NodeRB* it = _root;

	while (it && it->_key != key) // пока не найдем
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

	NodeRB* it = _root; // текущий итератор
	NodeRB* prev = nullptr; // предыдущий узел итератора

	while (it)
	{
		//if (it->_key == key) throw std::invalid_argument("Key already in tree!"); // если ключ уже в дереве

		prev = it;

		if (_compar(it->_key, key)) // идем по одной ветви
		{
			if (it->_right) { it = it->_right; continue; }
			else { return it->_right = new NodeRB(key, nullptr, nullptr, prev, RED); }
		}
		else // или по другой
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
    // попадание в этот метод уже означает, что папа есть (а вот про дедушку пока не известно)
    
	NodeRB* grandPa = nd->_parent->_parent; // дед

	if (!grandPa) return nd->_parent; // должно быть корректно

    NodeRB* uncle = nd->_parent->_parent->_left == nd->_parent 
		? grandPa->_right 
		: grandPa->_left; // для левого случая нужен правый дядя и наоборот.

    if (uncle && uncle->_color == RED) // если дядя такой же красный, как сам нод и его папа...
    { // дядю и папу красим в черное, а дедушку — в коммунистические цвета
		nd->_parent->_color = BLACK;
		uncle->_color = BLACK;
		grandPa->_color = RED;
		 
        // теперь чередование цветов "узел-папа-дедушка-дядя" — К-Ч-К-Ч, но надо разобраться, что там
        // с дедушкой и его предками, поэтому продолжим с дедушкой
		return grandPa;
    }

    // дядя черный, смотрим, является ли узел "правильно-правым" у папочки
    if (nd->_parent->_parent->_left == nd->_parent 
		&& nd->_parent->_right == nd) // для левого случая нужен правый узел
    {                  
		rotLeft(nd->_parent); // CASE2 в действии
		nd = nd->_left;
    }

	if (nd->_parent->_parent->_right == nd->_parent 
		&& nd->_parent->_left == nd) // для правого случая нужен левый узел
	{
		rotRight(nd->_parent); // CASE2 в действии
		nd = nd->_right;
	}

	nd->_parent->_color = BLACK;

	nd->_parent->_parent->_color = RED; // деда в красный

	if (nd->_parent->_left == nd 
		&& nd->_parent->_parent->_left == nd->_parent) rotRight(nd->_parent->_parent);
	else rotLeft(nd->_parent->_parent); /* (n == n->parent->right) && (n->parent == g->right) */

    return nd;
}


template <typename Element, typename Compar >
void RBTree<Element, Compar>::rebalance(NodeRB* nd)
{
	while (nd->_parent && nd->_parent->_color == RED) // пока папа — цвета пионерского галстука, действуем
	{
		nd = rebalanceDUG(nd); // локальная перебалансировка семейства "папа, дядя, дедушка" и повторная проверка
    } 

	_root->_color = BLACK; // корень всегда черный
}

template <typename Element, typename Compar>
void RBTree<Element, Compar>::rotLeft(typename RBTree<Element, Compar>::NodeRB* nd)
{
    NodeRB* y = nd->_right; // правый потомок, который станет после левого поворота "выше"

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
	NodeRB* y = nd->_left; // левый потомок, который станет после правого поворота "выше"

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


