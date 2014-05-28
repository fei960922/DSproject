/** @file */
#ifndef __DEQUE_H
#define __DEQUE_H

#include "ElementNotExist.h"
#include "IndexOutOfBound.h"
/*
    Author: Jerry Xu;
    Number: 5130309056;
    Class:  Data Struct---2014;
    Version:
        0.1     2014/5/3    Finish without iterator;
        0.2     2014/5/5    Finish without Compile;
        1.0     2014/5/19   Pass TA Test;
        1.1     2014/5/27   Bug fixed; Pass All Test;
*/
/**
 * An deque is a linear collection that supports element insertion and removal at both ends.
 * The name deque is short for "double ended queue" and is usually pronounced "deck".
 * Remember: all functions but "contains" and "clear" should be finished in O(1) time.
 *
 * You need to implement both iterators in proper sequential order and ones in reverse sequential order.
 */
template <class T>
class Deque
{
	int sz,capacity;
	T *data;
	int head,tail;
	void DoubleArray(){
        T *temp;
        temp = new T[capacity*2];
        if (head>tail) {
        	for (int i=head;i<capacity;i++) temp[i+capacity] = data[i];
        	for (int i=0;i<=tail;i++) temp[i] = data[i];
        	head += capacity;
        } else
    		for (int i=head;i<=tail;i++) temp[i] = data[i];
        delete [] data;
        data = temp;
        capacity *= 2;
    }
    inline int dec(int x) {return (x+capacity-1)%capacity;}
    inline int inc(int x) {return (x+1)%capacity;}
public:
    class Iterator
    {
    	int idx,de,removed;
    	Deque *dq;
    public:
    	Iterator(int x,Deque *xd = 0){
    	    de = x;dq = xd; removed = 1;
    	    idx = (de)?(dq->dec(dq->head)):(dq->tail);
        }
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {return (de)?(idx!=dq->dec(dq->tail)):(idx!=dq->head);}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
            removed = 1;
        	if (de) {
	        	if (idx==dq->dec(dq->tail)) throw ElementNotExist();
	        	return dq->data[idx = dq->inc(idx)];
	        } else {
	        	if (idx==dq->head) throw ElementNotExist();
	        	return dq->data[idx = dq->dec(idx)];
	        }
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
        	if (dq->sz && removed){
                bool ok = (idx>=dq->head);
        		if (ok) {
        			for (int i=idx;i>dq->head;i--) dq->data[i] = dq->data[i-1];dq->head++;
        		} else {for (int i=idx;i<dq->tail-1;i++) dq->data[i] = dq->data[i+1];dq->tail--;}
        		if (de!=ok) idx = (de)?(dq->dec(idx)):(dq->inc(idx));
        		dq->sz--;   removed = 0;
        	} else throw ElementNotExist();
        }
    };

    /**
     * TODO Constructs an empty deque.
     */
    Deque() {sz = head = tail = 0;	data = new T[capacity = 4];}

    /**
     * TODO Destructor
     */
    ~Deque() {delete [] data;}

    /**
     * TODO Assignment operator
     */
    Deque& operator=(const Deque& x) {
    	capacity = x.capacity;  sz = x.sz;
    	head = x.head;			tail = x.tail;
        delete [] data;	data = new T[capacity];
        for (int i=0;i<capacity;i++) data[i] = x.data[i];
    }

    /**
     * TODO Copy-constructor
     */
    Deque(const Deque& x) {
    	capacity = x.capacity;  sz = x.sz;
    	head = x.head;			tail = x.tail;
        data = new T[capacity];
        for (int i=0;i<capacity;i++) data[i] = x.data[i];
    }

	/**
	 * TODO Inserts the specified element at the front of this deque.
	 */
	void addFirst(const T& e) {
        if (sz == capacity-1) DoubleArray();
		data[head = dec(head)] = e; sz++;
	}

	/**
	 * TODO Inserts the specified element at the end of this deque.
	 */
	void addLast(const T& e) {
        if (sz == capacity-1) DoubleArray();
        data[tail] = e; sz++;
        tail = inc(tail);
	}

	/**
	 * TODO Returns true if this deque contains the specified element.
	 */
	bool contains(const T& e) const {
		if (sz==0) return false;
		if (head>tail) {
			for (int i=head;i<capacity;i++) if (data[i]==e) return true;
			for (int i=0;i<tail;i++) if (data[i]==e) return true;
		} else
			for (int i=head;i<tail;i++) if (data[i]==e) return true;
		return false;
	}

	/**
	 * TODO Removes all of the elements from this deque.
	 */
	 void clear() {head = 0;	tail = 0;	sz = 0;}

	 /**
	  * TODO Returns true if this deque contains no elements.
	  */
	bool isEmpty() const {return (sz==0);}

	/**
	 * TODO Retrieves, but does not remove, the first element of this deque.
	 * @throw ElementNotExist
	 */
	 const T& getFirst() {
	 	if (sz) return data[head];
	 	else throw ElementNotExist();
	 }

	 /**
	  * TODO Retrieves, but does not remove, the last element of this deque.
	  * @throw ElementNotExist
	  */
	 const T& getLast() {
	 	if (sz) return data[(tail-1)%capacity];
	 	else throw ElementNotExist();
	 }

	 /**
	  * TODO Removes the first element of this deque.
	  * @throw ElementNotExist
	  */
	void removeFirst() {
		if (sz--) head = inc(head);
		else throw ElementNotExist();
	}

	/**
	 * TODO Removes the last element of this deque.
	 * @throw ElementNotExist
	 */
	void removeLast() {
		if (sz--) tail = dec(tail);
		else throw ElementNotExist();
	}

	/**
	 * TODO Returns a const reference to the element at the specified position in this deque.
	 * The index is zero-based, with range [0, size).
	 * @throw IndexOutOfBound
	 */
	const T& get(int index) const {
		if ((index<0)||(index>=sz)) throw IndexOutOfBound();
		return data[(index+head)%capacity];
	}

	/**
	 * TODO Replaces the element at the specified position in this deque with the specified element.
	 * The index is zero-based, with range [0, size).
	 * @throw IndexOutOfBound
	 */
	void set(int index, const T& e) {
		if ((index<0)||(index>=sz)) throw IndexOutOfBound();
		data[(index+head)%capacity] = e;
	}

	/**
	 * TODO Returns the number of elements in this deque.
	 */
	 int size() const {return sz;}

	 /**
	  * TODO Returns an iterator over the elements in this deque in proper sequence.
	  */
	 Iterator iterator() {return Iterator(1,this);}

	 /**
	  * TODO Returns an iterator over the elements in this deque in reverse sequential order.
	  */
	 Iterator descendingIterator() {return Iterator(0,this);}
/*
	 void print(){
        std::cout<<"Size:"<<sz<<" Capacity:"<<capacity<<" Head:"<<head<<" Tail:"<<tail<<'\n';
        if (tail>=head)  for (int i=head;i<tail;i++) std::cout<<"<"<<data[i]<<"> ";
        else {for (int i=head;i<capacity;i++) std::cout<<"<"<<data[i]<<"> ";
            for (int i=0;i<tail;i++) std::cout<<"<"<<data[i]<<"> ";}
        std:: cout<<"\nFinished!\n";
	 }*/
};

#endif
