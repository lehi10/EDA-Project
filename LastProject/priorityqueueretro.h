#ifndef PRIORITYQUEUERETRO_H
#define PRIORITYQUEUERETRO_H
#include <iostream>
#include "avltree.h"
#include <time.h>


using namespace std;

class PrioQueueRetro
{
    public:
        AVLTree * updateTree;
        AVLTree * insertsTree;
        AVLTree * priorityTree;
        ofstream file;
        time_t timer;
        int stimer ;


        PrioQueueRetro();
        void insertNow(int value) ; //insert in Now
        void insertInTime(int time,int value);
        int popMinNow();
        void deleteMinInTime(int time);
        int get_time();

        void imprimir();
};

#endif // PRIORITYQUEUERETRO_H

PrioQueueRetro::PrioQueueRetro()
{
    stimer =0;

    priorityTree = new AVLTree;
    insertsTree = new AVLTree;
    updateTree = new AVLTree;

}

void PrioQueueRetro::insertNow(int value)
{
    vector<int> _mods(3);
    _mods[0]=value;
    _mods[1]=0;//kind
    _mods[2]=0;//kind2

    int _time= get_time();
    updateTree->insert(_time,_mods);

    vector<int> _mods2(3);
    _mods[0]=value;
    _mods[1]=0;//kind
    _mods[2]=0;//kind2
    insertsTree->insert(_time,_mods2);
    priorityTree->insert(value,vector<int>(1));
}

int PrioQueueRetro::popMinNow()
{

}

void PrioQueueRetro::deleteMinInTime(int time)
{

}


void PrioQueueRetro::insertInTime(int time, int value)
{
    vector<int> mods(3);
    mods[0]=value;
    mods[1]=0;//kind
    mods[2]=0;//kind2

    updateTree->insert(time,mods);

    cout<<"nodo : "<<updateTree->find(time)->key;
    Node* it=updateTree->root;

    while(it->left)
        it=it->left;

    while(it)
    {
        cout<<it->key<<" ";
        it=it->next;
    }
    cout<<endl;

/*

    updateTree->insert(time,_mods);
    /
    insertsTree->insert(time,_mods);


    priorityTree->insert(value,NULL);
*/
}



void PrioQueueRetro::imprimir()
{
    ofstream file("updateTree.dot");
    file<<"digraph G {"<<endl<<"node [shape=record];";
    // Priority Queue
    priorityTree->graphviz(priorityTree->root, file);
    //AVL Tree of Updates
    updateTree->graphviz(updateTree->root,file);
    updateTree->graphvizRow(file);
    // AVLTree of Inserts
    insertsTree->graphviz(insertsTree->root,file);
    //insertsTree->graphvizRow(file);
    file<<"}"<<endl;
    file.close();
    system("dot -Tpdf updateTree.dot -o updateTree.pdf ");
    //system("evince updateTree.pdf");
}

int PrioQueueRetro::get_time()
{
    //return time(&timer)%10000;
    stimer += 10;
    return stimer;

}
