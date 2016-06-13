#define _CRT_SECURE_NO_WARNINGS 1
#pragma once

#include<iostream>
#include"FileCompress.hpp"
#include<assert.h>
#include <Windows.h>
using namespace std;

void TestCompress()
{
	FileCompress fc1;
	int begin = GetTickCount();
	fc1.Compress("text.txt");
	int end = GetTickCount();
	cout << "Compress : " << end - begin << endl;
}
void TestUnCompress()
{
	FileCompress fc2;
	int begin = GetTickCount();
	fc2.UnCompress("text.txt");
	int end = GetTickCount();
	cout << "UnCompress : " << end - begin << endl;
}
void Test()
{
	//TestHuffmanTree();
	TestCompress();
	cout << "**************************************************" << endl << endl;
	TestUnCompress();
}

int main()
{
	Test();
	system("pause");
	return 0;
}