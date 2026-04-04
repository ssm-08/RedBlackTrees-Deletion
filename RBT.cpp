#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>

using namespace std;

// Tree node
struct Node {

  int value;
  bool red;
  Node* left;
  Node* right;
  Node* parent;
  
};

void print(Node* c, int depth);
Node* search(Node* c, int n);
void insertion(Node*& r, Node* c, Node* n);
void deletion(Node*& r, Node* n);

void insertFix(Node*& r, Node* c);
void deleteFix(Node*&, Node* c);

void leftRotate(Node*& r, Node* x);
void rightRotate(Node*& r, Node* x);

int main() {

  // Program variables
  Node* root = NULL;

  bool run = true;
  char input[99] = "";
  int num = 0;
  
  // const char* SEARCH = "SEARCH";
  const char* INS = "INSERT";
  const char* UPL = "UPLOAD";
  // const char* ROT = "ROTATE";
  const char* DEL = "DELETE";
  const char* PRINT = "PRINT";
  const char* QUIT = "QUIT";

  // Command loop
  while (run == true) {

    cout << "Enter command: ";
    cin >> input;
    cout << endl;

    if (strcmp(input, INS) == 0) { // Insert value
      cout << "Enter number: ";
      cin >> num;
      cout << endl;

      if (!search(root, num)) {
	Node* data = new Node{num, true, NULL, NULL, NULL};
	insertion(root, root, data);
      }
    } else if (strcmp(input, UPL) == 0) { // File read

      int i = 0;
      fstream data("data.txt");
      data.seekg(0, data.end);
      i = data.tellg();
      i++;
      data.seekg(0, data.beg);

      char numbers[i];
      data.getline(numbers, i);
      char* value = strtok(numbers, " ");

      while (value != NULL) {

	num = atoi(value);

	if (!search(root, num)) {
	  Node* data = new Node{num, true, NULL, NULL, NULL};
	  insertion(root, root, data);
	}

	value = strtok(NULL, " ");
      }
    } else if (strcmp(input, DEL) == 0) {

      cout << "Enter a number: ";
      cin >> num;
      cout << endl;

      if (search(root, num)) {
	deletion(root, search(root, num));
      }
      
    } else if (strcmp(input, PRINT) == 0) { // Print tree
      if (root) {
	print(root, 0);
      }
    } else if (strcmp(input, QUIT) == 0) { // Quit
      run = false;
    }
  }
}

void leftRotate(Node*& r, Node* x) {

  cout << "left beg" << endl;
  
  Node* y = x->right;

  if (y == NULL) {
    return;
  }
  
  // Handle sub-trees
  x->right = y->left;
  
  if (y->left) {
    y->left->parent = x;
  }

  // Handle node relationships
  y->parent = x->parent;

  if (x->parent == NULL) {
    r = y;
  } else if (x->parent->left == x) {
    x->parent->left = y;
  } else if (x->parent->right == x) {
    x->parent->right = y;
  }

  x->parent = y;
  y->left = x;

  cout << "left end" << endl;
}

void rightRotate(Node*& r, Node* x) {

  cout << "right beg" << endl;
  
  Node* y = x->left;

  if (y == NULL) {
    return;
  }
  
  // Handle sub-trees
  x->left = y->right;

  if (y->right) {
    y->right->parent = x;
  }

  // Handle node relationships
  y->parent = x->parent;

  if (x->parent == NULL) {
    r = y;
  } else if (x->parent->left == x) {
    x->parent->left = y;
  } else if (x->parent->right == x) {
    x->parent->right = y;
  }

  x->parent = y;
  y->right = x;

  cout << "right end" << endl;
}

void insertFix(Node*& r, Node* c) {

  Node* p = c->parent;
  
  if (p == NULL) { // Root
    c->red = false;
    return;
  }

  Node* gP = p->parent;

  if (p->red) {
    if (gP->left == p) { // Left side
      if (gP->right != NULL && gP->right->red) { // Red uncle

	cout << "L Red uncle" << endl;
	
	gP->red = true;
	gP->right->red = false;
	gP->left->red = false;

	insertFix(r, gP);
	
      } else if (p->right == c) { // Black uncle: corner case

	cout << "L C Black uncle" << endl;
	
	leftRotate(r, p);
	insertFix(r, p);
	
      } else { // Black uncle: slant case

	cout << "L S Black uncle" << endl;
	
	rightRotate(r, gP);
	p->red = false;
	gP->red = true;
	
      }
    } else { // Right side

      if (gP->left != NULL && gP->left->red) { // Red uncle

	cout << "R Red uncle" << endl;
	
        gP->red = true;
        gP->right->red = false;
        gP->left->red = false;

        insertFix(r, gP);

      } else if (p->left == c) { // Black uncle: corner case

	cout << "R C Black uncle" << endl;
	
        rightRotate(r, p);
        insertFix(r, p);

      } else { // Black uncle: slant case

	cout << "R S Black uncle" << endl;
	
        leftRotate(r, gP);
        p->red = false;
        gP->red = true;

      }
    }
  }
}

