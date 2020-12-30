#include <iostream>
#include <windows.h>
#include <thread>
#include "windows.h"
#include <string>

using namespace std;

HANDLE mut; LPCSTR str="HELLO";
DWORD Res; int Ccount(0);


void Barber()
{
    Res=WaitForSingleObject(mut,INFINITE);
    if(Res==WAIT_OBJECT_0)
    {
        cout << "BARBER works" << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
        ::Ccount--; cout << "BARBER shears" << endl;
        if(::Ccount>0){ReleaseMutex(mut);}
        else {cout << "BARBER sleeps" << endl;}
    }
    else
    {
        cout << "Wake up! BARBER" << endl;
        ReleaseMutex(mut); Barber();
    }
}

void Clients()
{
    int const Length=3; int it=0; thread people_count[Length];
    cout << "Max CLIENTs: " << Length << endl;
    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(2000));
        cout << "CLIENT enters" << endl;
        if(::Ccount+1>Length){ cout << "Enter later!" << endl; }
        else
        {
            if(it>=Length)
            {
                for(int i=it%3;i<Length;)
                {
                    if(people_count[i].joinable()){ people_count[i].join(); break; }
                }
            }
            ::Ccount++; for(int i=0; i<Length; i++)
            {
                if(!people_count[i].joinable())
                {
                    people_count[i]=thread(Barber); break;
                }
            }
        }
        it++;
    }
}


int main()
{
    mut=CreateMutex(NULL, FALSE, str);
    Clients(); CloseHandle(mut);
}
