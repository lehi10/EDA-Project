#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include<fstream>
#include <vector>
#include <map>

using namespace std;


class Node
{
    public:
        int key; // Tiempo
        Node *left;
        Node *right;
        int height;
        vector<int> mods;

        Node * next;
        Node * prev;

        Node(int _key){
            key   = _key;
            left   = nullptr;
            right  = nullptr;
            height = 1;
            next=nullptr;
            prev=nullptr;
        }
};

int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b)
{
    return (a > b)? a : b;
}


class AVLTree
{
    public:
        Node *root;

        AVLTree(){root=nullptr;}

        Node *rightRotate( Node **y);
        Node *leftRotate(Node **x);
        int getBalance(Node *N);
        Node* insert(Node* node, vector<int> _mods,int key , Node * next, Node *prev );
        void insert(int key,vector<int> _mods);
        Node * balanceo(Node *node,int key);
        void graphviz(Node *node, ofstream & file);
        void graphvizTreeInserts(Node *node, ofstream & file);

        void graphvizRow(ofstream &file);
        void preOrder(Node *node);
        void imprimir(ofstream & file);
        Node *find(int time, Node *node);
        Node *find(int time);
        Node * minValueNode(Node* node);
        Node *deleteNode(struct Node* root, int key);
        void deleteNode(int key);

};



Node * AVLTree::rightRotate( Node **y)
{
    Node *x = (*y)->left;
    Node *T2 = x->right;
    x->right = *y;
    (*y)->left = T2;
    (*y)->height = max(height((*y)->left), height((*y)->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    return x;
}


Node * AVLTree::leftRotate(Node **x)
{
    Node *y = (*x)->right;
    Node *T2 = y->left;
    y->left = *x;
   (*x)->right = T2;
   (*x)->height = max(height((*x)->left), height((*x)->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    return y;
}

int AVLTree::getBalance(Node *N)
{
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}


Node * AVLTree::balanceo(Node * node,int key)
{
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);


    if (balance > 1 && key < node->left->key)
        return rightRotate(&node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(&node);

    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(&node->left);
        return rightRotate(&node);
    }


    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(&node->right);
        return leftRotate(&node);
    }
    return node;
}

Node* AVLTree::insert(Node* node, vector<int> _mods,int key , Node * next, Node *prev )
{
    if (node == nullptr)
    {
        Node * newNode=new Node(key);
        newNode->mods=_mods;
        newNode->next=next;
        newNode->prev=prev;
        if(prev)
            newNode->prev->next=newNode;
        if(next)
            newNode->next->prev=newNode;
        return newNode;

    }
    if (key <= node->key)
    {
        next=node;
        //node->prev= node->left;
        node->left  = insert(node->left, _mods,key,next,prev);
    }
    else if (key  > node->key)
    {
        prev=node;
        node->right = insert(node->right, _mods,key,next, prev);
    }
    return balanceo(node,key);
}

void AVLTree::insert(int key,vector<int> _mods)
{
    root=insert(root,_mods,key,nullptr,nullptr);
}


Node * AVLTree::minValueNode(Node* node)
{
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

Node* AVLTree::deleteNode(Node* node, int key)
{
    if (node == nullptr)
        return node;

    if ( key < node->key )
        node->left = deleteNode(node->left, key);

    else if( key > root->key )
        node->right = deleteNode(node->right, key);

    else
    {
        if( (node->left == nullptr) || (node->right == nullptr) )
        {
            Node *temp = root->left ? node->left : node->right;
        if (temp == nullptr)
            {
                temp = node;
                node = nullptr;
            }
            else
             *node = *temp;
            free(temp);
        }
        else
        {
            struct Node* temp = minValueNode(node->right);

            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }
    }

    if (node == nullptr)
      return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0)
        return rightRotate(&node);

    if (balance > 1 && getBalance(node->left) < 0)
    {
        root->left =  leftRotate(&node->left);
        return rightRotate(&node);
    }

    if (balance < -1 && getBalance(node->right) <= 0)
        return leftRotate(&node);

    if (balance < -1 && getBalance(node->right) > 0)
    {
        root->right = rightRotate(&root->right);
        return leftRotate(&node);
    }
    return node;
}

void AVLTree::deleteNode(int key)
{
    root=deleteNode(root,key);
}

//------------------------------------------------------------------------------------------------



void AVLTree::graphviz(Node *node, ofstream & file)
{
    if(node != nullptr)
    {
        file<<"\""<<node<<"\""<<" [ shape=record, label=\" "<<node->key<<" | { "<< node->mods[0] <<"| { " <<node->mods[1]<<" | "<<node->mods[2]<<" } } "<<"\"];"<<endl;
        if(node->left)
            file<<"\""<<node<<"\""<<" -> "<<"\""<<node->left<<"\""<<endl;
        if(node->right)
            file<<"\""<<node<<"\""<<" -> "<<"\""<<node->right<<"\""<<endl;
        graphviz(node->left,file);
        graphviz(node->right,file);
    }
}



void AVLTree::graphvizTreeInserts(Node *node, ofstream & file)
{
    if(node != nullptr)
    {
        file<<"\""<<node<<"\""<<" [ shape=record, label=\" "<<node->key<<" | { "<< node->mods[0] <<"| " <<node->mods[1]<<"} "<<"\"];"<<endl;
        if(node->left)
            file<<"\""<<node<<"\""<<" -> "<<"\""<<node->left<<"\""<<endl;
        if(node->right)
            file<<"\""<<node<<"\""<<" -> "<<"\""<<node->right<<"\""<<endl;
        graphvizTreeInserts(node->left,file);
        graphvizTreeInserts(node->right,file);
    }
}




void AVLTree::graphvizRow(ofstream & file)
{
    Node* it=root;
    while(it->left)
        it=it->left;

    file<<"\""<<it<<"\"";
    while(it->next)
    {
        it=it->next;
        file<<" ->  \""<<it<<"\"";
    }
    file<< "[color=\"red\"] ;"<<endl;
/*

    it=root;
    while(it->right)
        it=it->right;

    file<<"\""<<it<<"\"";
    while(it->prev)
    {
        it=it->prev;
        file<<" ->  \""<<it<<"\"";
    }
    file<< "[color=\"red\"] ;"<<endl;
*/
}

void AVLTree::preOrder(Node *node)
{
    if(node != nullptr)
    {
        cout<< node->key<<" ";
        preOrder(node->left);
        preOrder(node->right);
    }
}

Node *AVLTree::find(int key, Node *node)
{
    if(key < node->key)
    {
        if(node->left)
            return find(key,node->left);
        else
            return node->prev;
        // no se puede insertar antes del tiempo inicial
    }
    else
    {
        if(key == node->key)
            return node;
        else
        {
            if(node->right)
                return find(key,node->right);
            else
                return node;
        }
    }
}


Node *AVLTree::find(int key)
{
    return find(key, root);
}



void graphvizTreeCommon(map<int,int> mymap, ofstream & file)
{
    for (std::map<int,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    {
        file<< it->first << " -> ";
    }
    file<<"x"<<endl;
}


#endif // AVLTREE_H
