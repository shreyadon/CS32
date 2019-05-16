// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.


#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED

#include "provided.h"
template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap();
    ~MyMap();
    void clear();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
    
private:
    struct Node
    {
        Node(KeyType key,ValueType value)
        {
            m_key = key;
            m_value = value;
            left = right = nullptr;
        }
        KeyType m_key;
        ValueType m_value;
        Node *left, *right;
    };
    Node *m_rootPtr;
    int m_size;
    void freeTree(Node* cur);
    //    ValueType* preOrder(node* rootPtr, const KeyType& key);
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
{
    m_rootPtr = nullptr;
    m_size = 0;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap()
{
    clear();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
    freeTree(m_rootPtr);
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const
{
    return m_size;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    if (find(key) == nullptr) // if no association of value to the key, attempt to insert new value
    {
        if (m_rootPtr == nullptr) // no tree exists prior to this insertion
        {
            // make the root pointer point to a new node with the given key and value
            m_rootPtr = new Node(key, value);
            m_size++;
            return;
        }
        Node *cur = m_rootPtr;
        for (;;) // the loop runs until the value can not be inserted i.e. the value already exists in the tree or the value is successfully inserted at an appropriate place.
        {
            if (key == cur->m_key) // key already exists mapped to the value at the cur node(root pointer for the first iteration of the for loop) so can't insert again
                return;
            if (key < cur->m_key) // if the key is less than the key that is at the cur node
            {
                if (cur->left != nullptr) // some value exists on the left so execute the loop again after changing the cur
                    cur = cur->left;
                else // no value exists here so create a new node to the left of the cur node
                {
                    cur->left = new Node(key, value);
                    m_size++;
                    return;
                }
            }
            else if (key > cur->m_key) // if the key is greater than the key that is at the cur node
            {
                if(cur->right != nullptr) // some value exists on the right so execute the loop again after changing the cur
                    cur = cur->right;
                else // no value exists here so create a new node to the right of the cur node
                {
                    cur->right = new Node(key, value);
                    m_size++;
                    return;
                }
            }
        }
    }
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>:: find(const KeyType& key) const
{
    Node *cur = m_rootPtr;
    while (cur != nullptr) // run till you find the value at the specified key or you can not find it because the key does not exist in the tree
    {
        if (key == cur->m_key) // the key of the cur is the key you are looking for then return the value at the key
            return &cur->m_value;
        else if (key < cur->m_key) // if the key is less than the key that is at the cur node so execute the loop again after changing the cur node to the one on the left
            cur = cur->left;
        else // if the key is greater than the key that is at the cur node so execute the loop again after changing the cur node to the one on the right
            cur = cur->right;
    }
    return nullptr; // no matching key exists in the map
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::freeTree(Node* cur)
{
    if (cur == nullptr) // if the tree is already empty
        return;
    freeTree(cur->left); // free the left side of the tree
    freeTree(cur->right); // free the right side of the tree
    delete cur;
}



#endif // MYMAP_INCLUDED


//#ifndef MYMAP_INCLUDED
//#define MYMAP_INCLUDED
//
//#include <map>  // YOU MUST NOT USE THIS HEADER IN CODE YOU TURN IN
//
//// In accordance with the spec, YOU MUST NOT TURN IN THIS CLASS TEMPLATE,
//// since you are not allowed to use any STL associative containers, and
//// this implementation uses std::map.
//
//// This code is deliberately obfuscated.
//
//// If you can not get your own MyMap class template working, you may use
//// this one during development in order to let you proceed with implementing
//// the other classes for this project; you can then go back to working on
//// fixing your own MyMap class template.
//
//template <typename KeyType, typename ValueType>
//class MyMap
//{
//public:
//    MyMap(const MyMap &other) = delete;
//    MyMap &operator=(const MyMap &other) = delete;
//    typedef KeyType O;using O1=int;void clear(){l01.clear();}
//    using l0=ValueType;MyMap(){}using l10=O const;O1 size()
//    const{return l01.size();}using ll0=l0 const;using Const=
//    MyMap<O,l0>;void associate(l10&Using,ll0&first){l01[
//                                                        Using]=first;}using l1=std::map<O,l0>;using l00=Const
//    const;ll0*find(l10&l11)const{auto first=l01.find(l11);
//        return(first!=l01.end()?&first->second:0);}l0*find(l10&
//                                                           l01){return(l0*)(*(l00*)(this)).find(l01);}private:l1
//    l01;
//};
//
//#endif // MYMAP_INCLUDED
