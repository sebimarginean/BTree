#ifndef BTREE_H_
#define BTREE_H_

#include <iostream>
#include <vector>

using namespace std;

struct Node
{
    int n;
    int *keys;
    Node **children;
    bool leaf;

    Node(const int t)
    {
        keys = new int[2 * t - 1];
        children = new Node *[2 * t];
        leaf = true;
        n = 0;
    }

    ~Node()
    {
        /*if (!leaf)
            for (int i = 0; i < n; i++)
                delete c[i];*/
        delete[] children;
    }

    /**
     * Search the key in node
     */
    int findKey(int k)
    {
        int i = 0;
        while (i < n && keys[i] < k)
            i++;
        return i;
    }
};

struct BTree
{
    Node *root;
    int t; // Ramification

    BTree(int);
    ~BTree();
    Node *allocateNode();
    Node *search(Node *x, int key);
    int min(Node *w);
    int max(Node *w);
    void insert(int key);
    void insertNonfull(Node *x, int k);
    void splitChild(Node *, int, Node *);

    void del(int key);
    void del(Node *x, int k);
    int successor(Node *x, int k);
    int predecessor(Node *x, int k);
    void removeFromLeaf(Node *x, int i);
    void removeFromNonLeaf(Node *x, int i);
    void fill(Node *x, int i);
    void borrowFromLeft(Node *x, int i);
    void borrowFromRight(Node *x, int i);
    void merge(Node *x, int i);

    void display(Node *, int);
    void inorderDisplay(Node *);
    void indentedDisplay();

    void diskRead(Node *);
    void diskWrite(Node *);
};

Node *BTree::allocateNode()
{
    return new Node(t);
}

BTree::BTree(int i) : t(i)
{
    Node *x = allocateNode();
    diskWrite(x);
    root = x;
}

BTree::~BTree()
{
    delete root;
}

Node *BTree::search(Node *x, int k)
{
    int i = 0;
    while (i < x->n && k > (x->keys)[i])
        i++;
    if (i < x->n && k == (x->keys)[i])
        return x;
    if (x->leaf)
        return 0;
    else
    {
        diskRead((x->children)[i]);
        return search((x->children)[i], k);
    }
}
/**
 * Smallest key in tree
 */
int BTree::min(Node *w)
{
    return w->leaf ? w->keys[0] : min(w->children[0]);
}

/**
 * Biggest key in tree
 */
int BTree::max(Node *w)
{
    return w->leaf ? w->keys[w->n - 1] : max(w->children[w->n]);
}

/**
 * Divide y node. Move median key to x and other half of keys to new node z
 */
void BTree::splitChild(Node *x, int i, Node *y)
{
    Node *z = allocateNode();
    z->leaf = y->leaf;
    z->n = t - 1;
    for (int j = 0; j <= t - 2; j++)
        (z->keys)[j] = (y->keys)[j + t];
    if (!(y->leaf))
        for (int j = 0; j <= t - 1; j++)
            (z->children)[j] = (y->children)[j + t];
    y->n = t - 1;
    for (int j = x->n; j >= i + 1; j--)
    {
        (x->children)[j + 1] = (x->children)[j];
        (x->keys)[j] = (x->keys)[j - 1];
    }
    (x->children)[i + 1] = z;
    (x->keys)[i] = (y->keys)[t - 1];
    x->n = 1 + x->n;
    diskWrite(y);
    diskWrite(z);
    diskWrite(x);
}

/**
 * Insert k key into nonfull node x. Any full node will be split.
 */
void BTree::insertNonfull(Node *x, int k)
{
    int i = x->n - 1;
    if (x->leaf)
    {
        while (i >= 0 && k < (x->keys)[i])
        {
            (x->keys)[i + 1] = (x->keys)[i];
            i--;
        }
        (x->keys)[i + 1] = k;
        x->n = x->n + 1;
        diskWrite(x);
    }
    else
    {
        while (i >= 0 && k < (x->keys)[i])
            i--;
        i++;
        diskRead((x->children)[i]);
        if ((x->children)[i]->n == 2 * t - 1)
        {
            splitChild(x, i, (x->children)[i]);
            if (k > (x->keys)[i])
                i++;
        }
        insertNonfull((x->children)[i], k);
    }
}

/**
 * Insert k key in tree. If root is full, it wil be devided
 */
void BTree::insert(int k)
{
    Node *r = root;
    if (r->n == 2 * t - 1)
    {
        Node *s = allocateNode();
        root = s;
        s->leaf = false;
        s->n = 0;
        (s->children)[0] = r;
        splitChild(s, 0, r);
        insertNonfull(s, k);
    }
    else
        insertNonfull(r, k);
}

void BTree::diskRead(Node *x) {}
void BTree::diskWrite(Node *x) {}

void BTree::display(Node *x, int indent)
{
    if (x == 0)
        return;
    bool b = x->leaf;
    for (int i = 0; i < x->n; i++)
    {
        if (!b)
            display((x->children)[i], indent + 2);
        for (int j = 0; j < indent; j++)
            cout << ' ';
        cout << (x->keys)[i] << '\n';
    }
    if (!b)
        display((x->children)[x->n], indent + 2);
}

void BTree::inorderDisplay(Node *x)
{
    bool b = x->leaf;
    for (int i = 0; i < x->n; i++)
    {
        if (!b)
            inorderDisplay((x->children)[i]);
        cout << (x->keys)[i] << ' ';
    }
    if (!b)
        inorderDisplay((x->children)[x->n]);
}

void BTree::indentedDisplay()
{
    cout << "The B-tree is" << endl;
    display(root, 0);
}

