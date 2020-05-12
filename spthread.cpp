#include "spthread.h"
#include <string>
using namespace std;
/*
 * Constructor: spthread
 * Usage: spthread thread;
 * -------------------------------
 * Initialize a new empty spthread.
 */
spthread::spthread()
{
}

/*
 * Constructor: spthread
 * Usage: spthread thread = spthread(user, write, read);
 * -----------------------------------------------------
 * Initialize a new spthread with one thread for a user.
 */
spthread::spthread(string user, bool write, bool read) {
    lock *rw_lock = new lock;
    rw_lock -> is_rd = read;
    rw_lock -> is_wt = write;
    user_map.add(user, rw_lock);
}

/*
 * Method: init_lock
 * Usage: init_lock(user);
 * ----------------------
 * Initialize a new thread for a user.
 */
int spthread::init_lock(string user) {
    if (user_map.keys().contains(user)) {
        error("The thread exists already.");
    } else {
        lock *rw_lock = new lock;
        rw_lock -> is_rd = false;
        rw_lock -> is_wt = false;
        user_map.add(user, rw_lock);
    }
    return 1;
}

/*
 * Method: rdlock
 * Usage: rdlock(user);
 * ----------------------
 * Add reader lock to a user's thread.
 */
int spthread::rdlock(string user) {
    if (user_map.keys().contains(user)) {
        if (!global_wt) {
            global_rd = true;
            user_map.get(user) -> is_rd = true;
            return 1;
        } else {
            error("Cannot add read lock, since write lock exists.");
        }
    } else {
        init_lock(user);
        return rdlock(user);
    }
}

/*
 * Method: wtlock
 * Usage: wtlock(user);
 * ----------------------
 * Add writer lock to a user's thread.
 */
int spthread::wrlock(string user){
    if (user_map.keys().contains(user)){
        if ((!global_rd) && (!global_wt)) {
            global_wt = true;
            user_map.get(user)->is_wt = true;
            return 1;
        } else {
            error("Cannot add write lock, since there is write or read lock.");
        }
    } else {
        init_lock(user);
        return  wrlock(user);
    }
}

/*
 * Method: unlock
 * Usage: unlock(user);
 * ----------------------
 * Release lock for the user's thread.
 */
int spthread::unlock(string user){
    if (user_map.get(user)->is_wt) {
        global_wt = false;
        user_map.get(user)->is_wt = false;
        return 1;
    } else if(user_map.get(user)->is_rd) {
        int num_rdlock = 0;
        for (lock * rwlock : user_map.values()) {
            if (rwlock->is_rd) num_rdlock += 1;
        }
        global_rd = !(num_rdlock == 1);
        user_map.get(user)->is_rd = false;
        return 1;
    } else {
        return 1;
    }
}
