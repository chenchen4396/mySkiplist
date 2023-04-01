#include <iostream>
#include <type_traits>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

int main() {

    //SkipList<int, int> skipList(6);
    SkipList<int, std::string> skipList(6);
    //SkipList<std::string, std::string> skipList(6);
    // skipList.insert_element("a", "天"); 
    // skipList.insert_element("b", "天"); 
    // skipList.insert_element("c", "向"); 
    // skipList.insert_element("d", "上"); 
    skipList.insert_element(1, "天"); 
    skipList.insert_element(2, "天"); 
    skipList.insert_element(3, "向"); 
    skipList.insert_element(4, "上"); 
    // skipList.insert_element(1, 1); 
    // skipList.insert_element(2, 2); 
    // skipList.insert_element(3, 3); 
    // skipList.insert_element(4, 4); 
    //std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.dump_file();
    
    skipList.load_file();
    
    // skipList.search_element(9);
    // skipList.search_element(18);

    skipList.display_list();

    //skipList.delete_element("a");
    //skipList.delete_element("b");

    std::cout << "skipList size:" << skipList.size() << std::endl;

    // skipList.display_list();
}
