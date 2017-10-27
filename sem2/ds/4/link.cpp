#include "link.h"
#include <stdio.h>

using namespace std;

int z = 0;

link::link(int id_space){
	head = NULL;
	current = NULL;
	temp = NULL;

	IDSpace = id_space;
	max_value = pow(2, IDSpace);
}

void link::generateTable(){
	int q = 0;
	for(int i = 0; i < n.size(); i++){
		q++;
	}
	for(int i = 0; i < n.size(); i++){
		std::vector< pair <int, int> > temp;
		for(int j = 0; j < IDSpace; j++){
			float value = (n[i] + pow(2, j));
			value = fmod(value, pow(2, IDSpace));
			for(int x = 0; x < n.size(); x++){
				if(value == n[x]){
					value = n[x];
					break;
				}
				else if(value > n[x] && value < n[x+1]){
					value = n[x+1];
					break;
				}
				else if(value < n[x]){
					value = n[x];
					break;
				}
				else if(value > n[q-1]){
					value = n[0];
					break;
				}
			}
			temp.push_back(make_pair(j+1, value));
		}
		v.push_back(temp);
	}
}

void link::insert(int data){
	node* n = new node();
	n->data = data;
	n->next = NULL;

	if(head != NULL){
		current = head;
		while(current->next != NULL){
			current = current->next;
		}
		current->next = n;
	}
	else{
		head = n;		
	}
}

void link::printFingerTable(){
	cout << endl << endl;
	for(int k = 0; k < n.size(); k++){
		cout << "Node " << n[k] << " :" << endl;
		for(int i = k; i < k + 1; i++){
			for(int j = 0; j < v[i].size(); j++){
				cout << v[i][j].first << "->" << v[i][j].second << "  ";	
			}
			cout << endl;
		}
	}
}

void link::printFingerTableNode(int nodeNum){
	printFingerTableNode(head, nodeNum);
}

void link::printFingerTableNode(node* head, int nodeNum){
	node* current = head;
	while(current != NULL){
		if(current->data == nodeNum){
			break;
		}
		current = current->next;
	}
	int t;
    for(int i = 0; i < n.size(); i++){
    	if(current->data == n[i])
    		t = i;
    }
    cout << "Node " << current->data << " : " << endl; 
    for(int j = 0; j < v[t].size(); j++)
    	cout << v[t][j].first << "->" << v[t][j].second << "  ";

}

void link::printList(){
	current = head;
	while(current != NULL){
		cout << current->data  << "->";
		current = current->next;
	}
	current = head;
	cout << current->data;
	cout << endl;

	printFingerTable();
	
	cout << endl;
}

void link::DeleteNode(int node_number){
	DeleteNode(node_number, head);
}

void link::DeleteNode(int node_number, node *head)
{
	node* temp;
	node* current = head;
	node* del;
	if(head == NULL){
		cout << "\n\nError deleting node.... List empty\n\n";
	}
	else{
		if(head->data == node_number){
			temp = head;
			head = head->next;
			delete temp;
		}
		else{
			while(current != NULL && current->data != node_number){
				temp = current;
				current = current->next;
			}
			if(current->data == node_number){
				del = current;
				current = current->next; 
				temp->next = current;
				delete del;
			}
			else{
				cout << "\nElement not in list..\n";
			}
		}
	}
	n.clear();
	v.clear();
}


void link::sortList(){
	sortList(head);
}

void link::sortList(node* head){
	node* current = head;
    node* next;
    int temp;

    while (current && current->next)
    {
        node *next = current->next;
        while (next)
        {
            if (current->data > next->data)
            {
                std::swap(next->data, current->data);
            }
            next = next->next;
        }
        current = current->next;
    }
    current = head;
    while(current != NULL){
    	n.push_back(current->data);
    	current = current->next;
    }
    generateTable();	
}

void link::lookup(int key, int number){
	lookup(head, key, number);
}

void link::lookup(node* head, int key, int number){
	node* current = head;
	while(current != NULL){
		if(current->data == number)
			break;
    	current = current->next;
    }
    int t;
    int q;
    int array[IDSpace];
   	//cout << endl << endl << current->data << endl;
    for(int i = 0; i < n.size(); i++){
    	if(current->data == n[i])
    		t = i;
    }

    if(current->data < key && key < v[t][0].second){		//CASE 1
			number = v[t][0].second;
			cout << "Jump to node : " << number << endl;
			z = number;
			lookup(head, key, number);
	}
	else{
		for(int j = 0; j < v[t].size(); j++){
			//cout << v[t][j].second << "  ";
			if(v[t][j].second <= key && key < v[t][j+1].second){				//CASE 2
				//cout << v[t][j].first << "->" << v[t][j].second << "  ";
				number = v[t][j].second;
				z = number;
				cout << "Jump to node : " << number << endl;
				lookup(head, key, number);	
			}
		}
		if(z == 0){
			int i = 0;
			for(int k = 0; k < n.size(); k++){
				//cout << v[i][k].first << "->" << v[i][k].second << "  ";
				array[k] = v[i][k].second;
			}
			for(int k = 0; k < n.size(); k++){
				if(array[0] < array[k])
					array[0] = array[k];
			}
			for(int i = 0; i < n.size(); i++){
    			if(array[0] == n[i])
    				q = i;
    		}
    		for(int k = 0; k < n.size(); k++){
				//cout << v[i][k].first << "->" << v[i][k].second << "  ";
				array[k] = v[q][k].second;
				cout  << array[k] << "  ";
			}
			for(int k = 0; k < n.size() - 1; k++){
				if(key == array[k]){
					cout << "Jump to node : " << array[k] << endl;
					lookup(head, key, number);
				}
				else if(key > array[k] && key < array[k+1])
					number = array[k];
					cout << "Jump to node : " << number << endl;
					lookup(head, key, number);
			}
		}
	}
}