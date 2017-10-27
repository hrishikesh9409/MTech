#pragma once

#include "node.h"
#include <algorithm>
#include <vector>
#include <utility>
#include <math.h>

class link{
private:
	node* head;
	node* current;
	node* temp;

	int IDSpace;
	long int max_value;

	std::vector<std::vector< std::pair < int, int > > > v;
	std::vector<int> n;
	std::vector<int> userList;

	void sortList(node* head);
	void generateTable(int data);
	void generateTable();
	void printFingerTable();
	void printFingerTableNode(node* head, int nodeNum);
	void DeleteNode(int node_number, node* head);
	void lookup(node* head, int key, int number);
	bool errorCheck(node* head, int num);

public:
	link(int max_value);
	void insert(int data);
	void printList();
	void printFingerTableNode(int nodeNum);
	void sortList();
	void DeleteNode(int node_number);
	void lookup(int key, int number);
	void userData(int fileNum);
	bool errorCheck(int num);
};