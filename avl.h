#pragma once
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class AVL {
    struct Node {
        string name;
        int ID, height = 1;
        Node* left;
        Node* right;

        Node();
        Node(string _name, int _id);
        int findHeight(Node* root);
        int balanceFactor(Node* root);
    };

    Node* root;

public:
    AVL();
    void insert(string NAME, int ID);

    void remove(int ID);
    void removeInorder(int N);
    void search(int ID);
    void search(string NAME);
    void printInorder();
    void printPreorder();
    void printPostorder();
    void printLevelCount();

private:
    Node* insertHelper(Node* root, string& NAME, int& ID);
    Node* removeHelper(Node* root, int& ID, bool& remove);
    void removeVectorHelper(Node* root, vector<Node*>& vector);
    int rotation(Node* root);
    void searchHelper(Node* root, int& ID, bool& found);
    void searchHelper(Node* root, string& name, bool& found);
    string inOrderHelper(Node* root);
    string preOrderHelper(Node* root);
    string postOrderHelper(Node* root);
    Node* rotateLeft(Node* root);
    Node* rotateRight(Node* root);
    Node* rotateLeftRight(Node* root);
    Node* rotateRightLeft(Node* root);
};
AVL::Node::Node() {
    name = "";
    ID = 0;
    left = nullptr;
    right = nullptr;
}
AVL::Node::Node(string _name, int _id) {
    name = _name;
    ID = _id;
    left = nullptr;
    right = nullptr;
}
//Node functions
//height starts at 1
//Consulted Jakob Melendez and GeeksForGeeks https://www.geeksforgeeks.org/avl-tree-set-1-insertion/ about algorithm for assigning and updating height variables in insert, remove and rotations
int AVL::Node::findHeight(Node* root) {
    if (!root)
        return 0;
    else
        return root->height;
}
int AVL::Node::balanceFactor(Node* root) {
    if (!root->left && !root->right)
        return 0;
    if (root->left) {
        //Has both left and right children
        if (root->right)
            return root->left->height - root->right->height;
        else
            return root->left->height;
    }
    //only right child, return -1 * height of right
    else
        return -root->right->height;
}

AVL::AVL() {
    root = nullptr;
}
//AVL Functions
void AVL::insert(string NAME, int ID) {
    Node* temp = root;
    root = insertHelper(temp, NAME, ID);
}

//Finds where to insert a node, if it is unique as well as update the height of the nodes it passes
AVL::Node* AVL::insertHelper(Node* root, string& NAME, int& ID) {
    if (root == nullptr) {
        cout << "successful" << endl;
        return new Node(NAME, ID);
    }
    //reorder the nodes and their pointers as you come back up
    if (root->ID < ID)
        root->right = insertHelper(root->right, NAME, ID);
    else if (root->ID > ID)
        root->left = insertHelper(root->left, NAME, ID);
    //when ID isn't unique break out of function, dont want to return nullptr because it will remove the pre-existing ID
    else {
        cout << "unsuccessful" << endl;
        return root;
    }
    //because height set to 1 on initialization, don't need to update height of the lowest node
    root->height = 1 + max(root->left->findHeight(root->left), root->right->findHeight(root->right));

    //After updating height, can check left and right heights to determine the balance factor of current
    int rotationCase = rotation(root);

    switch (rotationCase) {
        //no rotations required
    case 0:
        break;
        //Left Left: Right rotation
    case 1:
        root = rotateRight(root);
        break;
        //Right Right: Left rotation
    case 2:
        root = rotateLeft(root);
        break;

        //Left Right: Left Right rotation
    case 3:
        root = rotateLeftRight(root);
        break;

        //Right Left: Right Left rotation
    case 4:
        root = rotateRightLeft(root);
        break;
    }
    //Return root so that nodes can update their respective positions and pointers coming back up
    return root;
}
//Determines what rotation case is needed
int AVL::rotation(Node* root) {
    if (root->balanceFactor(root) < 2 && root->balanceFactor(root) > -2)
        return 0;
    //Left cases
    else if (root->balanceFactor(root) == 2) {
        if (root->left->balanceFactor(root->left) == 1)
            return 1;
        else
            return 3;
    }
    //Right cases
    else {
        if (root->right->balanceFactor(root->right) == -1)
            return 2;
        else
            return 4;
    }
}
//Both of these are adaptations of what was shown in lecture by Professor Kapoor
//Right rotation for LL case
AVL::Node* AVL::rotateRight(Node* root) {
    Node* newRoot = root->left;
    Node* grandChild = newRoot->right;
    newRoot->right = root;
    root->left = grandChild;

    //update heights
    root->height = 1 + max(root->left->findHeight(root->left), root->right->findHeight(root->right));
    newRoot->height = 1 + max(newRoot->left->findHeight(newRoot->left), newRoot->right->findHeight(newRoot->right));

    return newRoot;
}
//Left rotation for RR case
AVL::Node* AVL::rotateLeft(Node* root) {
    Node* newRoot = root->right;
    Node* grandChild = newRoot->left;
    newRoot->left = root;
    root->right = grandChild;

    //update heights
    root->height = 1 + max(root->left->findHeight(root->left), root->right->findHeight(root->right));
    newRoot->height = 1 + max(newRoot->left->findHeight(newRoot->left), newRoot->right->findHeight(newRoot->right));

    return newRoot;
}

