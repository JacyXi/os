#ifndef PREDEFINED_H
#define PREDEFINED_H

#include <string.h>
#include "sPath.h"
#include "vector.h"

namespace bpt {

/* predefined B+ info */
#define BP_ORDER 20

/* key/value type */
typedef vector<sPath*> value_t;

typedef int key_t;

inline int keycmp(const key_t &a, const key_t &b) {
    if (a == b) return 0;
    else if (a > b) return 1;
    else return -1;
}

#define OPERATOR_KEYCMP(type) \
    bool operator< (const key_t &l, const type &r) {\
        return keycmp(l, r.key) < 0;\
    }\
    bool operator< (const type &l, const key_t &r) {\
        return keycmp(l.key, r) < 0;\
    }\
    bool operator== (const key_t &l, const type &r) {\
        return keycmp(l, r.key) == 0;\
    }\
    bool operator== (const type &l, const key_t &r) {\
        return keycmp(l.key, r) == 0;\
    }

}

#endif /* end of PREDEFINED_H */
