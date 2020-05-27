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
    string init_lock(string user);
    string rdlock(string user);
    string wrlock(string user);
    string unlock(string user);
    bool has_wr(string user);
    bool has_rd(string user);

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
