#include <iostream> 
#include <cstdlib>
#include <string>
#include <cmath>
#include <cstring>
#include <mutex> 
#include <shared_mutex> 
#include <fstream>
#define STORE_FILE "./store/dumpFile"
// typedef std::shared_lock<std::shared_mutex> ReadLock;
// typedef std::lock_guard<std::shared_mutex> WriteLock;
std::shared_mutex mtx;
std::string delimiter = ":";

//Class template to implement node
template<typename K, typename V> 
class Node {
public:
    
    Node() {} 
    
    Node(K k, V v, int); 

    ~Node();

    K get_key() const;

    V get_value() const;
    
    int get_node_level() const;

    void set_value(V);
    
    // Linear array to hold pointers to next node of different level
    Node<K, V> **forward;

    

private:
    int node_level;
    K key;
    V value;
};
// Class template for Skip list
template <typename K, typename V> 
class SkipList {
public: 
    SkipList(int);
    ~SkipList();
    
    Node<K, V>* create_node(K, V, int);
    
    int insert_element(K, V);
   
    void display_list();
    
    bool search_element(K);
    
    void delete_element(K);
    
    void dump_file();
    
    void load_file();
    
    int size();
    
    int get_random_level();

private:
    void get_key_value_from_string(const std::string& str, std::string* key, std::string* value);
    bool is_valid_string(const std::string& str);

    // Maximum level of the skip list 
    int _max_level;

    // current level of skip list 
    int _skip_list_level;

    // pointer to header node 
    Node<K, V> *_header;

    // file operator
    std::ofstream _file_writer;
    std::ifstream _file_reader;

    // skiplist current element count
    int _node_count;
};
template<typename K, typename V> 
Node<K, V>::Node(const K k, const V v, int level) {
    this->key = k;
    this->value = v;
    this->node_level = level; 

    // level + 1, because array index is from 0 - level
    this->forward = new Node<K, V>*[level+1];
    
	// Fill forward array with 0(NULL) 
    memset(this->forward, 0, sizeof(Node<K, V>*)*(level+1));
};
template<typename K, typename V> 
Node<K, V>::~Node() {
    delete []forward;
};

template<typename K, typename V> 
K Node<K, V>::get_key() const {
    return key;
};

template<typename K, typename V> 
V Node<K, V>::get_value() const {
    return value;
};
template<typename K, typename V> 
int Node<K, V>::get_node_level() const{
    return node_level;
};
template<typename K, typename V> 
void Node<K, V>::set_value(V value) {
    this->value=value;
};



template<typename K, typename V> 
SkipList<K, V>::SkipList(int max_level) {

    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_node_count = 0;

    K k;
    V v;
    this->_header = new Node<K, V>(k, v, _max_level);
};

template<typename K, typename V> 
SkipList<K, V>::~SkipList() {

    if (_file_writer.is_open()) {
        _file_writer.close();
    }
    if (_file_reader.is_open()) {
        _file_reader.close();
    }
    
    Node<K,V>* current=_header->forward[0];
    while(current){
        Node<K,V> *tmp=current->forward[0];
        delete current;
        current=tmp;
    }
    delete _header;
}


template<typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(const K k, const V v, int level) {
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n;
}


