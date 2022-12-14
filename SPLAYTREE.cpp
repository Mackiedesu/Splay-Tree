#include <iostream>

using namespace std;

// Node 
struct Node {
	int data;
	Node* parent; 
	Node* left; 
	Node* right; 
};

typedef Node* NodePtr;

// Splay Tree 
class SplayTree {
private:
	NodePtr root;

	void preOrderHelper(NodePtr node) {
		if (node != nullptr) {
			cout << node->data << " ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(NodePtr node) {
		if (node != nullptr) {
			inOrderHelper(node->left);
			cout << node->data << " ";
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(NodePtr node) {
		if (node != nullptr) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout << node->data << " ";
		}
	}

	NodePtr searchTreeHelper(NodePtr node, int key) {
		if (node == nullptr || key == node->data) {
			return node;
		}

		if (key < node->data) {
			return searchTreeHelper(node->left, key);
		}
		return searchTreeHelper(node->right, key);
	}

	// xoay trái tại x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != nullptr) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// xoay phải tại x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != nullptr) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
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

	// It's splay time
	void splay(NodePtr x) {
		while (x->parent) {
			if (!x->parent->parent) {
				if (x == x->parent->left) {
					// zig
					rightRotate(x->parent);
				}
				else {
					// zag
					leftRotate(x->parent);
				}
			}
			else if (x == x->parent->left && x->parent == x->parent->parent->left) {
				// zig-zig 
				rightRotate(x->parent->parent);
				rightRotate(x->parent);
			}
			else if (x == x->parent->right && x->parent == x->parent->parent->right) {
				// zag-zag 
				leftRotate(x->parent->parent);
				leftRotate(x->parent);
			}
			else if (x == x->parent->right && x->parent == x->parent->parent->left) {
				// zig-zag 
				leftRotate(x->parent);
				rightRotate(x->parent);
			}
			else {
				// zag-zig 
				rightRotate(x->parent);
				leftRotate(x->parent);
			}
		}
	}
	
	// joins two trees s and t
	NodePtr join(NodePtr s, NodePtr t){
		if (!s) {
			return t;
		}

		if (!t) {
			return s;
		}
		NodePtr x = maximum(s);
		splay(x);
		x->right = t;
		t->parent = x;
		return x;
	}

	// splits the tree into s and t
	void split(NodePtr &x, NodePtr &s, NodePtr &t) {
		splay(x);
		if (x->right) {
			t = x->right;
			t->parent = nullptr;
		} else {
			t = nullptr;
		}
		s = x;
		s->right = nullptr;
		x = nullptr;
	} 

	void deleteNodeHelper(NodePtr node, int key) {
		NodePtr x = nullptr;
		NodePtr t, s;
		while (node != nullptr) {
			if (node->data == key) {
				x = node;
			}

			if (node->data <= key) {
				node = node->right;
			}
			else {
				node = node->left;
			}
		}

		if (x == nullptr) {
			cout << "Couldn't find key in the tree" << endl;
			return;
		}
		split(x, s, t); // chia cây
		if (s->left) { // xóa x
			s->left->parent = nullptr;
		}
		root = join(s->left, t);
		delete(s);
		s = nullptr;
	}
	
	void printHelper(NodePtr root, string indent, bool last) {
		// print the tree structure on the screen
	   	if (root != nullptr) {
		   cout<<indent;
		   if (last) {
		      cout<<"└────";
		      indent += "     ";
		   } else {
		      cout<<"├────";
		      indent += "|    ";
		   }

		   cout<<root->data<<endl;

		   printHelper(root->left, indent, false);
		   printHelper(root->right, indent, true);
		}
	}

public:
	SplayTree() {
		root = nullptr;
	}

	// Duyệt Pre-Order 
	// Node->Left Subtree->Right Subtree
	void preorder() {
		preOrderHelper(this->root);
	}

	// Duyệt In-Order 
	// Left Subtree -> Node -> Right Subtree
	void inorder() {
		inOrderHelper(this->root);
	}

	// Duyệt Post-Order 
	// Left Subtree -> Right Subtree -> Node
	void postorder() {
		postOrderHelper(this->root);
	}

	// tìm khóa k trên cây rồi trả về node tương ứng
	NodePtr searchTree(int k) {
		NodePtr x = searchTreeHelper(this->root, k);
		if (x) {
			splay(x);
		}
		return x;
	}

	// tìm node có khóa nhỏ nhất
	NodePtr minimum(NodePtr node) {
		while (node->left != nullptr) {
			node = node->left;
		}
		return node;
	}

	// tìm node có khóa lớn nhất
	NodePtr maximum(NodePtr node) {
		while (node->right != nullptr) {
			node = node->right;
		}
		return node;
	}

	// insert khóa vào cây theo đúng vị trí
	void insert(int key) {
		// normal BST insert
		NodePtr node = new Node;
		node->parent = nullptr;
		node->left = nullptr;
		node->right = nullptr;
		node->data = key;
		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != nullptr) {
			y = x;
			if (node->data < x->data) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		// y là cha x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		}
		else if (node->data < y->data) {
			y->left = node;
		}
		else {
			y->right = node;
		}

		// SPLAY!!
		splay(node);
	}

	NodePtr getRoot() {
		return this->root;
	}

	// xóa node 
	void deleteNode(int data) {
		deleteNodeHelper(this->root, data);
	}
	
	void splaytreePrint() {
		printHelper(this->root, "", true);
	}

};

int main() {
	SplayTree bst;
	bst.insert(33);
	bst.insert(44);
	bst.insert(67);
	bst.insert(5);
	bst.insert(89);
	bst.insert(41);
	bst.insert(98);
	bst.insert(1);
	bst.splaytreePrint();
	bst.searchTree(33);
	bst.searchTree(44);
	bst.splaytreePrint();
	bst.deleteNode(89);
	bst.deleteNode(67);
	bst.deleteNode(41);
	bst.deleteNode(5);
	bst.splaytreePrint();
	bst.deleteNode(98);
	bst.deleteNode(1);
	bst.deleteNode(44);
	bst.deleteNode(33);
	bst.splaytreePrint();
	return 0;
}
