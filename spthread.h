#ifndef SPTHREAD_H
#define SPTHREAD_H
#include <map.h>
#include <string>
#include <queue.h>

using namespace std;

class spthread
{
public:

    /*
     * Constructor: spthread
     * Usage: spthread thread;
     * -------------------------------
     * Initialize a new empty spthread.
     */
    spthread();

    /*
     * Destructor: ~spthread
     * Usage: ~spthread();
     * -------------------
     * Destruct spthread.
     */
    ~spthread();

    /*
     * Constructor: spthread
     * Usage: spthread thread = spthread(user, write, read);
     * -----------------------------------------------------
     * Initialize a new spthread with one thread for a user.
     */
    spthread(string user, bool write, bool read);

    /*
     * Method: init_lock
     * Usage: init_lock(user);
     * ----------------------
     * Initialize a new thread for a user.
     */
    string init_lock(string user);

    /*
     * Method: rdlock
     * Usage: rdlock(user);
     * ----------------------
     * Add reader lock to a user's thread.
     */
    string rdlock(string user);

    /*
     * Method: wtlock
     * Usage: wtlock(user);
     * ----------------------
     * Add writer lock to a user's thread.
     */
    string wrlock(string user);

    /*
     * Method: unlock
     * Usage: unlock(user);
     * ----------------------
     * Release lock for the user's thread.
     */
    string unlock(string user);

    /*
     * Method: has_wr
     * Usage: hhas_wr(user);
     * ----------------------
     * Check whether user has writer lock.
     */
    bool has_wr(string user);

    /*
     * Method: has_rd
     * Usage: hhas_rd(user);
     * ----------------------
     * Check whether user has reader lock.
     */
    bool has_rd(string user);

private:
    /*
     * Struct lock: Here, the lock has 2 data fields.
     * Reader lock and writer lock.
     */
    struct lock{
        bool is_rd;
        bool is_wt;
    };
    /*bool global writer lock*/
    bool global_wt = false;
    /*bool global reader lock*/
    bool global_rd = false;
    /*Map user map*/
    Map<string, lock*> user_map;
    /*Queue reader waiting list*/
    Queue<string> rd_queue;
    /*Queue writer waiting list*/
    Queue<string> wt_queue;

};

#endif // SPTHREAD_H
