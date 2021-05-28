#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

/*
* ---------------------------------
*         Red-Black Tree
* ---------------------------------
*/

enum Colour
{
	RED,
	BLACK
};

// Node data structure
struct Node
{
	Colour colour;
	int value; // Node value
	Node *parent;
	Node *left, *right; // Children nodes
};

typedef Node *NodePtr;

class RBTree
{
private:
	NodePtr root; // The root node of the tree
	NodePtr TNULL;

	void initializeNULLNode(NodePtr node, NodePtr parent)
	{
		node->value = 0;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->colour = BLACK;
	}

	void preOrderHelper(NodePtr node)
	{
		if (node != TNULL)
		{
			cout << node->value << " ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(NodePtr node)
	{
		if (node != TNULL)
		{
			inOrderHelper(node->left);
			cout << node->value << " ";
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(NodePtr node)
	{
		if (node != TNULL)
		{
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout << node->value << " ";
		}
	}

	NodePtr searchTreeHelper(NodePtr node, int key)
	{
		if (node == TNULL || key == node->value)
		{
			return node;
		}

		if (key < node->value)
		{
			return searchTreeHelper(node->left, key);
		}

		return searchTreeHelper(node->right, key);
	}

	void deleteFixup(NodePtr x)
	{
		NodePtr s;
		while (x != root && x->colour == BLACK)
		{
			if (x == x->parent->left)
			{
				s = x->parent->right;
				if (s->colour == RED)
				{
					s->colour = BLACK;
					x->parent->colour = RED;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->colour == BLACK && s->right->colour == BLACK)
				{
					s->colour = RED;
					x = x->parent;
				}
				else
				{
					if (s->right->colour == BLACK)
					{
						s->left->colour = BLACK;
						s->colour = RED;
						rightRotate(s);
						s = x->parent->right;
					}

					s->colour = x->parent->colour;
					x->parent->colour = BLACK;
					s->right->colour = BLACK;
					leftRotate(x->parent);
					x = root;
				}
			}
			else
			{
				s = x->parent->left;
				if (s->colour == RED)
				{
					s->colour = BLACK;
					x->parent->colour = RED;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->colour == BLACK && s->right->colour == BLACK)
				{
					s->colour = RED;
					x = x->parent;
				}
				else
				{
					if (s->left->colour == BLACK)
					{
						s->right->colour = BLACK;
						s->colour = RED;
						leftRotate(s);
						s = x->parent->left;
					}
					s->colour = x->parent->colour;
					x->parent->colour = BLACK;
					s->left->colour = BLACK;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->colour = BLACK;
	}

	void rbTransplant(NodePtr u, NodePtr v)
	{
		if (u->parent == nullptr)
		{
			root = v;
		}
		else if (u == u->parent->left)
		{
			u->parent->left = v;
		}
		else
		{
			u->parent->right = v;
		}

		v->parent = u->parent;
	}

	void deleteNodeHelper(NodePtr node, int key)
	{
		NodePtr z = TNULL;
		NodePtr x, y;
		while (node != TNULL)
		{
			if (node->value == key)
			{
				z = node;
			}

			if (node->value <= key)
			{
				node = node->right;
			}
			else
			{
				node = node->left;
			}
		}

		if (z == TNULL)
		{
			cout << "Couldn't find key in the tree" << endl;
			return;
		}

		y = z;
		int y_original_colour = y->colour;
		if (z->left == TNULL)
		{
			x = z->right;
			rbTransplant(z, z->right);
		}
		else if (z->right == TNULL)
		{
			x = z->left;
			rbTransplant(z, z->left);
		}
		else
		{
			y = minimum(z->right);
			y_original_colour = y->colour;
			x = y->right;
			if (y->parent == z)
			{
				x->parent = y;
			}
			else
			{
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->colour = z->colour;
		}
		delete z;
		if (y_original_colour == BLACK)
		{
			deleteFixup(x);
		}
	}

	// fix the red-black tree
	void insertFixup(NodePtr k)
	{
		NodePtr u;
		while (k->parent->colour == RED)
		{
			if (k->parent == k->parent->parent->right)
			{
				u = k->parent->parent->left; // uncle
				if (u->colour == RED)
				{
					u->colour = BLACK;
					k->parent->colour = BLACK;
					k->parent->parent->colour = RED;
					k = k->parent->parent;
				}
				else
				{
					if (k == k->parent->left)
					{
						k = k->parent;
						rightRotate(k);
					}
					k->parent->colour = BLACK;
					k->parent->parent->colour = RED;
					leftRotate(k->parent->parent);
				}
			}
			else
			{
				u = k->parent->parent->right; // uncle

				if (u->colour == RED)
				{
					u->colour = BLACK;
					k->parent->colour = BLACK;
					k->parent->parent->colour = RED;
					k = k->parent->parent;
				}
				else
				{
					if (k == k->parent->right)
					{
						k = k->parent;
						leftRotate(k);
					}
					k->parent->colour = BLACK;
					k->parent->parent->colour = RED;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root)
			{
				break;
			}
		}
		root->colour = BLACK;
	}

public:
	RBTree()
	{
		TNULL = new Node;
		TNULL->colour = BLACK;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}

	void preorder()
	{
		preOrderHelper(this->root);
	}

	void inorder()
	{
		inOrderHelper(this->root);
	}

	void postorder()
	{
		postOrderHelper(this->root);
	}

	NodePtr searchTree(int k)
	{
		return searchTreeHelper(this->root, k);
	}

	NodePtr minimum(NodePtr node)
	{
		while (node->left != TNULL)
		{
			node = node->left;
		}
		return node;
	}

	NodePtr maximum(NodePtr node)
	{
		while (node->right != TNULL)
		{
			node = node->right;
		}
		return node;
	}

	NodePtr successor(NodePtr x)
	{
		if (x->right != TNULL)
		{
			return minimum(x->right);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->right)
		{
			x = y;
			y = y->parent;
		}
		return y;
	}

	NodePtr predecessor(NodePtr x)
	{
		if (x->left != TNULL)
		{
			return maximum(x->left);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->left)
		{
			x = y;
			y = y->parent;
		}

		return y;
	}

	// rotate left bout Node x
	void leftRotate(NodePtr x)
	{
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL)
		{
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr)
		{
			this->root = y;
		}
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	void rightRotate(NodePtr x)
	{
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL)
		{
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr)
		{
			this->root = y;
		}
		else if (x == x->parent->right)
		{
			x->parent->right = y;
		}
		else
		{
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	void insert(int key)
	{
		// Ordinary Binary Search Insertion
		NodePtr node = new Node;
		node->parent = nullptr;
		node->value = key;
		node->left = TNULL;
		node->right = TNULL;
		node->colour = RED; // new node is red

		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL)
		{
			y = x;
			if (node->value < x->value)
			{
				x = x->left;
			}
			else
			{
				x = x->right;
			}
		}

		node->parent = y;
		if (y == nullptr)
		{
			root = node;
		}
		else if (node->value < y->value)
		{
			y->left = node;
		}
		else
		{
			y->right = node;
		}

		if (node->parent == nullptr)
		{
			node->colour = BLACK;
			return;
		}

		if (node->parent->parent == nullptr)
		{
			return;
		}

		// Fix the tree
		insertFixup(node);
	}

	NodePtr getRoot()
	{
		return this->root;
	}

	// delete the node from the tree
	void deleteNode(int value)
	{
		deleteNodeHelper(this->root, value);
	}

	// // TODO: initialise empty tree

	// void rBInsert(int value) {
	//     // Insert function
	// }

	// void rBDelete(int value) {
	//     // delete function
	// }

	// void rotateLeft(Node x) {
	//     /*
	//     * Rotate the node so it becomes the child of its
	//     * right branch
	//     */
	// }

	// void rotateRight(Node x) {
	//     /*
	//     * Rotate the node so it becomes the child of its
	//     * left branch
	//     */
	// }

	// Node rBFind(int value) {
	//     /*
	//     *find the value we are looking for, or null if it
	//     * does not exist
	//     */
	//     return Node();
	// }

	// Will include other capabilities as necessary
};

/*
* ---------------------------------
*         Hash Table
* ---------------------------------
*/

class HashTable
{
	int bucketCount; // Number of buckets

	// TODO: Array containing buckets

public:
	void hashInsert(int item){

	};

	void hashDelete(int value){

	};

	void hashSearch(int key){

	};

	// Will include other capabilities as necessary
};

/*
* ---------------------------------
*         Dynamic Programming
* ---------------------------------
*/

void help()
{
	/* 
    * Print out functions the user has access to
    * with brief descriptions
    */

	cout << "Commands..." << endl;
	cout << "\n";
	cout << "help: Print this list of commands"
		 << " and descriptions" << endl;
	cout << "\n";
	cout << "load FILENAME: Load spreadsheet from local file"
		 << " FILENAME.txt" << endl;
	cout << "\n";
	cout << "save FILENAME: Save current spreadsheet as"
		 << " FILENAME.txt (program terminates)" << endl;
	cout << "\n";
	cout << "create m n: create a spreadsheet with m rows and"
		 << " n columns (initialised with values of 0)" << endl;
	cout << "\n";
	cout << "print: Print out spreadsheet" << endl;
	cout << "\n";
	cout << "set m n value: Set the item located at row m,"
		 << " column n to be value" 
		 << endl;
	cout << "\n";
	cout << "search: Search in spreadsheet" << endl;
	cout << "\n";
	cout << "sort: " << endl;
	cout << "\n";
	cout << "close: Terminate program (you"
		 << " will lose all unsaved data)" << endl;
	cout << "\n";
};

class Spreadsheet
{
private:
	// int *testArray;
	// int testArraySize;

	int **currentSpreadsheet;
	int rows;	 //m
	int columns; //n

	// int** spreadsheetArray = new int*[5];
	// for (int i = 0; i < 5; i++) {
	//     spreadsheetArray[i] = new int[5];
	// }

public:
	/* 
    * The user will be able to access these functions
    * in the terminal
    */

	void setArraySize(int m, int n)
	{
		currentSpreadsheet = new int *[m];
		for (int i = 0; i < m; i++)
		{
			currentSpreadsheet[i] = new int[n];
		}

		rows = m;
		columns = n;
		// testArray = new int[s];
		// testArraySize = s;
		// cout << "Size of array is: " << testArraySize << endl;
	}

	//    int spreadsheet[250000][2500]; // rows x columns (m x n)

	// Save spreadsheet to file
	bool save(string filename) // Returns true if saved successfully
	{
		string output = print();
		if (output.empty())
		{
			cout << "There was nothing to save..." << endl;
			return false;
		}

		string file = filename + ".txt";
		ofstream out(file);
		out << output;
		out.close(); // not strictly neccesary as the
			// deconstructor of ofstream can
			// handle this for us as soon as
			// out goes out of scope.

		cout << "Your spreadsheet has been saved as "
			 << file << endl;
		return true;
	};

	// Load from file a spreadsheet into the program
	bool load(string filename)
	{

		string file = filename + ".txt";

		ifstream in(file);

		if (in.is_open())
		{
			int rowCount = 0;
			int columnCount = 0;
			string line;

			while (std::getline(in, line))
			{
				rowCount++;
				if (rowCount == 1)
				{
					size_t n = std::count(line.begin(), line.end(), ',');
					columnCount = n + 1;
				}
			}

			setArraySize(rowCount, columnCount);

			int currentRow = 0;		  // Initialised at 0
			int currentColumn = 0;
			string currentDigit = ""; // initialised empty

			in.close();
			ifstream in(file); // resetting

			while (in.good())
			{
				while (std::getline(in, line) && currentRow <= rowCount)
				{
					currentColumn = 0;
					currentDigit = "";
					for (char & c : line)
					{
						if (c == ',')
						{
							currentSpreadsheet[currentRow][currentColumn] 
								= stoi(currentDigit);
							currentColumn += 1;
							currentDigit = "";
							continue;
						}
						currentDigit += c;
					}
					currentSpreadsheet[currentRow][currentColumn] = stoi(currentDigit);

					currentRow++;
				}
			}

			cout << "You have loaded a spreadsheet with " 
				 << rows << " rows and " << columns 
				 << " columns!" << endl;
			return true;
		}
		else
		{
			cout << "Unable to open file" << endl;
			return false;
		};
	};

	void create(int m, int n)
	{
		// Create m x n array initialised with 0s

		setArraySize(m, n);

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				currentSpreadsheet[i][j] = 0;
			}
		}

		cout << "You have created a spreadsheet with " << m
			 << " rows and " << n << " columns!" << endl;
	};

	string print()
	{
		/* 
                * Display spreadsheet in text
                * Note: Will only work up to a certain size of
                * spreadsheet
                */

		string output;

		auto start_time = high_resolution_clock::now(); // Start time

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				if (j == (columns - 1))
				{
					if (i == (rows - 1))
					{
						output += to_string(currentSpreadsheet[i][j]);
						break;
					}
					output += to_string(currentSpreadsheet[i][j]);
					output += "\n";
					break;
				}
				output += to_string(currentSpreadsheet[i][j]) + ",";
			}
		}

		auto end = high_resolution_clock::now(); // End time
    	auto duration = duration_cast<milliseconds>(end - start_time);
		cout << "\n";
    	cout << "(The time taken for this operation was " << duration.count() 
			 << "ms)" << endl;
		cout << "\n";

		return output;
	};

	void set(int m, int n, int value) {
		currentSpreadsheet[m][n] = value;
	}

	// @param: given range of cells
	void search(){
		// Search elements within a given range of cells
	};

	// @param: given range of cells
	void sort(){
		// Sort elements within a given range of cells
	};

	// TODO: Include basic arithmetic operations

	// Will include other capabilities as necessary
};

// General dynamic programming helper functions go here

/*
* ---------------------------------
*         Main 
* ---------------------------------
*/

int main()
{
	// Pipeline of program

	cout << "\n";
	cout << "Welcome to my simple spreadsheet program :)" << endl;
	cout << "\n";
	Spreadsheet spreadsheet;
	help();

	bool spreadsheetExists = false;
	bool saved = false;

	string input;
	do
	{
		cout << "\n";
		cout << "Type your command here..." << endl;

		getline(cin, input);
		cout << "You entered: " << input << endl;

		cout << "\n";
		if (input == "help")
		{
			help();
		}
		else if (input.rfind("load ", 0) == 0)
		{
			int count = std::count(input.begin(), input.end(), ' ');
			if (count != 1)
			{
				cout << "Sorry, your command was not understood,"
					 << " please type it again" << endl;
				continue;
			}

			string filename;
			filename = input.substr(5, input.length() - 1);

			spreadsheetExists = spreadsheet.load(filename);;
		}
		else if (input.rfind("save ", 0) == 0)
		{
			int count = std::count(input.begin(), input.end(), ' ');
			if (count != 1)
			{
				cout << "Sorry, your command was not understood,"
					 << " please type it again" << endl;
				continue;
			}

			string filename;
			filename = input.substr(5, input.length() - 1);

			saved = spreadsheet.save(filename);
		}
		else if ((input.rfind("create ", 0) == 0))
		{
			if (spreadsheetExists)
			{
				cout << "You already have a spreadsheet, please"
					 << " restart the program first by typing"
					 << " \"save FILENAME\" or \"close\"" << endl;
				continue;
			}

			int count = std::count(input.begin(), input.end(), ' ');
			if (count != 2)
			{
				cout << "Sorry, your command was not understood,"
					 << " please type it again" << endl;
				continue;
			}

			string firstInt;
			string secondInt;

			input = input.substr(7, input.length() - 1);

			size_t pos = input.find(" ");
			firstInt = input.substr(0, pos);
			secondInt = input.substr(pos + 1, input.length() - 1);

			int firstIntAsInt = stoi(firstInt);
			int secondIntAsInt = stoi(secondInt);

			if (firstIntAsInt > 250000 && secondIntAsInt > 2500) {
				cout << "Too many rows and too many columns" << endl;
				continue;
			}
			if (firstIntAsInt > 250000) {
				cout << "Too many rows" << endl;
				continue;
			}
			if (secondIntAsInt > 2500) {
				cout << "Too many columns" << endl;
				continue;
			}

			spreadsheet.create(firstIntAsInt, secondIntAsInt);
			spreadsheetExists = true;
		}
		else if (input == "print")
		{
			if (!spreadsheetExists)
			{
				cout << "There is nothing to print" << endl;
			}
			else
			{
				cout << "Here is your current spreadsheet:" << endl;
				cout << spreadsheet.print() << endl;
			}
		}
		else if (input.rfind("set ", 0) == 0) {
			if (!spreadsheetExists) {
				cout << "There is no spreadsheet currently" 
					 << " selected" << endl;
				continue;
			}

			int count = std::count(input.begin(), input.end(), ' ');
			if (count != 3)
			{
				cout << "Sorry, your command was not understood,"
					 << " please type it again" << endl;
				continue;
			}

			int pos1 = input.find(" ");
			int pos2 = input.find(" ", pos1 + 1);
			int pos3 = input.find(" ", pos2 + 1);

			string firstInt = input.substr(pos1 + 1, pos2 - pos1 - 1);
			string secondInt = input.substr(pos2 + 1, pos3 - pos2 - 1);
			string thirdInt = input.substr(pos3 + 1, input.length() - 1);

			spreadsheet.set(stoi(firstInt) - 1, stoi(secondInt) - 1, stoi(thirdInt));

			cout << "Row " << firstInt << ", column " << secondInt
				 << " has been set to " << thirdInt << endl;
		}
		else
		{
			cout << "Sorry, your command was not understood,"
				 << " please type it again" << endl;
		}

	} while (!(saved || input == "close"));

	cout << "\n";
	cout << "Program closing.." << endl;
	cout << "Thanks for using my spreadsheet program!" << endl;
	return 0;
}