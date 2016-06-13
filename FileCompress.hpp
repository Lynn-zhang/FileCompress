#define _CRT_SECURE_NO_WARNINGS 1
#pragma once

#include<iostream>
#include<string>
#include<assert.h>
#include"Huffman.hpp"
using namespace std;

//������Ч�ַ���Ϣ
typedef unsigned long long LongType;
struct CharInfo
{
	unsigned char _ch;   // �ַ�
	LongType _count;    // ���ִ���
	string _code;			 // Huffman code

	CharInfo(LongType count = 0)
		:_ch(0)
		, _count(count)
	{}
	bool operator!=(const CharInfo& info)		const
	{
		return _count != info._count;
	}
	CharInfo operator + (const CharInfo& info) 
	{
		return _count + info._count;
	}
	bool operator<(const CharInfo& info)
	{
		return _count < info._count;
	}
};
//���������롢�ļ�ѹ������ѹ��
class FileCompress
{
public:
	//���캯������ʼ���ַ�����
	FileCompress()
	{
		for (int i= 0; i < 256; i++)
		{
			_info[i]._ch = i;
		}
	}	
	//ѹ��
	void Compress(const char* fileName)
	{
		assert(fileName);
		// 1. ��ȡ�ļ���ͳ��ÿ���ַ����ֵĴ���
		FILE *fOut = fopen(fileName, "rb"); //�Զ����������ļ�
		assert(fOut);
		cout <<endl<< "	�ļ��򿪳ɹ�����ʼͳ���ַ���Ϣ ..." << endl;
		char ch = fgetc(fOut);
		int count = 0;  //��¼�ļ����ַ��ܸ���
		while (ch != EOF)
		{
			count++;
			_info[(unsigned char)ch]._count++;
			ch = fgetc(fOut);			
		}
		printf("	(������%d���ַ�)\n\n",count);
		// 2. �����ַ����ֵĴ�������Huffman����������ÿ���ַ���Ӧ��Huffman����
		cout << "	��������Huffman���� ..." << endl;
		CharInfo invalid(0);
		HuffmanTree<CharInfo> tree(_info, 256, invalid);
		string code;
		GenerateHuffmanCode(tree.GetTop(), code);

		// 3. ��Huffman����д��ѹ���ļ�
		cout << "	���ڽ�ѹ���ַ�д��ѹ���ļ� ..." << endl;
		string CompressFileName = fileName;
		CompressFileName += ".compress";
		FILE* finCompress = fopen(CompressFileName.c_str(), "wb");
		assert(finCompress);
		//�ض�λ�ļ�ָ�룬���ļ���ʼ����ȡ
		fseek(fOut, 0, SEEK_SET);
		ch = fgetc(fOut);
		unsigned char value = 0;
		int pos = 0;
		while (ch != EOF)
		{
				for (size_t i = 0; i <_info[(unsigned char)ch]._code.size(); i++)
				{
						value <<= 1;
						if (_info[(unsigned char)ch]._code[i] == '1')
							value |= 1;
		
						if (++pos==8)
						{
							// Huffman���������㹻8λ����ѹ���ַ�д���ļ������pos��value,������������
							fputc(value, finCompress);
							pos = 0;
							value = 0;
					}

				}
				ch = fgetc(fOut);
			}
			if (pos!=0)
			{	//����8λ�򽫵�λ������λ			
				value = value << (8 - pos);
				fputc(value, finCompress);
			}

		// 4.���ַ�������Ϣд�������ļ�
		string configFileName = fileName;
		configFileName += ".config";
		FILE* finConfig = fopen(configFileName.c_str(), "wb");
		assert(finConfig);
	
		//д���ļ��ַ�����
		char strCount[32];
		_itoa(count , strCount, 10);
		fputs("The total number of characters :	", finConfig);
		fputs(strCount, finConfig);
		fputc('\n', finConfig);

		for (int i = 0; i < 256; i++)
		{
			if (_info[i] != invalid)
			{
				fputc(_info[i]._ch, finConfig);	
				fputc(',', finConfig);
				_itoa((int)_info[i]._count, strCount, 10);
				fputs(strCount, finConfig);
				fputc('\n', finConfig);
			}
		}
		fclose(fOut);
		fclose(finCompress);
		fclose(finConfig);
		cout << endl << "	ѹ���ɹ� !" << endl << endl;
		cout << "**************************************************" << endl<< endl;
	}
	//��ѹ��
	void UnCompress(const char* fileName)
	{
		// 1. ��ȡ�����ļ���Ϣ����ʼ��_info[]
		cout << endl<<"	���ڶ�ȡ�����ļ���Ϣ ..." << endl;
		string configFileName = fileName;
		configFileName += ".config";
		FILE* foutConfig = fopen(configFileName.c_str(), "rb");
		string line;
		unsigned char ch ;
		GetLine(foutConfig, line);
		line.clear();
	
		while (GetLine(foutConfig, line))
		{
			if (!line.empty())
			{
				ch = line[0];
				_info[ch]._count = atoi(line.substr(2).c_str()); //	,��������Ϊ���ַ���������Ҫ���ַ���ʽת��Ϊ����
				line.clear();
			}
			else	//�����ߵ�����˵���������ַ�Ϊ \r �� \n
				line = '\n';
		}
	
		// 2.���ݶ�����������Ϣ���ؽ�Huffman��
		cout << "	�����ؽ�Huffman�� ..." << endl;
		CharInfo invalid(0);
		HuffmanTree<CharInfo> tree(_info,256,invalid);

		// 3.��ȡѹ���ļ���Ϣ����ѹ��
		cout << "	���ڽ�ѹ ..." << endl;
		string uncompressFileName = fileName;
		uncompressFileName += ".uncompress";
		FILE* fin = fopen(uncompressFileName.c_str(), "wb");
		string compressFileName = fileName;
		compressFileName += ".compress";
		FILE* fout = fopen(compressFileName.c_str(), "rb");
		assert(fin);
		assert(fout);

		HuffmanTreeNode<CharInfo>* root = tree.GetTop();
		HuffmanTreeNode<CharInfo>* cur = root;
		ch = fgetc(fout);
		int pos = 8;
		LongType charCount = root->_weight._count;  //�ַ�����
		while (charCount > 0)
		{
			//��Ҷ�ӽڵ�
			while (cur!=NULL && cur->_left!=NULL && cur->_right!=NULL)
			{
				if (pos == 0)
				{
					pos = 8;
					ch = fgetc(fout);
				}
				--pos;

				//�õ�ch�е�posλ,����pos����HuffmanTree,��Ҷ�ӽڵ�
				if (ch & (1 << pos))
				{
					cur = cur->_right;
				}
				else
				{
					cur = cur->_left;
				}
			}
			//cur��Ҷ�ӽڵ�
			if (cur && charCount--)
			{
				fputc(cur->_weight._ch, fin);
				cur = root;
			}
		}
		
		fclose(fout);
		fclose(fin);
		fclose(foutConfig);
		cout <<endl<< "	��ѹ��� !" << endl<<endl;
		cout << "**************************************************" << endl << endl;

	}

protected:
	//Huffman����
	void GenerateHuffmanCode(HuffmanTreeNode<CharInfo>* root, string code)
	{
		assert(root);
		if (root->_left != NULL&&root->_right != NULL)
		{
			GenerateHuffmanCode(root->_left, code + '0');
			GenerateHuffmanCode(root->_right, code + '1');
		}
		if (root->_left == NULL&&root->_right == NULL)
		{	//��ʱ��rootΪҶ�ӽڵ㣬��Ҫ���б���Ľڵ�
			_info[root->_weight._ch]._code = code;
		}
	}
	// ��ȡ�ļ��е�һ��
	bool GetLine(FILE* fout, string &line)
	{
		char ch = fgetc(fout);
		if (ch == EOF)
			return false;

		while (ch!=EOF && ch != '\n')
		{
			line += ch;
			ch = fgetc(fout);
		}
		return true;
	}

protected:
	CharInfo _info[256];
};