void deleteFix(Node*& r, Node* n) {

  // 1. Sibling red
  
  // 2. Sibling black

  // 2a. Close nephew red
  // 2b. Distant nephew red
  // 2c. Red parent
  // 2d. All black

  Node* p = n->parent;
  Node* s;
  Node* c;
  Node* d;

  if (p == NULL) { // Root
    return;
  }
  
  if (p->left == n) { // Left side

    s = p->right;
    c = s->left;
    d = s->right;

    if (s->red) { // 1

      leftRotate(r, p);
      s->red = false;
      p->red = true;

      deleteFix(r, n);
      
    } else { // 2
      if (c != NULL && c->red) { // 2a

	rightRotate(r, s);
	c->red = false;
	s->red = true;

	deleteFix(r, n);
	
      } else if (d != NULL && d->red) { // 2b

	leftRotate(r, p);
	d->red = false;
	bool temp = s->red;
	s->red = p->red;
	p->red = temp;
	
      } else if (p->red) { // 2c

	p->red = false;
	s->red = true;
	
      } else { // 2d

	s->red = true;

	deleteFix(r, p);
	
      }
    }
    
  } else { // Right side

    s = p->left;
    c = s->right;
    d = s->left;

    if (s->red) { // 1

      rightRotate(r, p);
      s->red = false;
      p->red = true;

      deleteFix(r, n);
      
    } else { // 2
      if (c != NULL && c->red) { // 2a

	leftRotate(r, s);
	c->red = false;
	s->red = true;

	deleteFix(r, n);
	
      } else if (d != NULL && d->red) { // 2b

	rightRotate(r, p);
	d->red = false;
	bool temp = s->red;
	s->red = p->red;
	p->red = temp;
	
      } else if (p->red) { // 2c

	p->red = false;
	s->red = true;
	
      } else { // 2d

	s->red = true;

	deleteFix(r, p);
	
      }
    }  
  }
}

void print(Node* c, int depth) {

  Node* left = c->left;
  Node* right = c->right;

  if (right != NULL) {
    print(right, depth + 1);
  }
  
  for (int i = 0; i < depth; i++) {
    cout << "\t";
  }

  if (c->red) {
    cout << "\033[31mR\033[0m";
  } else {
    cout << "B";
  }
  
  cout << c->value << endl;

  if (left != NULL) {
    print(left, depth + 1);
  }
}

Node* search(Node* c, int n) {

  // End or no node
  if (c == NULL) {
    return c;
  }

  // Search and return
  if (n < c->value) {
    return search(c->left, n);
  } else if (n > c->value) {
    return search(c->right, n);
  } else {
    return c;
  }
}

void insertion(Node*& r, Node* c, Node* n) {

  if (r == NULL) {
    r = n;
    insertFix(r, n);
  } else if (n->value < c->value) {
    n->parent = c;

    if (c->left == NULL) {
      c->left = n;
      insertFix(r, n);
    } else {
      insertion(r, c->left, n);
    }
  } else if (n->value > c->value) {
    n->parent = c;

    if (c->right == NULL) {
      c->right = n;
      insertFix(r, n);
    } else {
      insertion(r, c->right, n);
    }
  }
}

void deletion(Node*& r, Node* n) {

  Node* successor = NULL;
  
  if (n->right != NULL && n->left != NULL) { // Two child

    // Find in order successor
    
    successor = n->right;

    while (successor->left != NULL) {
      successor = successor->left;
    }

    int temp = n->value;
    n->value = successor->value;
    successor->value = temp;

    deletion(r, successor);
    
  } else { // One child or no child

    if (n->left) { 
      successor = n->left;
      n->left->red = false;
    } else if (n->right) {
      successor = n->right;
      n->right->red = false;
    } else {
      if (n->red == false) { // Black leaf

	deleteFix(r, n);
	
      }
    }

    // Link successor in tree and delete node

    if (successor) {
      successor->parent = n->parent;
    }
      
    if (n == r) {
      r = successor;
    } else if (n->parent->left == n) {
      n->parent->left = successor;
    } else if (n->parent->right == n) {
      n->parent->right = successor;
    }

    delete n;
    
  }
}
