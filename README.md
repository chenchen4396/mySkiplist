原作者：https://github.com/youngyangyang04/Skiplist-CPP


本项目是基于跳表实现的轻量级键值型存储引擎，使用C++实现。插入数据、删除数据、查询数据、数据展示、数据落盘、文件加载数据，以及数据库大小显示。


优化：
    
    
    1.使用了读写锁，在多线程下效率更高
    
    2.修复了load_file存在的bug
    
    3.Key、value可支持string、int
    
    4.解决了大部分内存泄露等问题
    
    5.优化了部分代码，提高效率