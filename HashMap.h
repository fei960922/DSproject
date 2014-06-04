/** @file */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "ElementNotExist.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

/*
    Author: Jerry Xu;
    Number: 5130309056;
    Class:  Data Struct---2014;
    Version:
        0.1     2014/5/2    Finish without Iterator;
        0.2     2014/5/5    Finish without Compile;
        1.0     2014/5/19   Pass TA Test;
        1.1     2014/5/25   Bug fixed; Pass all Test;
        1.2     2014/6/4    Bug fixed;
*/
/**
 * HashMap is a map implemented by hashing. Also, the 'capacity' here means the
 * number of buckets in your internal implemention, not the current number of the
 * elements.
 *
 * Template argument H are used to specify the hash function.
 * H should be a class with a static function named ``hashCode'',
 * which takes a parameter of type K and returns a value of type int.
 * For example, the following class
 * @code
 *      class Hashint {
 *      public:
 *          static int hashCode(int obj) {
 *              return obj;
 *          }
 *      };
 * @endcode
 * specifies an hash function for integers. Then we can define:
 * @code
 *      HashMap<int, int, Hashint> hash;
 * @endcode
 *
 * Hash function passed to this class should observe the following rule: if two keys
 * are equal (which means key1 == key2), then the hash code of them should be the
 * same. However, it is not generally required that the hash function should work in
 * the other direction: if the hash code of two keys are equal, the two keys could be
 * different.
 *
 * Note that the correctness of HashMap should not rely on the choice of hash function.
 * This is to say that, even the given hash function always returns the same hash code
 * for all keys (thus causing a serious collision), methods of HashMap should still
 * function correctly, though the performance will be poor in this case.
 *
 * The order of iteration could be arbitary in HashMap. But it should be guaranteed
 * that each (key, value) pair be iterated exactly once.
 */
template <class K, class V, class H>
class HashMap
{
public:
    class Entry
    {
    public:
        K key;
        V value;
        Entry *next;
        Entry(K k = 0, V v = 0) {key = k;value = v;next = NULL;}
        const K& getKey() const {return key;}
        const V& getValue() const {return value;}
    };
private:
    int sz,capacity,maxhash;
    Entry **hash;
    inline int pos (K k) const {return (std::abs(H::hashCode(k))%capacity);}
    void print(char s = 'A') const {
        std::cout<<s<<" I_PRINT size="<<sz<<" maxhash="<<maxhash<<"\n";
        for (int i=0;i<capacity;i++)
            if (hash[i]==NULL) std::cout<<i<<hash[0]<<" : NULL\n";
            else {std::cout<<i<<" : ";Entry *idx = hash[i];
                while (idx) {std::cout<<" -> "<<idx->getKey();idx = idx->next;}
                std::cout<<"\n";}
        std::cout<<"FINISH\n";
    }   
    void I_put(K k,V v,Entry **h){
        Entry *temp = new Entry(k,v);
        int t = pos(k);
        temp->next = h[t];  h[t] = temp;
        if (t>maxhash) maxhash = t;
    }
    void I_clear(Entry **h,int ca){
        Entry *idx;
        for (int i=0;i<ca;i++) {
            idx = h[i];
            while (idx!=NULL) {
                Entry *temp = idx->next;
                delete idx;    idx = temp;
            }
            h[i] = NULL;
        }
    }
    Entry **I_copy(int c1,int c2,Entry **h1,int del = 0){
        Entry **thash = new Entry*[c2];
        for (int i=0;i<c2;i++) thash[i] = NULL;
        for (int i=0;i<c1;i++)
            for (Entry *idx = h1[i];idx!=NULL;idx = idx->next){
                I_put(idx->key,idx->value,thash);
            }
        if (del) {I_clear(h1,capacity/2);delete [] h1;}
        return thash;
    }
public:
    class Iterator
    {
        Entry *idx;
        int idxhash;
        const HashMap *hm;
    public:
        Iterator(const HashMap *x = 0){
            idxhash = (x->sz)?-1:-2;
            hm = x;idx = NULL;
        }

        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {return ((idxhash!=-2)&&((idxhash<hm->maxhash)||(idx->next)));}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
            if ((idxhash==hm->maxhash)&&(idx->next==NULL)) throw ElementNotExist();
            Entry *temp = idx;
            if ((idx)&&(idx->next)) idx = idx->next;
            else {
                idxhash++;
                while (hm->hash[idxhash]==NULL) idxhash++;
                idx = hm->hash[idxhash];
            }
            return *idx;
        }
    };

    /**
     * TODO Constructs an empty hash map.
     */
    HashMap() {
        sz = 0;    capacity = 11;   maxhash = -1;
        hash = new Entry*[capacity];
        for (int i=0;i<capacity;i++) hash[i] = NULL;
    }

    /**
     * TODO Destructor
     */
    ~HashMap() {I_clear(hash,capacity); delete [] hash;}

    /**
     * TODO Assignment operator
     */
    HashMap &operator=(const HashMap &x) {
        capacity = x.capacity;  sz = x.sz;  maxhash = x.maxhash;
        Entry **temp = I_copy(capacity,capacity,x.hash);
        I_clear(hash,capacity); delete [] hash;
        hash = temp;
    }

    /**
     * TODO Copy-constructor
     */
    HashMap(const HashMap &x) {
        capacity = x.capacity;  sz = x.sz;  maxhash = x.maxhash;
        hash = I_copy(capacity,capacity,x.hash);
    }

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator() const {return Iterator(this);}

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear() {I_clear(hash,capacity);   sz = 0;}

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const {
        for (Entry *idx = hash[pos(key)];idx!=NULL;idx = idx->next)
            if (idx->key==key) return true;
        return false;
    }

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const {
        for (int i=0;i<capacity;i++)
            for (Entry *idx = hash[i];idx!=NULL;idx = idx->next)
                if (idx->value==value) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const {
        for (Entry *idx = hash[pos(key)];idx!=NULL;idx = idx->next)
            if (idx->key==key) return idx->value;
        throw ElementNotExist();
    }

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const {return (sz==0);}
    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value) {   
        for (Entry *idx = hash[pos(key)];idx!=NULL;idx = idx->next)
            if (idx->key==key) {idx->value = value;return;}
        if (sz*2>=capacity) {
            capacity = capacity *2 +1;
            hash = I_copy(capacity/2,capacity,hash,1);
        }
        I_put(key,value,hash);  sz++;
    }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) {
        int p = pos(key);
        if (!hash[p]) throw ElementNotExist();
        if (hash[p]->key==key) {
            Entry *temp = hash[p]->next; delete hash[p];
            hash[p] = temp;  sz--;
            if ((maxhash==p)&&(temp==NULL)){
                maxhash--;
                while ((hash[maxhash]==NULL)&&(maxhash>0)) maxhash--;
                if (maxhash==0 && hash[maxhash]==NULL) maxhash = -1;
            }
            return;
        }
        for (Entry *idx = hash[p];idx->next!=NULL;idx = idx->next)
            if (idx->next->key==key) {
                Entry *temp = idx->next;
                idx->next = temp->next;
                delete temp;    sz--;    return;
            }
        throw ElementNotExist();
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {return sz;}
};

#endif
