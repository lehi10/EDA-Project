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
            cout<<"Insertado ! "<<endl;
        }
        if(opt=='I')
        {
            int value, iTime;
            cout<<"Inserte un tiempo  y el Valor : ";
            cin>>iTime>>value;
            pqueue.insertInTime(iTime,value);
            cout<<"Insertado ! "<<endl;
        }
        if(opt=='D')
        {
            int delTime;
            cout<<"Eliminar en tiempo : ";
            cin>>delTime;
            pqueue.deleteMinInTime(delTime);
            cout<<"Eliminado ! "<<endl;
        }
        pqueue.imprimir();

    }

    return 0;
}

