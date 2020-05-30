#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H

#include "BPlus_node.h"
#include "vector.h"
#include "sPath.h"
#include "set.h"

using namespace std;

enum COMPARE_OPERATOR{LT, LE, EQ, BE, BT, BETWEEN}; // 比较操作符：<、<=、=、>=、>、<>


struct SelectResult
{
    int keyIndex;
    CLeafNode* targetNode;
};

class CBPlusTree{
public:
    CBPlusTree();
    ~CBPlusTree();
    bool insert(KeyType key, const DataType& data);
    bool remove(KeyType key);
    bool update(KeyType oldKey, KeyType newKey);
    vector<DataType> select(KeyType compareKey, int compareOpeartor);
    vector<DataType> select(KeyType smallKey, KeyType largeKey);
    bool search(KeyType key);
    void clear();
    void print() const;

private:
    void recursive_insert(CNode* parentNode, KeyType key, const DataType& data);
    void recursive_remove(CNode* parentNode, KeyType key);
    void printInConcavo(CNode *pNode, int count)const;
    bool recursive_search(CNode *pNode, KeyType key)const;
    void changeKey(CNode *pNode, KeyType oldKey, KeyType newKey);
    void search(KeyType key, SelectResult& result);
    void recursive_search(CNode* pNode, KeyType key, SelectResult& result);
    void remove(KeyType key, DataType dataValue);
    void recursive_remove(CNode* parentNode, KeyType key, DataType dataValue);
private:
    CNode* m_Root;
    CLeafNode* m_DataHead;
    KeyType m_MaxKey;
};

#endif
