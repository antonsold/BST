#include <iostream>
#include <iterator>
#include <deque>

using namespace std;

template<typename T = int, class C = less<T> >
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
    int pre_order_helper(Node*&); //helper recursive function for pre-order traversal, returns number of elements
    pair<int, bool> post_order_helper(Node*&); //helper recursive function for post-order traversal, returns pair (height, balanced(0/1))
    static Node* tree_min(Node*&);
    static Node* tree_max(Node*&);
    Node* root;
    C cmp;
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
    int pre_order(); //pre-order traversal
    bool is_balanced(); //post-order traversal
    class tree_iterator: public std::bidirectional_iterator_tag{
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


template<typename T, class C>
typename BSTree<T, C>::Node* BSTree<T, C>::my_insert(BSTree<T, C>::Node* &tree, BSTree<T, C>::Node* parent, T value){
    if(tree==nullptr){
        tree = new BSTree<T, C>::Node;
        tree->key = value;
        tree->parent = parent;
        tree->left = nullptr;
        tree->right = nullptr;
        return tree;
    }
    else {
        if (!cmp(value, tree->key) && !cmp(tree->key, value))
            return nullptr;
        else if (cmp(value, tree->key))
            return my_insert(tree->left, tree, value);
        else
            return my_insert(tree->right, tree, value);
    }
}

template<typename T, class C>
int BSTree<T, C>::in_order_helper(BSTree<T, C>::Node *&tree) {
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

template<typename T, class C>
int BSTree<T, C>::pre_order_helper(BSTree<T, C>::Node *&tree) {
    int l, r;
    if(tree != nullptr){
        //do something with root here;
        l = pre_order_helper(tree->left);
        r = pre_order_helper(tree->right);
    }
    else
        return 0;
    return l + r + 1;
}

template<typename T, class C>
pair<int, bool> BSTree<T, C>::post_order_helper(BSTree<T, C>::Node *&tree) {
    pair<int, bool> l, r;
    bool balanced;
    int height;
    if(tree != nullptr){
        l = post_order_helper(tree->left);
        r = post_order_helper(tree->right);
        //do something with root here;
        balanced = !(!l.second || !r.second || l.first - r.first > 1 || l.first - r.first < -1);
        height = max(l.first, r.first) + 1;
        return make_pair(height, balanced);
    }
    else
        return make_pair(0, true);
}

template<typename T, class C>
int BSTree<T, C>::in_order() {
    return in_order_helper(root);
}

template<typename T, class C>
int BSTree<T, C>::pre_order() {
    return pre_order_helper(root);
}

template<typename T, class C>
bool BSTree<T, C>::is_balanced(){
    return post_order_helper(root).second;
}

template<typename T, class C>
void BSTree<T, C>::insert(T value) {
    BSTree<T, C>::Node *ptr = my_insert(this->root, nullptr, value);
    if (ptr != nullptr) {
        if (minptr == nullptr || ptr->key < minptr->key)
            minptr = ptr;
        if (maxptr == nullptr || maxptr->key < ptr->key)
            maxptr = ptr;
    }
}

template <typename T, class C>
typename BSTree<T, C>::Node* BSTree<T, C>::find(T value) const{
    BSTree<T, C>::Node *ptr = root;
    while(ptr != nullptr && ptr->key != value){
        if (cmp(ptr->key, value))
            ptr = ptr->right;
        else
            ptr = ptr->left;
    }
    return ptr;
}

template <typename T, class C>
typename BSTree<T, C>::Node* BSTree<T, C>::succ(Node *ptr) {
    if(ptr->right != nullptr)
        ptr = tree_min(ptr->right);
    else{
        while(ptr->parent != nullptr && ptr->parent->right == ptr)
            ptr = ptr->parent;
        ptr = ptr->parent;
    }
    return ptr;
}

template <typename T, class C>
typename BSTree<T, C>::Node* BSTree<T, C>::pred(Node* ptr){
    if (ptr->left != nullptr)
        ptr = tree_max(ptr->left);
    else{
        while(ptr->parent != nullptr && ptr == ptr->parent->left)
            ptr = ptr->parent;
        ptr = ptr->parent;
    }
    return ptr;
}


template <typename T, class C>
BSTree<T, C>::tree_iterator::tree_iterator(BSTree::Node *_ptr): ptr(_ptr), pos(0) {
    visited.push_back(_ptr);
}

template <typename T, class C>
typename BSTree<T, C>::tree_iterator& BSTree<T, C>::tree_iterator::operator=(const BSTree<T, C>::tree_iterator &it) {
    this->ptr = it.ptr;
    this->visited = it.visited;
    this->pos = it.pos;
    return *this;
}

template <typename T, class C>
typename BSTree<T, C>::Node* BSTree<T, C>::tree_iterator::operator*() {
    return ptr;
}

template <typename T, class C>
typename BSTree<T, C>::tree_iterator &BSTree<T, C>::tree_iterator::operator++() {
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

template <typename T, class C>
typename BSTree<T, C>::tree_iterator &BSTree<T, C>::tree_iterator::operator--() {
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

template <typename T, class C>
typename BSTree<T, C>::tree_iterator BSTree<T, C>::tree_iterator::operator++(int not_used) {
    BSTree<T, C>::tree_iterator tmp(*this);
    ++(*this);
    return tmp;
}

template <typename T, class C>
typename BSTree<T, C>::tree_iterator BSTree<T, C>::tree_iterator::operator--(int not_used) {
    BSTree<T, C>::tree_iterator tmp(*this);
    --(*this);
    return tmp;
}

template <typename T, class C>
bool BSTree<T, C>::tree_iterator::operator==(const BSTree::tree_iterator &it) const{
    if (this->ptr == nullptr || it.ptr == nullptr)
        return this->ptr == it.ptr;
    else
        return this->ptr->key == it.ptr->key;
}

template <typename T, class C>
bool BSTree<T, C>::tree_iterator::operator!=(const BSTree::tree_iterator &it) const{
    if (this->ptr == nullptr || it.ptr == nullptr)
        return this->ptr != it.ptr;
    else
        return this->ptr->key == it.ptr->key;
}

template <typename T, class C>
bool BSTree<T, C>::tree_iterator::operator>(const BSTree::tree_iterator &it) const{
    if (this->ptr == nullptr)
        return true;
    else if (it.ptr == nullptr)
        return false;
    else
        return this->ptr->key > it.ptr->key;
}

template <typename T, class C>
bool BSTree<T, C>::tree_iterator::operator<(const BSTree::tree_iterator &it) const{
    if (this->ptr == nullptr)
        return false;
    else if (it.ptr == nullptr)
        return true;
    else
        return this->ptr->key < it.ptr->key;
}

template <typename T, class C>
typename BSTree<T, C>::tree_iterator BSTree<T, C>::begin() {
    BSTree<T, C>::tree_iterator it(minptr);
    return tree_iterator(minptr);
}

template <typename T, class C>
typename BSTree<T, C>::tree_iterator BSTree<T, C>::end() {
    return tree_iterator(nullptr);
}

template <typename T, class C>
typename BSTree<T, C>::Node* BSTree<T, C>::tree_max(BSTree<T, C>::Node *&tree) {
    if (tree->right == nullptr)
        return tree;
    else
        return tree_max(tree->right);
}

template <typename T, class C>
typename BSTree<T, C>::Node* BSTree<T, C>::tree_min(BSTree<T, C>::Node *&tree) {
    if (tree->left == nullptr)
        return tree;
    else
        return tree_min(tree->left);
}

int main() {
    int x;
    BSTree<> t;
    cin >> x;
    while(x != 0){
        t.insert(x);
        cin >> x;
    }
    if(t.is_balanced())
        cout << "YES";
    else
        cout << "NO";
    return 0;
}
