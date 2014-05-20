/** @file */
#ifndef __TREEMAP_H
#define __TREEMAP_H

#include "ElementNotExist.h"
/*
    Author: Jerry Xu;
    Number: 5130309056;
    Class:  Data Struct---2014;
    Version:
        0.1     2014/5/4    Finish without Iterator;
        0.2     2014/5/19   Finish without compile;
        1.0     2014/5/19   Pass TA Test;
*/
/**
 * TreeMap is the balanced-tree implementation of map. The iterators must
 * iterate through the map in the natural order (operator<) of the key.
 */
template<class K, class V>
class TreeMap
{
public:
    class Entry {
    public:
        K key;
        V value;
        Entry *son[2];
        Entry *father;
        Entry() {father=son[0]=son[1]=NULL;}
        Entry(K k, V v) {key = k;value = v;father=son[0]=son[1]=NULL;}
        K getKey() const {return key;}
        V getValue() const {return value;}
    };
private:
    Entry *root;
    int sz;
    int sonc(Entry *u){return (u->father->son[0] == u)?0:1;}
    void zig(Entry *u){  //il=1 left, lr=0 right
        int il = 1-sonc(u);
        Entry *y = u->father;
        u->father = y->father;
        if (y==root) root = u;
        else y->father->son[sonc(y)] = u;
        y->son[1-il] = u->son[il];
        if (u->son[il]) u->son[il]->father = y;
        y->father = u;  u->son[il] = y;
    }
    void splay(Entry *u){
        while (u!=root) if (u->father==root) zig(u);
        else if (sonc(u) == sonc(u->father)) {zig(u->father);zig(u);}
        else {zig(u);zig(u);}
    }
    Entry *I_get(K key){
        Entry *idx = root;
        while (idx) {
            if (idx->key==key) return idx;
            if (idx->key<key) idx = idx->son[1];
            else idx = idx->son[0];
        }
        return NULL;
    }
    bool I_findv(V v,Entry *u){
        if (u==NULL) return false;
        return ((u->value==v)||(I_findv(v,u->son[0]))||(I_findv(v,u->son[1])));
    }
    void I_clear(Entry *u){
        if (u==NULL) return;
        I_clear(u->son[0]); I_clear(u->son[1]);
        delete u;
    }
    void I_copy(Entry *u,Entry *v){
        for (int i=0;i<2;i++) if (v->son[i]) {
            u->son[i] = new Entry(v->son[i]->key,v->son[i]->value);
            u->son[i]->father = u;
            I_copy(u->son[i],v->son[i]);
        }
    }
public:
    class Iterator
    {
        Entry *idx;
        int crt; //0=left child from F;1=right child from F;2=from left child;3=from right child;
        TreeMap *tm;
    public:
        Iterator(TreeMap *x) {
            tm = x;
            if (x->sz) {
                idx = x->root;
                while (idx->son[0]) idx = idx->son[0];
            } else idx = NULL;
        }
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {return (idx!=NULL);}

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
            if (idx==NULL) throw ElementNotExist();
            Entry *temp = idx;
            if (idx->son[1]) {
                idx = idx->son[1];
                while (idx->son[0]) idx = idx->son[0];
            } else {
                while ((idx!=tm->root)&&(tm->sonc(idx))) idx = idx->father;
                if (idx==tm->root) idx = NULL;
                else idx = idx->father;
            }
            return *temp;
        }
    };

    /**
     * TODO Constructs an empty tree map.
     */
    TreeMap() {sz = 0;root = NULL;}

    /**
     * TODO Destructor
     */
    ~TreeMap() {if (sz) I_clear(root);}

    /**
     * TODO Assignment operator
     */
    TreeMap &operator=(const TreeMap &x) {
        if (sz = x.sz) {
            root = new Entry(x.root->key,x.root->value);
            I_copy(root,x.root);
        }
    }

    /**
     * TODO Copy-constructor
     */
    TreeMap(const TreeMap &x) {
        if (sz = x.sz) {
            root = new Entry(x.root->key,x.root->value);
            I_copy(root,x.root);
        }
    }

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator() {return Iterator(this);}

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear() {I_clear(root);    root = NULL;    sz = 0;}

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) {return (I_get(key)!=NULL);}

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) {return (I_findv(value,root));}

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) {
        Entry *idx = I_get(key);
        if (idx) return idx->value;
        else throw ElementNotExist();
    }

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const {return (sz==0);}

    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value) {
        if (I_get(key)) {I_get(key)->value = value;return;}
        Entry *u = new Entry(key,value);   sz++;
        u->son[0] = NULL;   u->son[1] =NULL;
        u->key = key;       u->value = value;
        Entry *y = root;
        if (y==NULL) {root = u; return;}
        for (;;) {
            int il = (y->key<u->key)?1:0;
            if (y->son[il]) y = y->son[il];
            else {
                u->father = y;  y->son[il] = u;
                splay(u);break;
            }
        }
    }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) {
        Entry *u = I_get(key);
        splay(u);
        Entry *l = u->son[0];
        Entry *r = u->son[1];
        delete u;   sz--;
        if (sz==0) root = NULL;
        else if (l==NULL) {root = r;r->father = NULL;}
        else if (r==NULL) {root = l;l->father = NULL;}
        else {
            root = r;   r->father = NULL;
            while (r->son[0]) r = r->son[0];
            l->father = r;  r->son[0] = l;
            splay(l);
        }
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {return sz;}
};

#endif
