#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

void foo1(mutex& mtx)
{
    int i = 0;
    while (i < 3)
    {
        this_thread::sleep_for(chrono::milliseconds(2000));
        lock_guard<mutex> lg(mtx);
        cout << "FOO1" << endl;
        i++;
        this_thread::sleep_for(chrono::milliseconds(2000));
    }
}

void foo2(mutex& mtx)
{
    int i = 0;
    while (i < 3)
    {
        this_thread::sleep_for(chrono::milliseconds(2000));
        lock_guard<mutex> lg(mtx);
        cout << "FOO2" << endl;
        i++;
        this_thread::sleep_for(chrono::milliseconds(2000));
    }
}

void foo3(mutex& smtx, timed_mutex& tmtx)
{
    unique_lock<mutex> ul(smtx, std::defer_lock);

    ul.lock();
    cout << "THREAD WITH ID: " << this_thread::get_id() << " IS STARTING WORK!" << endl;
    ul.unlock();
    if (tmtx.try_lock_until(std::chrono::steady_clock::now() + std::chrono::seconds(20)))
    {
        this_thread::sleep_for(chrono::milliseconds(10000));
        cout << "FOO3 has been called by the thread with id: " << this_thread::get_id() << endl;
        this_thread::sleep_for(chrono::milliseconds(11000));
        tmtx.unlock();
    }
    else
    {
        cout << "TIME IS OVER! THREAD CAN'T WAIT ANY LONGER!" << endl;
    }
}

int main()
{
    mutex smtx;
    timed_mutex tmtx;

    

    thread th1(foo3, ref(smtx), ref(tmtx));
    thread th2(foo3, ref(smtx), ref(tmtx));



    th1.join();
    th2.join();
    return 0;
}