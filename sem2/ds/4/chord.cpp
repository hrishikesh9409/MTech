#include "link.h"

using namespace std;

int main(){

	int id_space;
	cout << "\nEnter the identifier space : ";
	cin >> id_space;
	long int max_value = pow(2, id_space);

	link l(id_space);

	cout << "Enter the number of nodes to be entered : ";
	int n;
	int node_value;
	cin >> n;
	cout << "\nEnter node values : \n";
	for(int i = 0; i < n; i++){
		cin >> node_value;
		if(node_value < max_value){
			l.insert(node_value);
		}
		else{
			cout << "\nNode value exceeds ID space, please enter again\n";
			i--;
		}
	}
	cout << "\nChoose one of the following options : \n";
	cout << "1. lookup a node using key and a start node \n";
	cout << "2. delete a node \n";
	cout << "3. lookup table of a particular node\n";
	cout << "4. Default view of the node and the table\n";
	cout << "5. Exit the program\n";
	cout << endl;
	l.sortList();
	while(true){
		cin >> n;
		if(n >= 1 && n <= 5){
			if(n == 1){
				int nodeNum;
				int key;
				cout << "\nEnter the key value : "; cin >> key;
				cout << "\nEnter the node to search from :"; cin >> nodeNum;

				l.lookup(key, nodeNum);

				//cout << "\nIf no results are seen then the given key and node fails the two chord conditions....\n";
			}
			if(n == 2){
				int del;
				cout << "\nEnter the node to deleted : "; cin >> del;
				l.DeleteNode(del);
			}
			if(n == 3){
				int nodeNum;
				cout << "\nEnter the node whose table is to be displayed : "; cin >> nodeNum;
				l.printFingerTableNode(nodeNum);
			}
			if(n == 4){
				l.printList();
			}
			if(n == 5){
				cout << "\ngoodbye!\n\n";
				exit(0);
			}
		}
		else{
			cout << "\nEnter values between 1 and 5 only\nPlease try again : \n";
		}
		cout << "\nEnter new operation to be performed : \n";
	}

	// l.sortList();
	// l.printList();
	// l.printFingerTableNode(3);

	// l.DeleteNode(2);
	// l.DeleteNode(3);
	// l.DeleteNode(34);
	// l.sortList();
	// l.printList();


	//int ans = l.lookup(16, 1);
	//cout << ans;

	cout << endl << endl;
	return 0;
}

