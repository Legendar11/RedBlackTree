/// "����������" ��������������� ������� ������������� � ����� rbtree.hpp.
///
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace Tree
{

/** \brief ������� ����� ������-������� ������.
 *
 *  \tparam Element ���������� ��� ���������, �������� � ������ (��. ����, key).
 *  \tparam Compar �������, ����������� ��������� ��������� ��� ����������� �������. �� ���������
 *  ����������� ����������� ������������ \c std::less.
 */
template <typename Element, typename Compar = std::less<Element> >
class RBTree
{
public:
    /** \brief ��� ����� ���� ������. */
    enum Color
    {
        BLACK,
        RED
    };

    /** \brief ���� ���.
     *
     *  ������� ����� ��������� ������ �������� �������� ��� �������� ���� � ��������� ������
     *  ��� ������ ���� � ��� ��������. ��� ������� � ����� ������������, � ���� ������ ���������
     *  �� ��������� � ������� ������ �������������, ����� ��� ����� ������ � ����� �����.
     */
    class NodeRB 
	{    
        // ������ ����� ������ ������ � ���������� ����!
        friend class RBTree<Element, Compar>;

    public:

        /** \brief ���������� �������� �������������� ���� ������������ ��������. */
        enum WhichChild 
		{
            LEFT,               ///< ����� �������
            RIGHT,              ///< ������ �������
            NONE                ///< ������ �� �������
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

        ~NodeRB()	///< ���������� ����.          
		{
			if (_left) delete _left;
    		if (_right) delete _right;
		}                                      

    protected:
        NodeRB(const NodeRB&);                      ///< �� �� ��������.
        NodeRB& operator= (NodeRB&);                ///< �������� ������������ ����������.
        
     protected:
        NodeRB*   _parent;                      	///< ��������.
        Color   _color;                         	///< ����.
        
        Element _key;                           	///< ������� ����� ����������.

        NodeRB*   _left;                          	///< ����� �������.
        NodeRB*   _right;                         	///< ������ �������.
    }; // class RBTree::NodeRB

    friend class NodeRB;

public:
    RBTree() : _root(nullptr) { } 		        	///< ����������� �� ���������.   

public:
    // �������� �������� ��� �������

    /** \brief ��������� ������� \c key � ������.
     *
     *
     *  �.�. ��������� �� ���������, �������� � ������ \c key � ������ ���� �� ������. ����
     *  �� ����� ������� ��� ����������, ������������ �������������� �������� \c std::invalid_argument.
     */
    void insert(const Element& key);
    
    /** \brief ���� ����, ��������������� ����� \c key, � ������� ���� �� ������ 
     *  � ����������� �����������������. 
     *
     *  ���� ���������������� ����� ��� � ������, ���������� �������������� �������� \c std::invalid_argument.
     */
    void remove(const Element& key); 
    
    /** \brief ���� ������� \c key � ������ � ���������� ��������������� ��� ����. 
     *
     *  \returns ���� �������� \c key, ���� �� ���� � ������, ����� \c nullptr.
     */
    NodeRB* find(const Element& key) const;	
	
	NodeRB* minimum(NodeRB* node = nullptr) const ///< ���������� ������������ �������� � ������.
	{
		NodeRB* it = node ? node : _root;
		while (it->_left) it = it->_left;
		return it;
	} 
	
	NodeRB* maximum(NodeRB* node = nullptr) const ///< ���������� ������������� �������� � ������.
	{
		NodeRB* it =  node ? node : _root;
		while (it->_right) it = it->_right;
		return it;
	} 
	
	/** \brief ���������� ������, ���� ������ �����, ���� �����. */
    bool isEmpty() const { return _root == nullptr; }

    /** \brief ���������� ������������ ��������� �� �������� �������. */
    const NodeRB* getRoot() const { return _root;  }
	
	/** \brief ������ ������. */
	void Print(std::ostream& os = std::cout);
	
	/** \brief ���� ������. */
	void Scanner(const std::string filenameIn, const std::string filenameOut);
	
protected:
	void delete_case1(NodeRB *n, NodeRB *p); ///< ������ ������� �������������� ����� ��������. 
	void delete_case2(NodeRB *n, NodeRB *p); ///< ������ ������� �������������� ����� ��������. 
	void delete_case3(NodeRB *n, NodeRB *p); ///< ������ ������� �������������� ����� ��������. 
	void delete_case4(NodeRB *n, NodeRB *p); ///< ��������� ������� �������������� ����� ��������. 
	void delete_case5(NodeRB *n, NodeRB *p); ///< ����� ������� �������������� ����� ��������. 
	void delete_case6(NodeRB *n, NodeRB *p); ///< ������ ������� �������������� ����� ��������. 

	void rbtransplant(NodeRB* u, NodeRB* v); ///< �������� ���� ���� ������

	void clear(NodeRB* node) ///< �������
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

    /** \brief ��������� ����� ���� � ������, ��� � ������� BST. 
     *
     *  ��������� �� ���������, ���������� �� ��, ��� � � \c insert().
     *  \return ��������� �� ��������������� �������.
     */
    NodeRB* insertNewBstEl(const Element& key);

    /** \brief ��������� ���������������� ������ ����� ���������� ������ �������� � ���� \c nd. 
     */
    void rebalance(NodeRB* nd);

    /** \brief ��������� ���������������� ��������� ������� ���� \c nd: ����, ���� � �������.
     *
     *  \returns ����� ���������� ����, ��� �������� ����� ���������� �������.
     */
    NodeRB* rebalanceDUG(NodeRB* nd);

    /** \brief ������� ��� �� ����� ��� ���������, ���������� ������ ��-��� ���. */
    void deleteNodeRB(NodeRB* nd);

    /** \brief ������� ��������� ������������ ���� \c nd �����.
     *
     *  ����������: ������ ������� ���� \c nd �� ������ ���� null, ����� ������������
     *  �������������� �������� \c std::invalid_argument.
     * 
     *  \return ���� � �������� ���� ��� ���������� �����, ���������� ��� (��� ����������� �������������),
     *  ������ ���������� \c nullptr.
     */
    void rotLeft(NodeRB* nd);

    /** \brief ������� ��������� ������������ ���� \c nd ������. ������� � ����������� 
      * ���������� (�����������) ������ ��������. 
      */
    void rotRight(NodeRB* nd);
    
    /** \brief ��������������� ����� ������.
      */
    void PrintInternal(NodeRB* node, std::ostream& os);
	
protected:
    RBTree(const RBTree&);                      ///< �� �� ��������.
    RBTree& operator= (RBTree&);                ///< �������� ������������ ����������.

protected:
    Compar _compar;                             ///< ���������� ��������� ���� ���������.

protected:
    // ��������� ������

    /** \brief �������� �������� ������� ������. ���� \c nullptr, ������ ������ �����. 
     *
     *  NB: ������ ������������ ����������, ��� ������ ������ �������� �� ���������������
     *  � ���� ����������� ����, � � ���� ������� ������� ������������ ����������� (sentinel)
     *  �������� ������-�����. ��� ��������� ��������� ��������� ������ ��������� �� ���� ����,
     *  ��� ����������� ������������� �������� ����������� ������� � ������. ������ ���, 
     *  � ���� �������, ������� ����������� ���������� ��������, ��� ����� ������� �����������
     *  ���� ������ ����� ����� ������� -INF, ���, � ���� �������, ����������� ��������������
     *  ����������� �� �������� ��������� ���������, ������� � ��������� ���������� �� ������������.
     */
    NodeRB* _root;
}; // class RBTree
} // namespace Tree

#include "RBTree.hpp"

