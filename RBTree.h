/// "Реализация" соответствующих методов располагается в файле rbtree.hpp.
///
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace Tree
{

/** \brief Главный класс красно-черного дерева.
 *
 *  \tparam Element Определяет тип элементов, хранимых в дереве (тж. ключ, key).
 *  \tparam Compar Функтор, выполняющий сравнение элементов для определения порядка. По умолчанию
 *  реализуется стандартным компаратором \c std::less.
 */
template <typename Element, typename Compar = std::less<Element> >
class RBTree
{
public:
    /** \brief Тип цвета узла дерева. */
    enum Color
    {
        BLACK,
        RED
    };

    /** \brief Узел КЧД.
     *
     *  Большая часть элементов класса является закрытой для внешнего мира и доступной только
     *  для самого узла и его потомков. Это сделано с целью инкапсуляции, а само дерево объявлено
     *  по отношению к данному классу дружественным, чтобы оно имело доступ к своим узлам.
     */
    class NodeRB 
	{    
        // Дерево имеет полный доступ к реализации узла!
        friend class RBTree<Element, Compar>;

    public:

        /** \brief Определяет варианты принадлежности узла относительно родителя. */
        enum WhichChild 
		{
            LEFT,               ///< левый потомок
            RIGHT,              ///< правый потомок
            NONE                ///< вообще не потомок
        };
        
    protected:
        NodeRB(const Element& key = Element(),
            NodeRB* left = nullptr,
            NodeRB* right = nullptr,
            NodeRB* parent = nullptr,
            Color col = BLACK)
            : _key(key), _left(left), _right(right), _parent(parent), _color(col)
        {
        }

        ~NodeRB()	///< Деструктор нода.          
		{
			if (_left) delete _left;
    		if (_right) delete _right;
		}                                      

    protected:
        NodeRB(const NodeRB&);                      ///< КК не доступен.
        NodeRB& operator= (NodeRB&);                ///< Оператор присваивания недоступен.
        
     protected:
        NodeRB*   _parent;                      	///< Родитель.
        Color   _color;                         	///< Цвет.
        
        Element _key;                           	///< Несомая узлом информация.

        NodeRB*   _left;                          	///< Левый потомок.
        NodeRB*   _right;                         	///< Правый потомок.
    }; // class RBTree::NodeRB

    friend class NodeRB;

public:
    RBTree() : _root(nullptr) { } 		        	///< Конструктор по умолчанию.   

public:
    // Основные операции над деревом

    /** \brief Вставляет элемент \c key в дерево.
     *
     *
     *  Т.к. дубликаты не допустимы, элемента с ключом \c key в дереве быть не должно. Если
     *  же такой элемент уже существует, генерируется исключительная ситуация \c std::invalid_argument.
     */
    void insert(const Element& key);
    
    /** \brief Ищет узел, соответствующий ключу \c key, и удаляет узел из дерева 
     *  с последующей перебалансировкой. 
     *
     *  Если соответствующего ключа нет в дереве, генерирует исключительную ситуацию \c std::invalid_argument.
     */
    void remove(const Element& key); 
    
    /** \brief Ищет элемент \c key в дереве и возвращает соответствующий ему узел. 
     *
     *  \returns узел элемента \c key, если он есть в дереве, иначе \c nullptr.
     */
    NodeRB* find(const Element& key) const;	
	
	NodeRB* minimum(NodeRB* node = nullptr) const ///< Нахождение минимального элемента в дереве.
	{
		NodeRB* it = node ? node : _root;
		while (it->_left) it = it->_left;
		return it;
	} 
	
	NodeRB* maximum(NodeRB* node = nullptr) const ///< Нахождение максимального элемента в дереве.
	{
		NodeRB* it =  node ? node : _root;
		while (it->_right) it = it->_right;
		return it;
	} 
	
	/** \brief Возвращает истину, если дерево пусто, ложь иначе. */
    bool isEmpty() const { return _root == nullptr; }

    /** \brief Возвращает неизменяемый указатель на корневой элемент. */
    const NodeRB* getRoot() const { return _root;  }
	
	/** \brief Печать дерева. */
	void Print(std::ostream& os = std::cout);
	
	/** \brief Тест дерева. */
	void Scanner(const std::string filenameIn, const std::string filenameOut);
	
protected:
	void delete_case1(NodeRB *n, NodeRB *p); ///< Первый вариант ребалансировки после удаления. 
	void delete_case2(NodeRB *n, NodeRB *p); ///< Второй вариант ребалансировки после удаления. 
	void delete_case3(NodeRB *n, NodeRB *p); ///< Третий вариант ребалансировки после удаления. 
	void delete_case4(NodeRB *n, NodeRB *p); ///< Четвертый вариант ребалансировки после удаления. 
	void delete_case5(NodeRB *n, NodeRB *p); ///< Пятый вариант ребалансировки после удаления. 
	void delete_case6(NodeRB *n, NodeRB *p); ///< Шестой вариант ребалансировки после удаления. 

	void rbtransplant(NodeRB* u, NodeRB* v); ///< Заменить один узел другим

	void clear(NodeRB* node) ///< Очистка
	{
		if (node)
		{
			clear(node->_left);
			clear(node->_right);
			node = nullptr;
			delete node;
		}
	}
	
protected:

    /** \brief Добавляет новый узел в дерево, как в обычном BST. 
     *
     *  Дубликаты не разрешены, исключение то же, что и у \c insert().
     *  \return Указатель на новодобавленный элемент.
     */
    NodeRB* insertNewBstEl(const Element& key);

    /** \brief Выполняет перебалансировку дерева после добавления нового элемента в узел \c nd. 
     */
    void rebalance(NodeRB* nd);

    /** \brief Выполняет перебалансировку локальных предков узла \c nd: папы, дяди и дедушки.
     *
     *  \returns Новый актуальный узел, для которого могут нарушаться правила.
     */
    NodeRB* rebalanceDUG(NodeRB* nd);

    /** \brief Удаляет нод со всеми его потомками, освобождая память из-под них. */
    void deleteNodeRB(NodeRB* nd);

    /** \brief Вращает поддерево относительно узла \c nd влево.
     *
     *  Требование: правый ребенок узла \c nd не должен быть null, иначе генерируется
     *  исключительная ситуация \c std::invalid_argument.
     * 
     *  \return Если у текущего узла был предыдущий левый, возвращает его (для дальнейшего использования),
     *  иначен возвращает \c nullptr.
     */
    void rotLeft(NodeRB* nd);

    /** \brief Вращает поддерево относительно узла \c nd вправо. Условия и ограничения 
      * аналогичны (симметрично) левому вращению. 
      */
    void rotRight(NodeRB* nd);
    
    /** \brief Вспомогательный метод печати.
      */
    void PrintInternal(NodeRB* node, std::ostream& os);
	
protected:
    RBTree(const RBTree&);                      ///< КК не доступен.
    RBTree& operator= (RBTree&);                ///< Оператор присваивания недоступен.

protected:
    Compar _compar;                             ///< Компаратор сравнения двух элементов.

protected:
    // Структура дерева

    /** \brief Реальный корневой элемент дерева. Если \c nullptr, значит дерево пусто. 
     *
     *  NB: иногда используется реализация, где корень дерева хранится не непосредственно
     *  в виде выделенного узла, а в виде правого потомка специального сторожевого (sentinel)
     *  элемента псевдо-корня. Это позволяет несколько упростить запись алгоритма за счет того,
     *  что исключается необходимость проверки специальных случаев с корнем. Однако это, 
     *  в свою очередь, требует обеспечение корректной проверки, что левый потомок сторожевого
     *  узла всегда будет иметь порядок -INF, что, в свою очередь, накладывает дополнительные
     *  ограничения на предикат сравнение элементов, поэтому в настоящей реализации не используется.
     */
    NodeRB* _root;
}; // class RBTree
} // namespace Tree

#include "RBTree.hpp"

