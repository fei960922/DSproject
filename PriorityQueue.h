/** @file */
#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H

#include "ArrayList.h"
#include "ElementNotExist.h"
#include <cstdio>
/*
    Author: Jerry Xu;
    Number: 5130309056;
    Class:  Data Struct---2014;
    Version:
        0.1     2014/5/1    Finish without iterator;
        0.2     2014/5/5    Finish without iterator::remove;
        0.3     2014/5/19   Pass TA Test without (iterator::remove&AL->PQ);
        0.4     2014/5/20   Pass TA Test without iterator::remove;
*/
/**
 * This is a priority queue based on a priority priority queue. The
 * elements of the priority queue are ordered according to their
 * natual ordering (operator<), or by a Comparator provided as the
 * second template parameter.
 * The head of this queue is the least element with respect to the
 * specified ordering (different from C++ STL).
 * The iterator does not return the elements in any partidatalar order.
 * But it is required that the iterator will eventually return every
 * element in this queue (even if removals are performed).
 */

/*----------------------------------------------------------------------*/
/**
 * Default Comparator with respect to natural order (operator<).
 */
template <class V>
class Less
{
public:
    bool operator()(const V& a, const V& b) { return a < b; }
};

/**
 * To use this priority queue, users need to either use the
 * default Comparator or provide their own Comparator of this
 * kind.
 * The Comparator should be a class with a public function
 * public: bool operator()(const V&, const V&);
 * overriding the "()" operator.
 * The following code may help you understand how to use a
 * Comparator and, especially, why we override operator().
 */

// #include <iostream>
// using namespace std;
//
// template <class T, class C = Less<T> >
// class Example
// {
// private:
//     C cmp;
// public:
//     bool compare(const T& a, const T& b)
//     {
//         return cmp(a, b);
//     }
// };
//
// int main()
// {
//     Example<int, Less<int> > example; // Less<int> can be omitted.
//     cout << example.compare(1, 2) << endl;
//     cout << example.compare(2, 1) << endl;
// }

/*----------------------------------------------------------------------*/
template <class V, class C = Less<V> >
class PriorityQueue
{
    int sz,capacity;
    V *data;
    C cmp;
    void DoubleArray(){
        capacity *= 2;
        V *temp;
        temp = new V[capacity+1];
        for (int i=1;i<=sz;i++) temp[i] = data[i];
        delete [] data;
        data = temp;
    }
    void PushDown(int idx){
        while (idx*2<=sz) if ((idx*2+1<=sz)&&(cmp(data[idx*2+1],data[idx*2]))){
            if (cmp(data[idx*2+1],data[idx])){
                std::swap(data[idx],data[idx*2+1]);idx = idx*2+1;
            } else break;
        } else if (cmp(data[idx*2],data[idx])){
            std::swap(data[idx*2],data[idx]);idx = idx*2;
        } else break;
    }
public:
    class Iterator
    {
        int idx;
        int *nxt,*prv;
        PriorityQueue *pq;
    public:
        Iterator(PriorityQueue *x){
            pq = x;  idx = 0;
            nxt = new int[pq->sz+1];    prv = new int[pq->sz+1];
            nxt[0] = 10000000;
            for (int i=0;i<=pq->sz;i++) prv[i] = i-1;
            for (int i=0;i<=pq->sz-1;i++) nxt[i] = i+1;
            nxt[pq->sz] = 10000000;
        }
        ~Iterator(){delete [] nxt;}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {return (nxt[idx]<=pq->sz);}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasnext() == false
         */
        const V &next() {
            if (nxt[idx]>pq->sz) throw ElementNotExist();
            else {idx = nxt[idx];return pq->data[idx];}
        }

		/**
		 * TODO Removes from the underlying collection the last element
		 * returned by the iterator.
		 * The behavior of an iterator is unspecified if the underlying
		 * collection is modified while the iteration is in progress in
		 * any way other than by calling this method.
		 * @throw ElementNotExist
		 */
        int I_swap(int x){
            std::swap(nxt[prv[idx]],nxt[prv[x]]);std::swap(prv[idx],prv[x]);
            std::swap(prv[nxt[idx]],prv[nxt[x]]);std::swap(nxt[idx],nxt[x]);
            std::swap(pq->data[idx],pq->data[x]);return x;
        }
        void remove() {std::cout<<idx<<' ';
            if (idx==0 || pq->sz==0) throw ElementNotExist();
            nxt[0] = nxt[idx];   prv[nxt[idx]] = 0;
            pq->data[idx] = pq->data[pq->sz];
            nxt[prv[pq->sz]] = idx; prv[idx] = prv[pq->sz];    pq->sz--;
            while (idx*2<=pq->sz) if ((idx*2+1<=pq->sz)&&(pq->cmp(pq->data[idx*2+1],pq->data[idx*2]))){
                if (pq->cmp(pq->data[idx*2+1],pq->data[idx]))
                idx = I_swap(idx*2+1);  else break;
            } else if (pq->cmp(pq->data[idx*2],pq->data[idx]))
            idx = I_swap(idx*2);        else break;
            while (idx>1) if (pq->cmp(pq->data[idx],pq->data[idx/2]))
            idx = I_swap(idx/2);        else break;
            idx = 0;
        }
    };

    /**
     * TODO Constructs an empty priority queue.
     */
    PriorityQueue() {sz = 0;  data = new V[5];  capacity = 4;}

    /**
     * TODO Destructor
     */
    ~PriorityQueue() {delete [] data;}

    /**
     * TODO Asizeignment operator
     */
    PriorityQueue &operator=(const PriorityQueue &x) {
        capacity = x.capacity;  sz = x.sz;
        delete [] data; data = new V[capacity+1];
        for (int i=1;i<=sz;i++) data[i] = x.data[i];
    }

    /**
     * TODO Copy-constructor
     */
    PriorityQueue(const PriorityQueue &x) {
        capacity = x.capacity;  sz = x.sz;
        data = new V[capacity+1];
        for (int i=1;i<=sz;i++) data[i] = x.data[i];
    }

	/**
	 * TODO Initializer_list-constructor
	 * Constructs a priority queue over the elements in this Array List.
     * Requires to finish in O(n) time.
	 */
	PriorityQueue(const ArrayList<V> &x) {
        capacity = x.capacity;  sz = x.sz;
        data = new V[capacity+1];
        for (int i=0;i<x.sz;i++) data[i+1] = x.data[i];
        for (int i=sz/2;i>0;i--) PushDown(i);
    }

    /**
     * TODO Returns an iterator over the elements in this priority queue.
     */
    Iterator iterator() {return Iterator(this);}

    /**
     * TODO Removes all of the elements from this priority queue.
     */
    void clear() {sz = 0;}

    /**
     * TODO Returns a const reference to the front of the priority queue.
     * If there are no elements, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &front() const {
        if (sz==0) throw ElementNotExist();
        return data[1];
    }

    /**
     * TODO Returns true if this PriorityQueue contains no elements.
     */
    bool empty() const {return (sz==0);}

    /**
     * TODO Add an element to the priority queue.
     */
    void push(const V &value) {
        if (sz==capacity) DoubleArray();
        sz++; data[sz] = value;   int idx = sz;
        while (idx>1) if (cmp(data[idx],data[idx/2])) {
            std::swap(data[idx],data[idx/2]);
            idx = idx/2;
        } else break;
    }

    /**
     * TODO Removes the top element of this priority queue if present.
     * If there is no element, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void pop() {
        if (sz==0) throw ElementNotExist();
        data[1] = data[sz];     sz--;
        PushDown(1);
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {return sz;}
};

#endif
