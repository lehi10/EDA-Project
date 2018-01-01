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
    stimer =-10;
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
    _mods2[0]=value;
    _mods2[1]=1;//kind
    insertsTree->insert(_time,_mods2);

    priorityTree->insert(value,vector<int>(1));
}

int PrioQueueRetro::popMinNow()
{

}

void PrioQueueRetro::deleteMinInTime(int time)
{
    vector<int> mods(3);
    mods[0]=0; // Este valor no existe por que este nodo solo es una actualización
    mods[1]=-1;//kind  -1 0 1
    mods[2]=0;//kind2 suma de anteriores

    cout<<"insertando actualización"<<endl;
    // Se inserta un nuevo nodo en el arbol de actualizaciones que está ordenado por tiempos
    updateTree->insert(time,mods);

    cout<<"Buscando puente cercano"<<endl;

    //Se busca el Puente más cercano
    Node* bridge=updateTree->find(time)->prev;//!!!!!!!!!!!!!!!!
    while( bridge->mods[2] != 0 and bridge!=nullptr)
        bridge=bridge->prev;

    //Cambiamos al puente de 0 a 1 , por que dejará de ser un puente.
    bridge->mods[1]=1;

    cout<<"actualizando para sumas de anteriores desde el puente : "<<bridge->key<<endl;
    //sumamos los mods para actualizar los estados
    Node* itSuma = updateTree->find(time);//!!!!!!!!!!!!!!!!
    while(itSuma)
    {
        if(itSuma->next)
        {
            int sumaMods= itSuma->next->mods[1]+itSuma->next->mods[2];
            itSuma->mods[2]=sumaMods;
            cout<<itSuma->mods[2]<<" ";
        }
        itSuma=itSuma->prev;
    }
    cout<<"buscando el insertsTree"<<endl;
    //El tiempo del puente más cercano se busca en el arbol de Insersiones
    Node * itInsert = insertsTree->find(bridge->key);//!!!!!!!!!!!!!!!!
    cout<<"bridge en inserts tree encontrado "<<itInsert->key<<endl;
    cout<<"buscando en el insertTree al menor presente actual"<<endl;
    //Se busca en el InsertTree al menor presente en el  actual
    Node * minCurrent=nullptr;

    while(itInsert)
    {
        if(itInsert->mods[1]==1)
        {
            if(minCurrent == nullptr || itInsert->mods[0] < minCurrent->mods[0] )
                minCurrent= itInsert;
        }
        cout<<itInsert->key<<" ";
        itInsert=itInsert->prev;

    }
    cout<<endl;


    cout<<"si existe el menor en la preiority Queue"<<endl;
    //Si existe el menor en el actual

    if(minCurrent)
    {
        cout<<"cambiamos el calor del nodo "<<endl;
        //Se cambia el valor del nodo insertado
        minCurrent->mods[1]=0;

        cout<<"se elimina al menor presente de la PQ "<<minCurrent->mods[0]<<endl;
        // Se elimina al menor presente del PriorityTree.
        priorityTree->deleteNode(minCurrent->mods[0]);
        return;
    }
    cout<<"No se encontro un minCurrent"<<endl;

/*
    // Si no existe el mayor eliminado significa que el nuevo está en el Actual
    else
    {
        // Insertamos en el Insert Tree con el marcador en 1
        vector<int> modInsert(3);
        modInsert[0]=value; // valor
        modInsert[1]=1; //kind  MARCADOR EN 1 POR QUE SI ESTA PRESENTE EN EL ACTUAL
        insertsTree -> insert(time,modInsert);
        // Insertamos el valor en el arbol de Prioridades
        priorityTree->insert(value,vector<int>(3));
        return;
    }
*/

}


void PrioQueueRetro::insertInTime(int time, int value)
{
    vector<int> mods(3);
    mods[0]=value;
    mods[1]=0;//kind  -1 0 1
    mods[2]=0;//kind2 suma de anteriores


    // Se inserta un nuevo nodo en el arbol de actualizaciones que está ordenado por tiempos
    updateTree->insert(time,mods);

    //Se busca el Puente más cercano
    Node* bridge=updateTree->find(time)->prev; //!!!!!!!!!!!!!!!!

    while( bridge->mods[2] != 0 and bridge != nullptr)
        bridge=bridge->prev;

    //El tiempo del puente más cercano se busca en el arbol de Insersiones
    Node * itInsert = insertsTree->find(bridge->key)->next;//!!!!!!!!!!!!!!!!

    //Se busca al mayor eliminado
    Node * maxDel=nullptr;
    while(itInsert)
    {
        if(itInsert->mods[1]==0)
        {
            if(maxDel == nullptr || maxDel->mods[0] < itInsert->mods[0] )
                maxDel= itInsert;
        }
        itInsert=itInsert->next;
    }

    //Si existe el mayor eliminado
    if(maxDel)
    {
        // Se inserta un nuevo nodo en el arbol de insersiones
        vector<int> modInsert(3);
        mods[0]=value; // valor
        mods[1]=0;//kind  MARCADOR 0 POR QUE NO ESTA PRESENTE EN EL ACTUAL
        insertsTree->insert(time,modInsert);


        // El mayor eliminado vuelve a la priorityTree
        maxDel->mods[1]=1;
        priorityTree->insert(maxDel->mods[0],vector<int>());

        return;
    }

    // Si no existe el mayor eliminado significa que el nuevo está en el Actual
    else
    {
        // Insertamos en el Insert Tree con el marcador en 1
        vector<int> modInsert(3);
        modInsert[0]=value; // valor
        modInsert[1]=1; //kind  MARCADOR EN 1 POR QUE SI ESTA PRESENTE EN EL ACTUAL
        insertsTree -> insert(time,modInsert);
        // Insertamos el valor en el arbol de Prioridades
        priorityTree->insert(value,vector<int>(3));
        return;
    }
}



void PrioQueueRetro::imprimir()
{
    ofstream file("updateTree.dot");
    file<<"digraph G {      "<<endl;
    file<<"label = \" Priority Queue Retroactive  \";"<<endl;
    // Priority Queue

    file<<"subgraph cluster_0 { "<<endl;
    file<<" color=lightgrey; node [style=filled];";

    priorityTree->graphvizTreeCommon(priorityTree->root, file);

    file<<"label = \" Priority Tree  \"; color=blue }"<<endl;

//--------------------------------------------------------------
    //AVL Tree of Updates

    file<<"subgraph cluster_1 { "<<endl;
    file<<" color=lightgrey; node [shape=record , style=filled];";

    updateTree->graphviz(updateTree->root,file);
    updateTree->graphvizRow(file);

    file<<"label = \" Update Tree Tree  \";  }"<<endl;


//--------------------------------------------------------------
    // AVLTree of Inserts
    file<<"subgraph cluster_2 { "<<endl;
    file<<" color=lightgrey; node [shape=record , style=filled];";

    insertsTree->graphvizTreeInserts(insertsTree->root,file);

    file<<"label = \" Inserts Tree  \";  }"<<endl;
//---------------------------------------------------------------


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
