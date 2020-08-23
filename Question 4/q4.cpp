#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Node {
public:
	int element;
	Node* next;
	Node* prev;
};

int main(int argc, char** argv) {
	ifstream testfile;
	testfile.open(argv[1]); //read the inputs from test file

	int capacity; //capacity is the maximum number of elements in the cache, defined as k in pdf
	int element_number; //number of different elements that can possibly requested, equals to k+1 in our case
	int request_number; //number of requests, defined as n in pdf

	testfile >> capacity;
	element_number = capacity + 1;

	int cache_size = 0; //cache size keeps number of elements in the cache, initially zero
	//vector<int> cache(element_number, -1); //cache is created as empty as all indexes are 0
	vector<int> cache(capacity, -1); //cache is created as empty as all indexes are 0

	testfile >> request_number;
	vector<int> requests(request_number);
	for (int i = 0; i < request_number; i++)
		testfile >> requests[i];

	vector<int> fif(requests.size()); //to keep the farthest element for each request

	vector<int> elements(element_number); //elements array {0, 1, ... , k}
	for (int i = 0; i < element_number; i++)
		elements[i] = i;

	vector<Node*> addresses; //array for keeping the address of double linked list nodes of elements
	//addresses[i] keeps the node address for elements[i]

	/* IMPLEMENT HERE */
		// 1. create the double linked list for elements vector, you can use Node class defined above
		// keep each node's address in addresses array

	for (int i = 0; i < element_number; i++){
		Node* newnode = new Node;
		newnode->element = elements[i];
		if (i == 0) newnode->prev = NULL;	//newnode is the first node so there is no prev node
		else{
			newnode->prev = addresses[i-1];	//previous node's address
			addresses[i-1]->next = newnode;	//previous node's next is newnode
		}
		if (i == element_number-1) newnode->next = NULL; //newnode is the last node so there is no next node

		addresses.push_back(newnode);
	}

	Node* head = addresses[0]; //initialize head
	addresses[0]->prev = addresses[element_number-1]; //in order to access last element make the link list circular
	addresses[element_number-1]->next = addresses[0]; //in order to access last element make the link list circular

	// 2. fill the fif (farthest-in-future) array considering the pseudocode given in pdf.
	for (int i = request_number-1; i >= 0; i--){
		Node* nodeptr = addresses[requests[i]];
		//remove nodeptr
		nodeptr->prev->next = nodeptr->next;
		nodeptr->next->prev = nodeptr->prev;
		//insert nodeptr between last element and head
		nodeptr->prev = head->prev;
		nodeptr->prev->next = nodeptr;
		nodeptr->next = head;
		head->prev = nodeptr;

		head = nodeptr; //make nodeptr the new head
		fif[i] = head->prev->element;
	}
		// 3. process the requests and print out necessary outputs, you can use following syntax:
	for(int i = 0; i < request_number; i++){
		bool inCache = false;
		for (int j = 0; j < capacity; j++){
		   if (cache[j] == requests[i]) inCache = true;
		 }
		if (!inCache){
			cout << "cache miss" << endl;
			if (cache_size > capacity-1) {
				cout << "cache is full, element " << fif[i] << " is evicted" << endl;
				for (int j = 0; j < capacity; j++){
					if (cache[j] == fif[i]){
						cache[j] = requests[i];
						cout << "element " << requests[i] << " is added into the cache" << endl;
					}
				}
			} else{
				cout << "element " << requests[i] << " is added into the cache" << endl;
				cache[cache_size++] = requests[i];
			}
		} else cout << "cache hit" << endl;
	}

	/*IMPLEMENT HERE*/

	return 0;


}
