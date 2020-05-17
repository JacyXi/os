#ifndef SPTHREAD_H
#define SPTHREAD_H
#include <map.h>
#include <string>
#include <queue.h>

using namespace std;

class spthread
{
public:

    spthread();
    ~spthread();
    spthread(string user, bool write, bool read);
    int init_lock(string user);
    int rdlock(string user);
    int wrlock(string user);
    int unlock(string user);

private:
    struct lock{
        bool is_rd;
        bool is_wt;
    };
    bool global_wt = false;
    bool global_rd = false;
    Map<string, lock*> user_map;
    Queue<string> rd_queue;
    Queue<string> wt_queue;

};

#endif // SPTHREAD_H