AVL::Node* AVL::rotateLeftRight(Node* root) {
    root->left = rotateLeft(root->left);
    root = rotateRight(root);
    return root;
}
AVL::Node* AVL::rotateRightLeft(Node* root) {
    root->right = rotateRight(root->right);
    root = rotateLeft(root);
    return root;
}
void AVL::remove(int ID) {
    bool remove = true;
    root = removeHelper(root, ID, remove);
    if (remove)
        cout << "successful" << endl;
}
AVL::Node* AVL::removeHelper(Node* root, int& ID, bool& remove) {
    //Might be able to just use this to address all cases because this should treat the root as normal
    if (root == nullptr) {
        remove = false;
        cout << "unsuccessful" << endl;
        return root;
    }
    else if (root->ID > ID)
        root->left = removeHelper(root->left, ID, remove);
    else if (root->ID < ID)
        root->right = removeHelper(root->right, ID, remove);
    //At Node of interest
    else {
        //no children
        if (!root->left && !root->right) {
            delete root;
            root = nullptr;
        }
        //one left child
        else if (root->left && !root->right) {
            Node* temp = root->left;
            delete root;
            root = temp;
        }
        //one right child
        else if (root->right && !root->left) {
            Node* temp = root->right;
            delete root;
            root = temp;
        }
        else {
            //Find left most right  node from the root
            Node* temp = root->right;

            //If root's right is not the left most right node
            if (temp->left) {
                while (temp->left)
                    temp = temp->left;
            }

            //Algorithm for reassigning nodes at this point from https://www.geeksforgeeks.org/binary-search-tree-set-2-delete/
            root->ID = temp->ID;
            root->name = temp->name;
            //Like insert, do this to reorganize the tree as you go back up
            root->right = removeHelper(root->right, temp->ID, remove);
        }
        //Update the heights after removing something
        if (root)
            root->height = 1 + max(root->left->findHeight(root->left), root->right->findHeight(root->right));
        return root;
    }
    return root;
}

void AVL::removeInorder(int N) {
    //make some vector and fill it
    vector<Node*> tree;
    removeVectorHelper(root, tree);
    if (N > tree.size() - 1)
        cout << "unsuccessful" << endl;
    else
        remove(tree.at(N)->ID);
}
//helper function used to fill a vector with nodes in the tree via inorder
void AVL::removeVectorHelper(Node* root, vector<Node*>& vector) {
    if (!root)
        return;
    else {
        removeVectorHelper(root->left, vector);
        vector.push_back(root);
        removeVectorHelper(root->right, vector);
    }
}
//use bool to catch for when searching is unsuccessful
void AVL::search(int ID) {
    Node* temp = root;
    bool found = false;
    searchHelper(temp, ID, found);
    if (!found)
        cout << "unsuccessful" << endl;
}
void AVL::searchHelper(Node* root, int& ID, bool& found) {
    if (root == nullptr)
        return;
    if (root->ID == ID) {
        found = true;
        cout << root->name << endl;
    }
    else {
        if (root->ID > ID)
            searchHelper(root->left, ID, found);
        else
            searchHelper(root->right, ID, found);
    }
}
void AVL::search(string NAME) {
    Node* temp = root;
    bool found = false;
    searchHelper(temp, NAME, found);
    if (!found)
        cout << "unsuccessful" << endl;
}
void AVL::searchHelper(Node* root, string& name, bool& found) {
    if (root == nullptr)
        return;
    else {
        if (root->name == name) {
            found = true;
            string foundID = to_string(root->ID);
            //at 0's to the beginning of the ID
            while (foundID.length() < 8) {
                foundID.insert(0, "0");
            }
            cout << foundID << endl;
        }
        searchHelper(root->left, name, found);
        searchHelper(root->right, name, found);
    }
}
void AVL::printInorder() {
    string print = inOrderHelper(root);
    //get rid of last comma and space
    print = print.substr(0, print.length() - 2);
    cout << print << endl;
}
string AVL::inOrderHelper(Node* root) {
    if (root == nullptr)
        return "";

    else
        return
        inOrderHelper(root->left) + root->name + ", " + inOrderHelper(root->right);
}
void AVL::printPreorder() {
    string print = preOrderHelper(root);
    print = print.substr(0, print.length() - 2);
    cout << print << endl;
}
string AVL::preOrderHelper(Node* root) {
    if (root == nullptr)
        return "";

    else
        return root->name + ", " + preOrderHelper(root->left) + preOrderHelper(root->right);
}
void AVL::printPostorder() {
    string print = postOrderHelper(root);
    print = print.substr(0, print.length() - 2);
    cout << print << endl;
}
string AVL::postOrderHelper(Node* root) {
    if (root == nullptr)
        return "";

    else
        return
        postOrderHelper(root->left) + postOrderHelper(root->right) + root->name + ", ";
}
//update height here since not done in remove function
void AVL::printLevelCount() {
    if (root == nullptr)
        cout << 0 << endl;

    else
        cout << root->height << endl;
}