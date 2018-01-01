#include "avltree.h"
#include "priorityqueueretro.h"
#include <thread>




void print_thread(PrioQueueRetro pqueue)
{
    pqueue.imprimir();
}

int main()
{

PrioQueueRetro pqueue;

char opt='1';
cout<<"Priority Queue :"<<endl;
while(opt!='0')
{
    cout<<"Opción : ";
    cin>>opt;
    if(opt=='i')
    {
        int value;
        cout<<"Inserte un valor a la Priority Queue : ";
        cin>>value;
        pqueue.insertNow(value);
        pqueue.imprimir();
        cout<<"Insertado ! "<<endl;
    }
    if(opt=='I')
    {
        int value,delTime;
        cout<<"Inserte un tiempo  y el Valor : ";
        cin>>delTime>>value;
        pqueue.insertInTime(delTime,value);
        pqueue.imprimir();
        cout<<"Insertado ! "<<endl;
    }

}



    pqueue.imprimir();

    cout<<"nodo : "<<pqueue.updateTree->find(13)->key<<endl;

  return 0;
}
