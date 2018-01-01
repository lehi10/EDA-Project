#ifndef NODOAVL_H
#define NODOAVL_H
#include <iostream>
using namespace std;

template <typename T>
class AVLnodo
{
    public:
            AVLnodo(T dato):m_dato(dato){hijos[0]=nullptr;hijos[1]=nullptr;}
            T m_dato;
            AVLnodo *hijo_d;
            AVLnodo *hijo_i;
            AVLnodo *hijos[2]={hijo_i,hijo_d};
            AVLnodo * padre;

};

#endif // NODOAVL_H
