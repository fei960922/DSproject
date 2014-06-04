/** @file */
#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"
/*
    Author: Jerry Xu;
    Number: 5130309056;
    Class:  Data Struct---2014;
    Version:
        0.1     2014/5/1    Finish without iterator;
        0.2     2014/5/4    Finish without Compile;
        1.0     2014/5/18   Pass TA Test;
        1.1     2014/5/23   Bug fixed, memory leak fixed, Pass all Test;
        2.0     2014/6/2    Final Version --- AC;
*/
/**
 * The ArrayList is just like vector in C++.
 * You should know that "capacity" here doesn't mean how many elements are now in this list, where it means
 * the length of the array of your internal implemention
 *
 * The iterator iterates in the order of the elements being loaded into this list
 */
template <class T>
class ArrayList
{
public:
    int sz,capacity;
    T *data;
    void DoubleArray(){
        capacity *= 2;
        T *temp;
        temp = new T[capacity];
        for (int i=0;i<sz;i++) temp[i] = data[i];
        delete [] data;
        data = temp;
    }
    class Iterator
    {
        int idx,removed;
        ArrayList * al;
    public:
        Iterator(ArrayList *x = 0){idx = -1;al = x;removed=0;}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {return (idx!=al->sz-1);}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
            if (idx==al->sz-1) throw ElementNotExist();
            else {idx++;removed=0;return al->data[idx];}
        }

        /**
         * TODO Removes from the underlying collection the last element
         * returned by the iterator
         * The behavior of an iterator is unspecified if the underlying
         * collection is modified while the iteration is in progress in
         * any way other than by calling this method.
         * @throw ElementNotExist
         */
        void remove() {
            if ((al->sz==0)||(idx==-1)||(removed==1)) throw ElementNotExist();
            al->sz--;   idx--;  removed = 1;
            for (int i=idx+2;i<=al->sz;i++) al->data[i-1] = al->data[i];
        }
    };

    /**
     * TODO Constructs an empty array list.
     */
    ArrayList() {sz = 0;  data = new T[capacity = 1];}

    /**
     * TODO Destructor
     */
    ~ArrayList() {delete [] data;}

    /**
     * TODO Assignment operator
     */
    ArrayList& operator=(const ArrayList& x) {
        capacity = x.capacity;  sz = x.sz;
        T *temp = new T[capacity];
        for (int i=0;i<sz;i++) temp[i] = x.data[i];
        delete [] data; data = temp;
    }

    /**
     * TODO Copy-constructor
     */
    ArrayList(const ArrayList& x) {
        capacity = x.capacity;  sz = x.sz;
        data = new T[capacity];
        for (int i=0;i<sz;i++) data[i] = x.data[i];
    }

    /**
     * TODO Appends the specified element to the end of this list.
     * Always returns true.
     */
    bool add(const T& e) {
        if (sz==capacity) DoubleArray();
        data[sz] = e; sz++; return true;
    }

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=sz means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element) {
        if (sz==capacity) DoubleArray();
        if ((index<0)||(index>sz)) throw IndexOutOfBound();
        for (int i=sz;i>index;i--) data[i] = data[i-1];
        data[index] = element;  sz++;
    }

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear() {sz = 0;}

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const {
        for (int i=0;i<sz;i++) if (e==data[i]) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {
        if ((index<0)||(index>=sz)) throw IndexOutOfBound();
        return data[index];
    }

    /**
     * TODO Returns true if this list contains no elements.
     */
    bool isEmpty() const {return (sz==0);}

    /**
     * TODO Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void removeIndex(int index) {
        if ((index<0)||(index>=sz)) throw IndexOutOfBound();
        for (int i=index+1;i<sz;i++) data[i-1] = data[i];
        sz--;
    }

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    bool remove(const T &e) {
        int idx = sz;
        for (int i=0;i<sz;i++) if (data[i]==e) {idx=i;break;}
        if (idx!=sz){
            for (int i=idx+1;i<sz;i++) data[i-1] = data[i];
            sz--; return true;}
        return false;
    }

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element) {
        if ((index<0)||(index>=sz)) throw IndexOutOfBound();
        data[index] = element;
    }

    /**
     * TODO Returns the number of elements in this list.
     */
    int size() const {return sz;}

    /**
     * TODO Returns an iterator over the elements in this list.
     */
    Iterator iterator() {return Iterator(this);}
};

#endif
