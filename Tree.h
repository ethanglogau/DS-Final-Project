#pragma once
#include <string>
#include <iostream>
#include <vector>
using namespace std;

class Tree {
public:
    struct Node {
        vector<string> ID;         //ID is spotify ID, use a vector for duplicates
        float value = 0.0f;         //value can be interchanged with whatever we are using
        int height = 1;
        Node* left = nullptr;
        Node* right = nullptr;

        Node();
        Node(string _ID, float _value);
        int findHeight(Node* root);
        int balanceFactor(Node* root);
    };
    Tree();
    void insert(string& ID, float& value);
    void PrintInOrder();
    Node* root = nullptr;

private:

    void PrintInOrderRec(Node* curr, vector<string>& temp);
    //string inOrderHelper(Node* root);
    Node* insertHelper(Node* root, string& ID, float& value);
    int rotation(Node* root);
    Node* rotateLeft(Node* root);
    Node* rotateRight(Node* root);
    Node* rotateLeftRight(Node* root);
    Node* rotateRightLeft(Node* root);
    Node* search(Node* root, float& value);
};
//Node functions
Tree::Node::Node(string _ID, float _value) {
    ID.push_back(_ID);
    value = _value;
}
//height starts at 1
int Tree::Node::findHeight(Node* root) {
    if (!root)
        return 0;
    else
        return root->height;
}
int Tree::Node::balanceFactor(Node* root) {
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
Tree::Tree() {
    root = nullptr;
}

//Tree Functions
void Tree::insert(string& ID, float& value) {
    Node* temp = root;
    root = insertHelper(temp, ID, value);
}

//Finds where to insert a node, if it is unique as well as update the height of the nodes it passes
Tree::Node* Tree::insertHelper(Node* root, string& ID, float& value) {
    if (root == nullptr) {
        return new Node(ID, value);
    }
    //reorder the nodes and their pointers as you come back up
    if (root->value < value)
        root->right = insertHelper(root->right, ID, value);
    else if (root->value > value)
        root->left = insertHelper(root->left, ID, value);
    //When value is not unique then add the ID to the vector of songs
    else {
        root->ID.push_back(ID);
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
int Tree::rotation(Node* root) {
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
Tree::Node* Tree::rotateRight(Node* root) {
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
Tree::Node* Tree::rotateLeft(Node* root) {
    Node* newRoot = root->right;
    Node* grandChild = newRoot->left;
    newRoot->left = root;
    root->right = grandChild;

    //update heights
    root->height = 1 + max(root->left->findHeight(root->left), root->right->findHeight(root->right));
    newRoot->height = 1 + max(newRoot->left->findHeight(newRoot->left), newRoot->right->findHeight(newRoot->right));

    return newRoot;
}

Tree::Node* Tree::rotateLeftRight(Node* root) {
    root->left = rotateLeft(root->left);
    root = rotateRight(root);
    return root;
}
Tree::Node* Tree::rotateRightLeft(Node* root) {
    root->right = rotateRight(root->right);
    root = rotateLeft(root);
    return root;
}

void Tree::PrintInOrder() {
    vector<string> temp;
    PrintInOrderRec(root, temp);
    if (temp.empty())
        cout << "" << endl;
    else {
        for (int i = 0; i < temp.size() - 1; i++)
            cout << temp[i] << ", ";
        cout << temp[temp.size() - 1] << endl;
    }
}
void Tree::PrintInOrderRec(Node* curr, vector<string>& temp) {
    if (curr == nullptr) {

    }
    else {
        PrintInOrderRec(curr->left, temp);
        for (string& x : curr->ID)
            temp.push_back(x);
        PrintInOrderRec(curr->right, temp);
    }
}