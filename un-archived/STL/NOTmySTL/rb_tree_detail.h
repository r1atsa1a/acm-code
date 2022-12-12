#include "rb_tree.h"
#ifndef _RB_TREE_DETAIL_H
#define _RB_TREE_DETAIL_H

namespace STL {
	template<class Value, class Ref, class Ptr>
	typename __rb_tree_iterator<Value, Ref, Ptr>::self & __rb_tree_iterator<Value, Ref, Ptr>::operator++() {
		increment();
		return *this;
	}
	template<class Value, class Ref, class Ptr>
	typename __rb_tree_iterator<Value, Ref, Ptr>::self __rb_tree_iterator<Value, Ref, Ptr>::operator++(int) {
		self tmp = *this;
		increment();
		return tmp;
	}

	template<class Value, class Ref, class Ptr>
	typename __rb_tree_iterator<Value, Ref, Ptr>::self & __rb_tree_iterator<Value, Ref, Ptr>::operator--() {
		decrement();
		return *this;
	}

	template<class Value, class Ref, class Ptr>
	typename __rb_tree_iterator<Value, Ref, Ptr>::self __rb_tree_iterator<Value, Ref, Ptr>::operator--(int) {
		self tmp = *this;
		decrement();
		return tmp;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::create_node(const value_type & x) {
		link_type tmp = get_node();
		construct(&tmp->value_field, x);
		return tmp;
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clone_node(link_type x)
	{
		return link_type();
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::destroy_node(link_type p) {
		destroy(&p->value_field);
		put_node(p);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		__insert(base_ptr x_, base_ptr y_, const value_type & v) {
		link_type x = link_type(x_);
		link_type y = link_type(y_);
		link_type z;
		if (y == header || x != 0 || key_compare(KeyOfValue()(v), Key(y))) {
			z = create_node(v);
			left(y) = z;
			if (y == header) {
				root() = z;
				rightmost() = z;
			}
			else if (leftmost() == y) {//���yΪ����ڵ�
				leftmost() = z;          //���²���ڵ�Ϊ����ڵ�
			}
		}
		else {
			z = create_node(v);
			right(y) = z;
			if (rightmost() == y) {
				rightmost() = z;
			}
		}
		parent(z) = y;
		left(z) = 0;
		right(z) = 0;
		__rebalance_rb_tree(z, header->parent);
		++node_count;
		return iterator(z);
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(link_type x, link_type p) {
		link_type top = clone_node(x);
		top->parent = p;
		if (x->right)
			top->right = __copy(right(x), top);
		p = top;
		x = left(x);

		while (x != 0) {
			link_type y = clone_node(x);
			p->left = y;
			y->parent = p;
			if (x->right)
				y->right = __copy(right(x), y);
			p = y;
			x = left(x);
		}
		return top;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x) {
		// ɾ���ڵ㣬�������µ�����
		while (x != 0) {
			__erase(right(x));
			link_type y = left(x);
			destroy_node(x);
			x = y;
		}
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::init() {
		//��header�ĸ�ָ������ҽڵ㶼ָ������
		header = get_node();
		color(header) = __rb_tree_red;
		root() = 0;
		leftmost() = header;
		rightmost() = header;
		value(header) = 0;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const Compare & comp)
		: node_count(0)
		, key_compare(comp) {
		init();
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
		: node_count(0)
		, key_compare(x.key_compare)
	{
		header = get_node();
		color(header) = __rb_tree_red;
		if (x.root() == 0) {
			root() = 0;
			leftmost() = header;
			rightmost() = header;
		}
		else {
			root() = __copy(x.root(), header);
			leftmost() = minimum(root());
			rightmost() = maximum(root());
		}
		node_count = x.node_count;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::~rb_tree()
	{
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		operator = (const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
	{
		// TODO: �ڴ˴����� return ���
	}

	//�����ڵ��ֵ�ظ��Ĳ������
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value & V) {

		link_type x = header;
		link_type y = root();
		while (x != 0) {//�Ӹ��ڵ㿪ʼѰ���ʵ���Ҷ�ӽڵ���в���
			y = x;
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
			//�������󡯵������󣬡�С���ߵ��ڵġ�������
		}
		return __insert(x, y, v);
	}

	//�������ڵ��ֵ�ظ��Ĳ�����������ظ�����Ч
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline  typename pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value & v) {
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while (x != 0) {//�Ӹ��ڵ㿪ʼѰ���ʵ���Ҷ�ӽڵ���в���
			y = x;
			comp = key_compare(KeyOfValue()(v), key(x));
			x = comp ? left(x) : right(x);
		}
		//��ʱyΪ�����ĸ��ڵ�
		iterator j = iterator(y);
		if (comp) {//����뿪whileѭ��ʱcompΪ�棬���ʾ�������y�����
			if (begin()==j ) {//��������ĸ��ڵ�Ϊ����ڵ�
				return pair<iterator, bool>(__insert(x, y, v), true);
			}
			else {//����
				//����j�ڵ�
				--j;
			}
		}
		if (key_compare(key(j.node), KeyOfValue()(v))) {
			//�¼�ֵ������нڵ�֮��ֵ�ظ�,ִ�а������
			return pair<iterator, bool>(__insert(x, y, v), true);
		}
		//���е��˴�����ʾ��ֵһ�������е�ĳ����ֵ�ظ�����ô�Ͳ����в���
		return pair<iterator, bool>(j, false);
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		__single_rotate_left(__rb_tree_node_base * x, __rb_tree_node_base *& root) {
		//XΪ��ת��
		__rb_tree_node_base* y = x->right;//yΪ��ת������ӽڵ�
		x->right = y->left;               //����x�����ӽڵ�
		if (y->left != 0)                 //��y�����ӽڵ�
			y->left->parent = x;          //����������ӽڵ�ĸ��ڵ�Ϊx
		y->parent = x->parent;            //����y�ĸ��ڵ�ָ��y���游�ڵ�
		if (x == root) {                  //xΪ���ڵ�
			root = y;
		}
		else if (x == x->parent->left) { //xΪ�丸�ڵ�����ӽڵ�
			x->parent->left = y;
		}
		else {                           //xΪ�丸�ڵ�����ӽڵ�
			x->parent->right = y;
		}
		//������ת���������������
		y->left = x;
		x->parent = y;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		__single_rotate_right(__rb_tree_node_base * x, __rb_tree_node_base *& root) {
		//ͬ__single_rotate_left
		__rb_tree_node_base* y = x->left;
		x->left = y->right;
		if (y->right != 0)
			y->right->parent = x;
		y->parent = x->parent;
		if (x == root) {
			root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
		__rebalance_rb_tree(__rb_tree_node_base * x, __rb_tree_node_base *& root) {
		x->color = __rb_tree_red;    //�½ڵ�Ϊ��
		while (x != root && x->parent->color == __rb_tree_red) {  //���ڵ�Ϊ��
			if (x->parent == x->parent->parent->left) {           //���ڵ����游�ڵ�����ӽڵ�
				__rb_tree_node_base* y = x->parent->parent->right;//yΪ�����ڵ㣨�����ڵ�ָ���ڵ���ֵܽڵ㣩
				if (y && y->color == __rb_tree_red) {             //�����һ������ �����ڵ���ڣ���Ϊ�죨�Ҹ��ڵ����游�ڵ�����ӽڵ㣩
					x->parent->color = __rb_tree_black;           //���ĸ��ڵ�Ϊ��
					y->color = __rb_tree_black;                   //���������ڵ�Ϊ��
					x->parent->parent->color = __rb_tree_red;     //�����游�ڵ�Ϊ��
					x = x->parent->parent;                        //�����ڵ㣬���ϼ���������ɫ����
				}
				else { //�޲����ڵ� ,�򲮸��ڵ�Ϊ��ɫ                         
					if (x == x->parent->right) { //�������������½ڵ�Ϊ���ڵ�����ӽڵ㣬�Ҹ��ڵ����游�ڵ�����ӽڵ㣨����˫����
						x = x->parent;
						__single_rotate_left(x, root);//�Բ����ĸ��ڵ�Ϊ����ת���������
					}
					//������������½ڵ�Ϊ���ڵ�����ӽڵ㣬�Ҹ��ڵ����游�ڵ�����ӽڵ㣨���е���ת��
					//��ɫ����
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__single_rotate_right(x->parent->parent, root);
				}
			}
			else {        //���ڵ�Ϊ�游�ڵ�����ӽڵ�
				__rb_tree_node_base* y = x->parent->parent->left;//yΪ�����ڵ�
				if (y && y->color == __rb_tree_red) {            //������ģ��в����ڵ㣬��Ϊ�죬�Ҹ��ڵ�Ϊ�游�ڵ�����ӽڵ㣨����˫����
					x->parent->color = __rb_tree_black;          //���ĸ��ڵ���ɫΪ��
					y->color = __rb_tree_black;                  //���Ĳ����ڵ�Ϊ��
					x->parent->parent->color = __rb_tree_red;    //�����游�ڵ�Ϊ��
					x = x->parent->parent;                       //���ϼ������е���
				}
				else {//       �޲����ڵ㣬���򲮸��ڵ�Ϊ��
					if (x == x->parent->left) {   //������壩����½ڵ�Ϊ���ڵ�����ӽڵ㣬�Ҹ��ڵ�Ϊ�游�ڵ�����ӽڵ㣨��ʱ����˫����
						x = x->parent;
						__single_rotate_right(x, root);//�Բ����ĸ��ڵ�Ϊ�������������
					}
					//�������������½ڵ�Ϊ���ڵ�����ӽڵ㣨ִ�е���ת��
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					__single_rotate_left(x->parent->parent, root);//����
				}
			}
		}// end of while
		root->color = __rb_tree_black;//���ڵ���ԶΪ��
	}
}





#endif // !_RB_TREE_DETAIL_H