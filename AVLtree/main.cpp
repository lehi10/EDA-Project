#include <iostream>
#include "AVLtree.h"
using namespace std;

int main()
{
    AVLtree<int> asd;
    asd.insert(10);
    asd.insert(20);
    asd.insert(30);


    asd.printPre(asd.m_head);
    return 0;
}

