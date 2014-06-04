/** @file */
#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"
#include <iostream>
/*
    Author: Jerry Xu;
    Number: 5130309056;
    Class:  Data Struct---2014;
    Version:
        0.1     2014/5/1    Finish half;
        0.2     2014/5/2    Finish without iterator;
        0.3     2014/5/5    Finish without Compile;
        1.0     2014/5/18   Pass TA Test;
        1.1     2014/5/23   Bug fixed, memory leak fixed, Pass all Test;
        1.2     2014/6/2    Final Version;
*/
/**
 * A linked list.
 *
 * The iterator iterates in the order of the elements being loaded into this list.
 */
template <class T>
class LinkedList
{
    struct Lnode {
        Lnode *prev;
        Lnode *next;
        T value;
        Lnode(T x){value = x;prev=NULL;next=NULL;}
        Lnode(){prev=NULL;next=NULL;}
    };
    Lnode *first,*last;
    int sz;
    Lnode *pos(int x) const {
        if (x>=sz) return NULL;
        Lnode *idx = first->next;
        while (x--) idx = idx->next;
        return idx;
    }
    void I_Delete(Lnode *idx){
        if (idx!=last) idx->next->prev = idx->prev;
        else last = idx->prev;
        if (idx!=first) idx->prev->next = idx->next;
        else first = idx->next;
        delete idx; sz--;
    }
    void I_Clear(){
        Lnode *idx = first->next;
        first->next = NULL;
        for (int i=1;i<sz;i++) {
            idx = idx->next;
            delete idx->prev;}
        delete idx; sz = 0;
        last = first;
    }
    void I_Copy(LinkedList *c) {
        sz = c->sz;
        Lnode *temp,*temp2;
        first = new Lnode;  temp2 = first;
        if (sz) for (Lnode *idx = c->first->next;idx!=NULL;idx = idx->next) {
            temp = new Lnode(idx->value);
            temp->prev = temp2; temp2->next = temp;
            temp2 = temp;
        } else temp = first;
        last = temp;
    }
public:
    class Iterator
    {
        Lnode *idx;
        LinkedList *ll;
        bool removed;
    public:
        Iterator(LinkedList *x){ll = x;idx = ll->first;removed=0;}

        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {return (idx!=ll->last);}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
            if (idx==ll->last) throw ElementNotExist();
            idx = idx->next;   removed = 0;  return (idx->value);
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
            if ((idx)&&(idx!=ll->first)&&(!removed)) {
                Lnode *temp = idx->prev;
                ll->I_Delete(idx);
                idx = temp;     removed = 1;
            } else throw ElementNotExist();
        }
    };

    /**
     * TODO Constructs an empty linked list
     */
    LinkedList() {sz = 0;first = last = new Lnode();}

    /**
     * TODO Copy constructor
     */
    LinkedList(LinkedList<T> &c) {I_Copy(&c);}

    /**
     * TODO Assignment operator
     */
    LinkedList<T>& operator=(LinkedList<T> &c) {I_Clear();delete first;I_Copy(&c);}

    /**
     * TODO Desturctor
     */
    ~LinkedList() {I_Clear();delete first;}

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element) {
        if ((index<0)||(index>sz)) throw IndexOutOfBound();
        Lnode *temp = new Lnode(element);    sz++;
        if (index==sz-1) {
            last->next = temp;  temp->prev = last;
            last = temp;
        }   else {
            Lnode *idx = (index==0)?first:pos(index-1);
            temp->prev = idx;     temp->next = idx->next;
            idx->next = temp;     temp->next->prev = temp;
        }
    }

    /**
     * TODO Appends the specified element to the end of this list.
     * Always returns true.
     */
    bool add(const T& e) {add(sz,e);    return true;}

    /**
     * TODO Inserts the specified element to the beginning of this list.
     */
    void addFirst(const T& elem) {add(0,elem);}

    /**
     * TODO Insert the specified element to the end of this list.
     * Equivalent to add.
     */
    void addLast(const T &elem) {add(sz,elem);}

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear() {I_Clear();sz = 0;}

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const {
        for (Lnode *idx = first->next;idx;idx = idx->next)
            if (idx->value==e) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {
        if ((index<0)||(index>=sz)) throw IndexOutOfBound();
        return pos(index)->value;
    }

    /**
     * TODO Returns a const reference to the first element.
     * @throw ElementNotExist
     */
    const T& getFirst() const {
        if (sz==0) throw ElementNotExist();
        return first->next->value;
    }

    /**
     * TODO Returns a const reference to the last element.
     * @throw ElementNotExist
     */
    const T& getLast() const {
        if (sz==0) throw ElementNotExist();
        return last->value;
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
        I_Delete(pos(index));
    }

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    bool remove(const T &e) {
        for (Lnode *idx = first->next;idx;idx = idx->next)
            if (e==idx->value) {I_Delete(idx);  return true;}
        return false;
    }

    /**
     * TODO Removes the first element from this list.
     * @throw ElementNotExist
     */
    void removeFirst() {
        if (sz==0) throw ElementNotExist();
        else I_Delete(first->next);
    }

    /**
     * TODO Removes the last element from this list.
     * @throw ElementNotExist
     */
    void removeLast() {
        if (sz==0) throw ElementNotExist();
        else I_Delete(last);
    }

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element) {
        if ((index<0)||(index>=sz)) throw IndexOutOfBound();
        pos(index)->value = element;
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