template<typename K, typename V>
int SkipList<K, V>::get_random_level(){

    int k = 0;
    while (rand() % 2) {
        k++;
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
}
//display_list
template<typename K, typename V> 
void SkipList<K, V>::display_list() {
    
    mtx.lock_shared();
    std::cout << "\n*****SkipList*****"<<"\n"; 
    for (int i = 0; i <= _skip_list_level; i++) {
        Node<K, V> *node = this->_header->forward[i]; 
        std::cout << "Level " << i << ": ";
        while (node != NULL) {
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
    mtx.unlock_shared();
}

template<typename K, typename V> 
int SkipList<K, V>::size() { 
    return _node_count;
}

template<typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string* key, std::string* value) {

    if(!is_valid_string(str)) {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter)+1, str.length());
}

template<typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {

    if (str.empty()) {
        return false;
    }
    if (str.find(delimiter) == std::string::npos) {
        return false;
    }
    return true;
}

template<typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value) {
    mtx.lock();
    Node<K, V> *current = this->_header;
    
    // create update array and initialize it 
    // update is array which put node that the node->forward[i] should be operated later
    Node<K, V> *update[_max_level+1];
    memset(update, 0, sizeof(Node<K, V>*)*(_max_level+1));  

    // start form highest level of skip list 
    for(int i = _skip_list_level; i >= 0; i--) {
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key) {
            current = current->forward[i]; 
        }
        update[i] = current;
    }

    // reached level 0 and forward pointer to right node, which is desired to insert key.
    current = current->forward[0];

    // if current node have key equal to searched key, we get it
    if (current != NULL && current->get_key() == key) {
        std::cout << "key: " << key << ", exists" << std::endl;
        mtx.unlock();
        return 1;
    }

    // if current is NULL that means we have reached to end of the level 
    // if current's key is not equal to key that means we have to insert node between update[0] and current node 
    else if (current == NULL || current->get_key() != key ) {
        
        // Generate a random level for node
        int random_level = get_random_level();

        // If random level is greater thar skip list's current level, initialize update value with pointer to header
        if (random_level > _skip_list_level) {
            for (int i = _skip_list_level+1; i < random_level+1; i++) {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }

        // create new node with random level generated 
        Node<K, V>* inserted_node = create_node(key, value, random_level);
        
        // insert node 
        for (int i = 0; i <= random_level; i++) {
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
        _node_count ++;
    }
    mtx.unlock();
    return 0;
}

template<typename K, typename V> 
void SkipList<K, V>::delete_element(K key) {
    mtx.lock(); 
    Node<K,V> *current=this->_header;
    Node<K,V> *update[_max_level+1];
    memset(update, 0, sizeof(Node<K, V>*)*(_max_level+1));
    for(int i=_skip_list_level;i>=0;--i){
        while(current->forward[i]!=NULL && current->forward[i]->get_key()<key){
            current=current->forward[i];
        }
        update[i]=current;
    }
    current=current->forward[0];
    if(current!=NULL && current->get_key()==key){
        for (int i = current->get_node_level(); i >= 0; --i) {
            update[i]->forward[i] = current->forward[i];
        }

        // Remove levels which have no elements
        while (_skip_list_level > 0 && _header->forward[_skip_list_level] == 0) {
            -- _skip_list_level; 
        }

        std::cout << "Successfully deleted key "<< key << std::endl;
        _node_count --;
    }
    else{
        std::cout <<"(deleted)Not exist "<<"key"<< key << std::endl;
    }
    mtx.unlock();
    return;

}

template<typename K,typename V>
bool SkipList<K,V>::search_element(K key){
    mtx.lock_shared();
    Node<K,V> *current=this->_header;
    for(int i=_skip_list_level;i>=0;--i){
        while(current->forward[i]!=NULL && current->forward[i]->get_key()<key){
            current=current->forward[i];
        }
    }
    current=current->forward[0];
    if(current!=NULL&&current->get_key()==key){
        std::cout << "Found key: " << key << ", value: " << current->get_value() << std::endl;
        mtx.unlock_shared();
        return true;
    }
    std::cout << "Not Found Key:" << key << std::endl;
    mtx.unlock_shared();
    return false;
}
template<typename K, typename V> 
void SkipList<K, V>::dump_file() {
    std::cout << "dump_file-----------------" << std::endl;
    _file_writer.open(STORE_FILE);
    Node<K, V> *node = this->_header->forward[0];  

    while (node != NULL) {
        _file_writer << node->get_key() << ":" << node->get_value() << "\n";
        //std::cout << node->get_key() << ":" << node->get_value() << ";\n";
        node = node->forward[0];
    }

    _file_writer.flush();
    _file_writer.close();
    return ;
}

template<typename K, typename V>
void SkipList<K, V>::load_file() {

    _file_reader.open(STORE_FILE);
    std::cout << "load_file-----------------" << std::endl;
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        insert_element(*key,*value);
    }
    delete key;
    delete value;
    _file_reader.close();
}
template<>
void SkipList<int, int>::load_file() {

    _file_reader.open(STORE_FILE);
    std::cout << "load_file-----------------" << std::endl;
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        int tmp=stoi(*key);
        int tmp2=stoi(*value);
        insert_element(tmp, tmp2);
    }
    delete key;
    delete value;
    _file_reader.close();
}
template<>
void SkipList<std::string, int>::load_file() {

    _file_reader.open(STORE_FILE);
    std::cout << "load_file-----------------" << std::endl;
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        int tmp2=stoi(*value);
        insert_element(*key, tmp2);
    }
    delete key;
    delete value;
    _file_reader.close();
}
template<>
void SkipList<int,std::string>::load_file() {

    _file_reader.open(STORE_FILE);
    std::cout << "load_file-----------------" << std::endl;
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        int tmp=stoi(*key);
        insert_element(tmp, *value);
    }
    delete key;
    delete value;
    _file_reader.close();
}