#include <iostream>
#include <iterator>
#include <deque>
using namespace std;

template<typename T>
class BSTree{
private:
    struct Node{
        T key;
        Node* left;
        Node* right;
        Node* parent;
    };
    Node* my_insert(Node*&, Node*, T); //helper function for insert()
    Node* root;
public:
    BSTree(){root= nullptr; minptr=nullptr; maxptr=nullptr;} //default constructor
    inline bool empty(){return root == nullptr;};
    void insert(T);
    Node* minptr; //pointer to minimum
    Node* maxptr; //pointer to maximum
    Node* find(T);
    static Node* succ(Node*); //successor
    static Node* pred(Node*); //predecessor
    class tree_iterator: public std::random_access_iterator_tag{
    private:
        Node* ptr;
        deque<Node*> visited;
        size_t pos;
    public:
        tree_iterator(): pos(0){};
        explicit tree_iterator(Node* _ptr);
        tree_iterator(const tree_iterator& it): ptr(it.ptr), visited(it.visited), pos(it.pos){};
        tree_iterator&operator=(const tree_iterator&);
        T operator*();
        tree_iterator&operator++();
        tree_iterator&operator--();
    };
};


template<typename T>
typename BSTree<T>::Node* BSTree<T>::my_insert(BSTree<T>::Node* &tree, BSTree<T>::Node* parent, T value){
    if(tree==nullptr){
        tree = new BSTree<T>::Node;
        tree->key = value;
        tree->parent = parent;
        tree->left = nullptr;
        tree->right = nullptr;
        return tree;
    }
    else {
        if (value == tree->key)
            return nullptr;
        if (value < tree->key)
            return my_insert(tree->left, tree, value);
        else
            return my_insert(tree->right, tree, value);
    }
}

template<typename T>
void BSTree<T>::insert(T value) {
    BSTree<T>::Node *ptr = my_insert(this->root, nullptr, value);
    if (minptr == nullptr || ptr->key < minptr->key)
        minptr = ptr;
    if (maxptr == nullptr || ptr->key > maxptr->key)
        maxptr = ptr;
}

template <typename T>
typename BSTree<T>::Node* BSTree<T>::find(T value) {
    BSTree<T>::Node *ptr = root;
    while(ptr != nullptr && ptr->key != value){
        if (ptr->key < value)
            ptr = ptr->right;
        else
            ptr = ptr->left;
    }
    return ptr;
}

template <typename T>
typename BSTree<T>::Node* BSTree<T>::succ(Node *ptr) {
    if(ptr->right != nullptr)
        ptr = ptr->right;
    else{
        while(ptr->parent != nullptr && ptr->parent->key < ptr->key)
            ptr = ptr->parent;
        ptr = ptr->parent;
    }
    return ptr;
}

template <typename T>
typename BSTree<T>::Node* BSTree<T>::pred(Node* ptr){
    if (ptr->left != nullptr)
        ptr = ptr->left;
    else{
        while(ptr->parent != nullptr && ptr->parent->key > ptr->key)
            ptr = ptr->parent;
        ptr = ptr->parent;
    }
    return ptr;
}

template <typename T>
BSTree<T>::tree_iterator::tree_iterator(BSTree::Node *_ptr): ptr(_ptr), pos(0) {
    visited.push_back(_ptr);
}

template <typename T>
typename BSTree<T>::tree_iterator& BSTree<T>::tree_iterator::operator=(const BSTree<T>::tree_iterator &it) {
    this->ptr = it.ptr;
    this->visited = it.visited;
    this->pos = it.pos;
    return *this;
}

template <typename T>
T BSTree<T>::tree_iterator::operator*() {
    return ptr->key;
}

template <typename T>
typename BSTree<T>::tree_iterator &BSTree<T>::tree_iterator::operator++() {
    if(pos == visited.size() - 1) {
        visited.push_back(succ(visited.back()));
        ptr = visited.back();
        ++pos;
    }
    else{
        ++pos;
        ptr = visited[pos];
    }
    return *this;
}

template <typename T>
typename BSTree<T>::tree_iterator &BSTree<T>::tree_iterator::operator--() {
    if(pos == 0){
        visited.push_front(pred(visited.front()));
        ptr = visited.front();
    }
    else{
        --pos;
        ptr = visited[pos];
    }
    return *this;
}

int main() {
    BSTree<int> t;
    t.insert(5);
    t.insert(4);
    t.insert(10);
    t.insert(0);
    t.insert(11);
    return 0;
}
