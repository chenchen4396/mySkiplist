#include <iostream>
#include <type_traits>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

int main() {

    
    //SkipList<int, std::string> skipList(6);
    SkipList<std::string, std::string> skipList(6);
    // skipList.insert_element("a", "天"); 
    // skipList.insert_element("b", "天"); 
    // skipList.insert_element("e", "向"); 
    // skipList.insert_element("d", "上"); 
    // skipList.insert_element("abc", "好好学习");
    // skipList.insert_element("bac", "天天向上");
	// skipList.insert_element(1, "学"); 
    //std::cout << "skipList size:" << skipList.size() << std::endl;

    //skipList.dump_file();

    /*当使用int型为键的类型时，用load_int_file()*/
    //skipList.load_int_file();
    skipList.load_file();
    
    // skipList.search_element(9);
    // skipList.search_element(18);

    //skipList.display_list();

    // skipList.delete_element(3);
    // skipList.delete_element(7);

    // std::cout << "skipList size:" << skipList.size() << std::endl;

    // skipList.display_list();
}
