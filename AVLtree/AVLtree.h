#ifndef AVLTREE_H
#define AVLTREE_H
#include "nodoAVL.h"

template <typename T>
class AVLtree
{
    public:
            AVLtree():m_head(nullptr){}
            AVLnodo<T> *m_head;
            bool find(T d,AVLnodo<T> ** &p );
            bool add(T dato);
            AVLtree<T> &  operator <<(T dato);
            void printIn(AVLnodo<T> * nodo);
            void printPos(AVLnodo<T> * nodo);
            void printPre(AVLnodo<T> * nodo);
            void balancear(AVLnodo<T> * nodo);
            void insert(AVLnodo<T> *nodo,T dato);
            void insert(T dato);
            void print();
};

#endif // AVLTREE_H
template <typename T >
void AVLtree<T>::insert(AVLnodo<T> *temp,T dato)
{

    T temp_d=temp->m_dato;
    if(temp_d < dato)
    {
        if(temp->hijos[1])
            return insert(temp->hijos[1],dato);
        AVLnodo<T> *new_nodo=new AVLnodo<T>(dato);
        temp->hijos[1]=new_nodo;
        new_nodo->padre=temp;

        balancear(new_nodo); //balanceo
        return;
    }
    if(temp_d > dato)
    {
        if(temp->hijos[0])
            return insert(temp->hijos[0],dato);
        AVLnodo<T> *new_nodo=new AVLnodo<T>(dato);
        temp->hijos[0]=new_nodo;
        new_nodo->padre=temp;
        balancear(new_nodo); //balanceo
        return;
    }
}
template <typename T>
void AVLtree<T>::insert(T dato)
{
    cout<<"insertado :"<<dato<<endl;
    if(m_head)
    {
        insert(m_head,dato);
        return;
    }
    m_head = new AVLnodo<T>(dato);
    m_head->padre=nullptr;

}

template <typename T>
bool AVLtree<T>::find(T d,AVLnodo<T> ** &p )
{
    p  =& m_head;
    while(*p)
    {
        if((*p)->m_dato== d)
            return true;
        p=&(*p)->hijos[(*p)->m_dato < d];
    }
    return false;
}



template <typename T>
bool AVLtree<T>::add(T dato)
{
    AVLnodo<T> **p;
    if(find (dato,p))
        return false;
    *p=new AVLnodo<T>(dato);

    return true;
}

template <typename T>
AVLtree<T> & AVLtree<T>:: operator <<(T dato)
{
    add(dato);
    return *this;
}



template <typename T>
void AVLtree<T>::printIn(AVLnodo<T> * nodo)
{
    if(!nodo) return;
    printIn(nodo->hijos[0]);
    cout<<nodo->m_dato<<" ";
    printIn(nodo->hijos[1]);

}

template <typename T>
void AVLtree<T>::printPos(AVLnodo<T> * nodo)
{
    if(!nodo) return;
    printPos(nodo->hijos[0]);
    printPos(nodo->hijos[1]);
    cout<<nodo->m_dato<<" ";
}

template <typename T>
void AVLtree<T>::printPre(AVLnodo<T> * nodo)
{
    if(!nodo) return;
    cout<<nodo->m_dato;
cout<<endl;
    if(nodo->padre)
        cout<<"padre :"<<nodo->padre->m_dato<<endl;
    cout<<"hijos : ";
    if(nodo->hijos[0])
        cout<<" i "<<nodo->hijos[0]->m_dato<<"  ";
    if(nodo->hijos[1])
    cout<<" d "<<nodo->hijos[1]->m_dato;
    cout<<endl;

    printPre(nodo->hijos[0]);
    printPre(nodo->hijos[1]);
}
template<typename T>
void AVLtree<T>::balancear(AVLnodo<T> * nodo)
{
    int cont=0;
    while(nodo)
    {
        if(nodo->hijos[0] && nodo->hijos[1])
            cont=0;
        if(nodo->hijos[0]==nullptr || nodo->hijos[1]==nullptr)
            cont++;
        if(cont > 2 )
        {
            AVLnodo<T> ** p_padre;
            if(nodo->padre==nullptr)
                p_padre=&m_head;
            else
            {
                if(nodo->m_dato > nodo->padre->m_dato)
                *p_padre=nodo->padre->hijos[1];

                if(nodo->m_dato < nodo->padre->m_dato)
                *p_padre=nodo->padre->hijos[0];
            }

            // si es lineal

            if( (nodo->hijos[0] && nodo->hijos[0]->hijos[0])|| (nodo->hijos[1] &&  nodo->hijos[1]->hijos[1]) )
            {
                *p_padre=nodo->hijos[nodo->hijos[1]!=nullptr];
                if((*p_padre)->hijos[0])
                {
                    (*p_padre)->hijos[1]=nodo;
                    nodo->padre=*p_padre;
                    nodo->hijos[0]=nullptr;
                    return;
                }

                (*p_padre)->hijos[0]=nodo;
                nodo->padre=*p_padre;
                nodo->hijos[1]=nullptr;
                return;
            }

            // si es en zig zag


            AVLnodo<T> *temp=nodo->hijos[nodo->hijos[1]!=nullptr];
            (*p_padre)=temp->hijos[temp->hijos[1] != nullptr];

            // Lado izquierdo

            if(nodo->hijos[0])
            {
                (*p_padre)->hijos[0]=nodo->hijos[0];
                (*p_padre)->hijos[0]->hijos[1]=nullptr;
                (*p_padre)->hijos[0]->padre=*p_padre;

                (*p_padre)->hijos[1]=nodo;
                (*p_padre)->hijos[1]->hijos[0]=nullptr;
                (*p_padre)->hijos[1]->padre=*p_padre;
                m_head->padre=nullptr; // nuevo padre
                return;
            }

            // lado derecho

            if(nodo->hijos[1])
            {
                (*p_padre)->hijos[1]=nodo->hijos[1];
                (*p_padre)->hijos[1]->hijos[0]=nullptr;
                (*p_padre)->hijos[1]->padre=*p_padre;

                (*p_padre)->hijos[0]=nodo;
                (*p_padre)->hijos[0]->hijos[1]=nullptr;
                (*p_padre)->hijos[0]->padre=*p_padre;
                m_head->padre=nullptr; // nuevo padre
            }
        }
        m_head->padre=nullptr;
        nodo=nodo->padre;
    }
    return;
}
