#pragma once

#include<iostream>
#include"MinHeap.h"
#include<queue>
#include<assert.h>
using namespace std;

//�ڵ���
template<class T>
struct HuffmanTreeNode
{
	T _weight;
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;

	HuffmanTreeNode(T weight=0)
		:_left(NULL)
		, _right(NULL)
		, _weight(weight)
	{}
};
//HuffmanTree
template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}
	HuffmanTree(const T*a, size_t size, const T&invalid)
	{
		_root=CreateHuffmanTree(a, size, invalid);
	}
	//����Huffman��
	Node* CreateHuffmanTree(const T* a, size_t size, const T&invalid);
	//�����ӡHuffman��
	void LevelOrder();
	//��ȡroot��
	Node* GetTop();
protected:
	Node* _root;
}; 

//����Huffman��
template<class T>
HuffmanTreeNode<T>* HuffmanTree<T>::CreateHuffmanTree(const T* a, size_t size, const T&invalid)
{
	assert(a);
	MinHeap<Node*> mh;
	//1.��ʼ����С��
	for (size_t i = 0; i<size; i++)
	{
		if (a[i] != invalid)
		{
			mh.Push(new Node(a[i]));
		}
	}
	// 2. ѡ����С�ʹ˴�С�������ڵ㣬���츸�ڵ㣬��������
	Node* parent = NULL;
	while (mh._array.size()>1)
	{
		Node* left = mh.GetHeapTop();
		mh.Pop();
		Node* right = mh.GetHeapTop();
		mh.Pop();
		//�������ڵ�
		parent = new Node(left->_weight + right->_weight);
		parent->_left = left;
		parent->_right = right;
		mh.Push(parent);
	}
	return parent;
}
//�����ӡHuffman��
template<class T>
void HuffmanTree<T>::LevelOrder()
{
	queue<Node*>qe;
	if (_root)
	{
		qe.push(_root);
	}
	while (!qe.empty())
	{
		Node* qu = qe.front();
		cout << qu->_weight << " ";
		qe.pop();
		if (qu->_left)
		{
			qe.push(qu->_left);
		}
		if (qu->_right)
		{
			qe.push(qu->_right);
		}
	}
}
template<class T>
HuffmanTreeNode<T>* HuffmanTree<T>::GetTop()
{
	return _root;
}

//void TestHuffmanTree()
//{
//	int array[] = { 9,8,7,6,5,4,3,2,1,0 };
//	HuffmanTree<int> ht(array, sizeof(array) / sizeof(int), -1);
//	ht.LevelOrder();
//}