/**************************************/
/**
 * Return the key that succeeds in order the key k
 */
int BTree::successor(Node *x, int k)
{
    // TODO

    Node *current = x->children[x->findKey(k) + 1];
    while (!current->leaf)
        current = current->children[0];

    return current->keys[0];
}

/**
 * Return the key that succeeds in order the key k
 */
int BTree::predecessor(Node *x, int k)
{
    // TODO

    Node *current = x->children[x->findKey(k)];
    while (!current->leaf)
        current = current->children[current->n];

    return current->keys[current->n - 1];
}

/**
 * Method removes the k key in tree.
 * It is using aux del(x, k)
 */
void BTree::del(int k)
{
    if (!root)
    {
        cout << "The tree is empty\n";
        return;
    }

    del(root, k);

    if (root->n == 0)
    {
        Node *tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->children[0];

        delete tmp;
    }
    return;
}

/**

 * This method finds the key that should be removed,
   if it doesn't exist in x, then it is going to it's descendents.
*/
void BTree::del(Node *x, int k)
{
    int i = x->findKey(k);

    // The key that should be removed is in x
    if (i < x->n && x->keys[i] == k)
    {
        // If the node is leaf, call removeFromLeaf
        // Else removeFromNonLeaf
        if (x->leaf)
            removeFromLeaf(x, i);
        else
            removeFromNonLeaf(x, i);
    }
    else
    {

        // If x is leaf, then key isn't present in tree
        if (x->leaf)
        {
            cout << "The key " << k << " does not exist in the tree\n";
            return;
        }

        // last shows if the key which should be removed is present in
        // subtree with root in the last (most right) child of x
        bool last = ((i == x->n) ? true : false);

        // If the child where the key is supposed to exist has less than t keys,
        // fill that child before descending

        if (x->children[i]->n < t)
            fill(x, i);

        // If the last child was merged, it merged with the previous one
        // and resort to the child children[i-1]. Otherwise, we will resort to
        // children[i] which now has at least t keys
        if (last && i > x->n)
            del(x->children[i - 1], k);
        else
            del(x->children[i], k);
    }
    return;
}

/**
 * The method removes the key at position i from a leaf node x
 */
void BTree::removeFromLeaf(Node *x, int i)
{
    for (int j = i + 1; j < x->n; j++)
        x->keys[j - 1] = x->keys[j];

    x->n--;

    return;
}

/**
 * The method removes the key at position i from an internal node x
 */
void BTree::removeFromNonLeaf(Node *x, int i)
{
    // TODO

    int k = x->keys[i];

    if (x->children[i]->n >= t)
    {
        int pred = max(x->children[i]);
        x->keys[i] = pred;
        del(x->children[i], pred);
    }
    else if (x->children[i + 1]->n >= t)
    {
        int succ = min(x->children[i + 1]);
        x->keys[i] = succ;
        del(x->children[i + 1], succ);
    }
    else
    {
        merge(x, i);
        del(x->children[i], k);
    }
    return;
}

/**
 * The method populates the children[i] of x that have less than t keys
 */
void BTree::fill(Node *x, int i)
{
    if (i != 0 && x->children[i - 1]->n >= t)
        borrowFromLeft(x, i);

    else if (i != x->n && x->children[i + 1]->n >= t)
        borrowFromRight(x, i);

    else
    {
        if (i != x->n)
            merge(x, i);
        else
            merge(x, i - 1);
    }
    return;
}

/**
 * Method borrows a key from children[i-1] for children[i]
 * which does not have enough keys
 */
void BTree::borrowFromLeft(Node *x, int i)
{
    Node *child = x->children[i];
    Node *sibling = x->children[i - 1];

    for (int j = child->n - 1; j >= 0; j--)
        child->keys[j + 1] = child->keys[j];

    if (!child->leaf)
    {
        for (int j = child->n; j >= 0; j--)
            child->children[j + 1] = child->children[j];
    }

    child->keys[0] = x->keys[i - 1];

    if (!child->leaf)
        child->children[0] = sibling->children[sibling->n];

    x->keys[i - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;

    return;
}

/**
 * Method borrows a key from children[i] for children[i-1]
 * which does not have enough keys
 */
void BTree::borrowFromRight(Node *x, int i)
{
    Node *child = x->children[i];
    Node *sibling = x->children[i + 1];

    child->keys[(child->n)] = x->keys[i];

    if (!(child->leaf))
        child->children[(child->n) + 1] = sibling->children[0];

    x->keys[i] = sibling->keys[0];

    for (int j = 1; j < sibling->n; j++)
        sibling->keys[j - 1] = sibling->keys[j];

    if (!sibling->leaf)
    {
        for (int j = 1; j <= sibling->n; j++)
            sibling->children[j - 1] = sibling->children[j];
    }

    child->n += 1;
    sibling->n -= 1;

    return;
}

/**
 * Method concatenates children[i] and children[i+1] of x
 * and add the key x[i] that separates the two sons
 */
void BTree::merge(Node *x, int i)
{
    // TODO
    Node *child = x->children[i];
    Node *sibling = x->children[i + 1];
    child->keys[t - 1] = x->keys[i];
    for (int j = 0; j < sibling->n; j++)
        child->keys[j + t] = sibling->keys[j];
    if (!child->leaf)
    {
        for (int j = 0; j <= sibling->n; j++)
            child->children[j + t] = sibling->children[j];
    }
    for (int j = i + 1; j < x->n; j++)
    {
        x->keys[j - 1] = x->keys[j];
        x->children[j] = x->children[j + 1];
    }
    child->n += sibling->n + 1;
    x->n--;
    delete sibling;
    return;
}

#endif /* BTREE_H_ */
