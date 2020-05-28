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
 * Destructor: ~spthread
 * Usage: ~spthread();
 * -----------------------------------------------------
 * Destruct spthread.
 */
spthread::~spthread(){
    user_map.clear();
    rd_queue.clear();
    wt_queue.clear();
}

/*
 * Method: init_lock
 * Usage: init_lock(user);
 * ----------------------
 * Initialize a new thread for a user.
 */
string spthread::init_lock(string user) {
    if (user_map.keys().contains(user)) {
        error("The thread exists already.");
    } else {
        lock *rw_lock = new lock;
        rw_lock -> is_rd = false;
        rw_lock -> is_wt = false;
        user_map.add(user, rw_lock);
    }
    string result = "Initialize a thread for ";
    result.append(user);
    result.append("\n");

    return result;
}

/*
 * Method: rdlock
 * Usage: rdlock(user);
 * ----------------------
 * Add reader lock to a user's thread.
 */
string spthread::rdlock(string user) {
    if (user_map.keys().contains(user)) {
        if (wt_queue.isEmpty()) {
            if (!global_wt) {
                global_rd = true;
                user_map.get(user) -> is_rd = true;
                string result = "Successfully assign a reader lock for ";
                result.append(user);
                result.append("\n");
                return result;
            } else if (!user_map.get(user)->is_wt){
                rd_queue.enqueue(user);
                string result = "Cannot add read lock for ";
                result.append(user);
                result.append(", since write lock exists. Already put into waiting line.");
                result.append("\n");
                return result;
            } else {
                global_wt = false;
                user_map.get(user) -> is_wt = false;
                global_rd = true;
                user_map.get(user) -> is_rd = true;
                string result = "Successfully back writer lock for ";
                result.append(user);
                result.append(" to reader lock.");
                result.append("\n");
                return result;
            }
        } else {
            rd_queue.enqueue(user);
            string result = "Cannot add read lock for ";
            result.append(user);
            result.append(", since write lock aquirement exists. Already put into waiting line.");
            result.append("\n");
            return result;
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
string spthread::wrlock(string user) {
    if (user_map.keys().contains(user)){
        if ((!global_rd) && (!global_wt)) {
            global_wt = true;
            user_map.get(user) -> is_wt = true;
            user_map.get(user) -> is_rd = false;
            string result ="Successfully assign a writer lock for ";
            result.append(user);
            result.append("\n");
            return result;

        } else {
            wt_queue.enqueue(user);
            string result ="Cannot add write lock for ";
            result.append(user);
            result.append(", since there is write or read lock. Already put into waiting line.");
            result.append("\n");
            return result;
        }
    } else {
        init_lock(user);
        return wrlock(user);
    }
}

/*
 * Method: unlock
 * Usage: unlock(user);
 * ----------------------
 * Release lock for the user's thread.
 */
string spthread::unlock(string user) {
    if (user_map.get(user)->is_wt) {
        global_wt = false;
        user_map.get(user)->is_wt = false;
        string result = "Successfully release a writer lock for ";
        result.append(user);
        if (!wt_queue.isEmpty() && !global_rd) {
            wrlock(wt_queue.dequeue());
        } else if (wt_queue.isEmpty() && !rd_queue.isEmpty()) {
            while (!rd_queue.isEmpty()) {
                result.append("\n");
                result.append(rdlock(rd_queue.dequeue()));
            }
        }
        result.append("\n");
        return result;

    } else if(user_map.get(user)->is_rd) {
        int num_rdlock = 0;
        for (lock * rwlock : user_map.values()) {
            if (rwlock->is_rd) num_rdlock += 1;
        }
        global_rd = !(num_rdlock == 1);
        user_map.get(user)->is_rd = false;
        string result = "Successfully release a reader lock for ";
        result.append(user);
        if (!global_rd && !global_wt && !wt_queue.isEmpty()) {
            result.append("\n");
            result.append(wrlock(wt_queue.dequeue()));
        }
        return result;
    } else {
        return "";
    }
}

/*
 * Method: has_wr
 * Usage: hhas_wr(user);
 * ----------------------
 * Check whether user has writer lock.
 */
bool spthread::has_wr(string user){
    return user_map.get(user)->is_wt;
}

/*
 * Method: has_rd
 * Usage: hhas_rd(user);
 * ----------------------
 * Check whether user has reader lock.
 */
bool spthread::has_rd(string user){
    return user_map.get(user)->is_rd;
}

