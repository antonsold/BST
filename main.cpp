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
    Node* my_insert(Node*&, Node*, T); //helper recursive function for insert()
    int in_order_helper(Node*&); //helper recursive function for in-order traversal, returns tree height
    Node* root;
public:
    BSTree(){root= nullptr; minptr=nullptr; maxptr=nullptr;} //default constructor
    inline bool empty() {return root == nullptr;};
    void insert(T);
    Node* minptr; //pointer to minimum
    Node* maxptr; //pointer to maximum
    Node* find(T) const;
    static Node* succ(Node*); //successor
    static Node* pred(Node*); //predecessor
    int in_order(); //in-order traversal
    class tree_iterator: public std::random_access_iterator_tag{
    private:
        Node* ptr;
        deque<Node*> visited;
        size_t pos;
    public:
        tree_iterator(): pos(0){};
        explicit tree_iterator(Node* _ptr);
        tree_iterator(const tree_iterator& it): ptr(it.ptr), visited(it.visited), pos(it.pos){}; //copy constructor
        tree_iterator&operator=(const tree_iterator&);
        Node* operator*();
        tree_iterator&operator++(); //pre-increment
        tree_iterator&operator--(); //pre-decrement
        tree_iterator operator++(int); //post-increment
        tree_iterator operator--(int); //post-decrement
        bool operator==(const tree_iterator&) const; //comparisons
        bool operator!=(const tree_iterator&) const;
        bool operator<(const tree_iterator&) const;
        bool operator>(const tree_iterator&) const;
    };
    tree_iterator begin();
    tree_iterator end();
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
        else if (value < tree->key)
            return my_insert(tree->left, tree, value);
        else
            return my_insert(tree->right, tree, value);
    }
}

template<typename T>
int BSTree<T>::in_order_helper(BSTree<T>::Node *&tree) {

    int l, r;
    if(tree != nullptr){

        l = in_order_helper(tree->left);
        //do something with root here;
        r = in_order_helper(tree->right);
    }
    else
        return 0;
    return max(l, r) + 1;
}

template<typename T>
int BSTree<T>::in_order() {
    return in_order_helper(root);
}


template<typename T>
void BSTree<T>::insert(T value) {
    BSTree<T>::Node *ptr = my_insert(this->root, nullptr, value);
    if (ptr != nullptr) {
        if (minptr == nullptr || ptr->key < minptr->key)
            minptr = ptr;
        if (maxptr == nullptr || maxptr->key < ptr->key)
            maxptr = ptr;
    }
}

template <typename T>
typename BSTree<T>::Node* BSTree<T>::find(T value) const{
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
typename BSTree<T>::Node* BSTree<T>::tree_iterator::operator*() {
    return ptr;
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

template <typename T>
typename BSTree<T>::tree_iterator BSTree<T>::tree_iterator::operator++(int not_used) {
    BSTree<T>::tree_iterator tmp(*this);
    ++(*this);
    return tmp;
}

template <typename T>
typename BSTree<T>::tree_iterator BSTree<T>::tree_iterator::operator--(int not_used) {
    BSTree<T>::tree_iterator tmp(*this);
    --(*this);
    return tmp;
}

template <typename T>
bool BSTree<T>::tree_iterator::operator==(const BSTree::tree_iterator &it) const{
    if (this->ptr == nullptr || it.ptr == nullptr)
        return this->ptr == it.ptr;
    else
        return this->ptr->key == it.ptr->key;
}

template <typename T>
bool BSTree<T>::tree_iterator::operator!=(const BSTree::tree_iterator &it) const{
    if (this->ptr == nullptr || it.ptr == nullptr)
        return this->ptr != it.ptr;
    else
        return this->ptr->key == it.ptr->key;
}

template <typename T>
bool BSTree<T>::tree_iterator::operator>(const BSTree::tree_iterator &it) const{
    if (this->ptr == nullptr)
        return true;
    else if (it.ptr == nullptr)
        return false;
    else
        return this->ptr->key > it.ptr->key;
}

template <typename T>
bool BSTree<T>::tree_iterator::operator<(const BSTree::tree_iterator &it) const{
    if (this->ptr == nullptr)
        return false;
    else if (it.ptr == nullptr)
        return true;
    else
        return this->ptr->key < it.ptr->key;
}

template <typename T>
typename BSTree<T>::tree_iterator BSTree<T>::begin() {
    BSTree<T>::tree_iterator it(minptr);
    return tree_iterator(minptr);
}

template <typename T>
typename BSTree<T>::tree_iterator BSTree<T>::end() {
    return tree_iterator(nullptr);
}

int main() {
    int x;
    BSTree<int> t;
    cin >> x;
    while(x != 0){
        t.insert(x);
        cin >> x;
    }
    cout << t.in_order();
    return 0;
}
