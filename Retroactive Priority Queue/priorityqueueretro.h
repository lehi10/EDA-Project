#ifndef PRIORITYQUEUERETRO_H
#define PRIORITYQUEUERETRO_H
#include <iostream>
#include "avltree.h"
#include <time.h>
#include <map>


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
        map<int,int> priorityTreeM;


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
    //priorityTree = new AVLTree;
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

    priorityTreeM[value]=value;
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

    // Se inserta un nuevo nodo en el arbol de actualizaciones que está ordenado por tiempos
    updateTree->insert(time,mods);

    //Rompiendo puente siguiente si existe
    Node * ChangeBridge = updateTree->find(time);
    while(ChangeBridge != nullptr && ChangeBridge->mods[1]!=0 )
    {
        ChangeBridge=ChangeBridge->next;
    }
    if(ChangeBridge)
    {
        cout<<"Bridge : "<<ChangeBridge->key<<endl;
        ChangeBridge->mods[1]=1;
    }

    Node * itChange=updateTree->find(time)->prev;//!!!!!!!!!!!!!!!!
    Node * minAlive=nullptr;

    while(itChange)
    {
        cout<<"entro al while"<<endl;
        if(itChange->mods[1]==0)
        {
            if(minAlive == nullptr || itChange->mods[0] < minAlive->mods[0] )
                minAlive= itChange;
        }
         itChange=itChange->prev;
    }

    minAlive->mods[1]=1;
    if(ChangeBridge)
        minAlive->mods[1]=0;

    //sumamos los mods para actualizar los estados

    Node* itSuma = nullptr;
    if(ChangeBridge)
        itSuma=ChangeBridge;
    else
        itSuma=updateTree->find(time);//!!!!!!!!!!!!!!!!

    while(itSuma)
    {
        if(itSuma->next)
        {
            int sumaMods= itSuma->next->mods[1]+itSuma->next->mods[2];
            itSuma->mods[2]=sumaMods;
        }
        itSuma=itSuma->prev;
    }

    //Se busca el Puente más cercano
    Node* bridge=updateTree->find(time);

    while( bridge->mods[2] != 0 and bridge!=nullptr)
        bridge=bridge->next;

    cout<<" Puente !! "<<bridge->key;
//-----------------------------------------------------------------------------------------
    //El tiempo del puente más cercano se busca en el arbol de Insersiones
    Node * itInsert = insertsTree->find(bridge->key);//!!!!!!!!!!!!!!!!
    //Se busca en el InsertTree al menor presente en el  actual
    Node * minCurrent=nullptr;

    while(itInsert)
    {
        if(itInsert->mods[1]==1)
        {
            if(minCurrent == nullptr || itInsert->mods[0] < minCurrent->mods[0] )
                minCurrent= itInsert;
        }
        if(ChangeBridge)
            itInsert=itInsert->next;
        else
            itInsert=itInsert->prev;
    }
    //Si existe el menor en el actual
    if(minCurrent)
    {
        //Se cambia el valor del nodo insertado
        minCurrent->mods[1]=0;
        // Se elimina al menor presente del PriorityTree.
        priorityTreeM.erase(minCurrent->mods[0]);
        //priorityTree->deleteNode(minCurrent->mods[0]);
        return;
    }
}


void PrioQueueRetro::insertInTime(int time, int value)
{
    vector<int> mods(3);
    mods[0]=value;
    mods[1]=0;//kind  -1 0 1
    mods[2]=0;//kind2 suma de anteriores

    // Se inserta un nuevo nodo en el arbol de actualizaciones que está ordenado por tiempos
    updateTree->insert(time,mods);

    Node * itChange=updateTree->find(time)->next;//!!!!!!!!!!!!!!!!
    Node * itMaxDel=nullptr;

    while(itChange)
    {
        if(itChange->mods[1]==1)
        {
            if(itMaxDel == nullptr || itChange->mods[0] > itMaxDel->mods[0] )
                itMaxDel= itChange;
        }
         itChange=itChange->next;
    }

    if(itMaxDel)
    {

        cout<<itMaxDel->key<<"  --  "<<endl;
        updateTree->find(time)->mods[1]=1;
        itMaxDel->mods[1]=0;

        // Actualiza las sumas
        Node* itSuma = itMaxDel;

        while(itSuma)
        {
            if(itSuma->next)
            {
                int sumaMods= itSuma->next->mods[1]+itSuma->next->mods[2];
                itSuma->mods[2]=sumaMods;
                cout<<itSuma->key<<" :"<<itSuma->mods[2]<<" -s-  ";
            }
            itSuma=itSuma->prev;
        }
        cout<<"termino sumas"<<endl;
    }

    Node* bridge=updateTree->find(time)->prev; //!!!!!!!!!!!!!!!!
    cout<<"Puente ? "<<bridge->mods[2]<<" - "<<bridge<<endl;

    while( bridge != nullptr && bridge->mods[2] != 0  )
    {
            cout<<" .. [ "<<bridge->key<<" - "<<bridge->mods[2]<<" ] ";
            bridge=bridge->prev;
    }

    cout<<"paso while"<<endl;

    // Se inserta un nuevo nodo en el arbol de inserts
    vector<int> modInsert(3);
    modInsert[0]=value; // valor
    modInsert[1]=0;//kind  MARCADOR 0 POR QUE NO ESTA PRESENTE EN EL ACTUAL
    insertsTree->insert(time,modInsert);
    cout<<bridge->key<<" inserto en inserts"<<endl;
    //El tiempo del puente más cercano se busca en el arbol de Insersiones    
    Node * itInsert = insertsTree->find(bridge->key);//!!!!!!!!!!!!!!!!
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

        // El mayor eliminado vuelve a la priorityTree
        maxDel->mods[1]=1;
        priorityTreeM[maxDel->mods[0]]=maxDel->mods[0];
        //priorityTree->insert(maxDel->mods[0],vector<int>(3));
        return;
    }
    // Si no existe el mayor eliminado significa que el nuevo está en el Actual
    else
    {
        // Insertamos en el Insert Tree con el marcador en 1
        priorityTreeM[value]=value;
        //priorityTree->insert(value,vector<int>(3));
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

    //priorityTree->graphvizTreeCommon(priorityTree->root, file);
    graphvizTreeCommon(priorityTreeM,file);

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
