# std::numeric_limits
        std::numeric_limits<double>::max() 取到double类型的最大值

# localtime、localtime_r、localtime_s
        time_t timep = time(0);
        localtime() 
        struct tm *localtime(const time_t* timep) //获取系统时间，精度为秒

        localtime_r()
        struct tm *localtime_r(const time_t *timep, struct tm *result) //获取系统时间，运行于Linux平台

        localtime_s()
        struct tm *localtime_r(struct tm *result, const time_t *timep) //获取系统时间，运行于Windows平台

# strftime()
        将时间格式化
        size_t strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr)
        
        str：指向目标数组的指针，用来保存格式化后的字符串
        maxsize：被复制到str的最大字符数
        format：C字符串，包含了普通字符和特殊说明符的任何组合——
        "/tmp/steer_log_simple_optimal_%F_%H%M%S.csv"：表示在/tmp目录下，创建steer_log_simple_optimal_日期_时分秒.csv文件

# std::fmod()
        浮点数取模
        double fmod (double  x，double  y); // y不能为0
        eg: fmod(4.3, 2.1) == 0.1

# noexcept关键字
        使用noexcept表明函数或操作不会发生异常，会给编译器更大的优化空间

# std::isnan()、std::isinf()
        isnan (); //判断一个浮点数是否是未定义或无法表示的值，如负根号，负log值，0/0等
                注意：1.0 / 0.0 并不是nan，而是inf
        isinf (); //判断一个浮点数是否为无限

# stl lower_bound()
        用于在指定区域内查找不小于目标值的第一个元素

# 常对象修改成员变量
        需要对变量添加mutable关键字

# 读、写入文件 ofstream
        ofstream是从内存到硬盘，ifstream是从硬盘到内存。
        文件读写的步骤：
        1、包含的头文件：#include <fstream>
        2、创建流, ofstream filename;
        3、打开文件(文件和流关联) open();
        4、读写 (写操作：<<,put( ), write( ) 读操作： >> , get( ),getline( ), read( ))
        5、关闭文件：把缓冲区数据完整地写入文件， 添加文件结束标志， 切断流对象和外部文件的连接 close();
        

        void open(const char* filename, int mode, int access);
        mode：打开文件的方式，在基类std::ios中定义，可以用 | 连接不同是打开方式属性
        ios::app：　　   以追加的方式打开文件  
        ios::ate：　　　 文件打开后定位到文件尾，ios:app就包含有此属性  
        ios::binary：　  以二进制方式打开文件，缺省的方式是文本方式。两种方式的区别见前文  
        ios::in：　　　  文件以输入方式打开（文件数据输入到内存）  
        ios::out：　　   文件以输出方式打开（内存数据输出到文件） 
        ios::nocreate：  不建立文件，所以文件不存在时打开失败  
        ios::noreplace： 不覆盖文件，所以打开文件时如果文件存在失败  
        ios::trunc：　　 如果文件存在，把文件长度设为0 
        access：打开文件的属性，可以用 | 或者 + 连接不同的属性
        0：普通文件，打开访问  
        1：只读文件  
        2：隐含文件  
        4：系统文件 

## csv 文件
        CSV文件有其特殊性，由于逗号分隔符的存在，写入文件时只需要注意不遗漏必要的逗号，即可生成格式化的CSV文件

## 操作符
        dec   格式化为十进制数值数据 输入和输出  
        endl  输出一个换行符并刷新此流 输出（关闭文件输出流） 
        ends  输出一个空字符 输出  
        hex   格式化为十六进制数值数据 输入和输出  
        oct   格式化为八进制数值数据 输入和输出  
        setpxecision(int p) 设置浮点数的精度位数 输出 

### example

```
std::string myGetLogFileName() {
  time_t raw_time;
  char name_buffer[80];
  std::time(&raw_time);
  std::tm time_tm;
  localtime_r(&raw_time, &time_tm);
  strftime(name_buffer, 80, "/tmp/my_%F_%H%M%S.csv",
           &time_tm);
  return std::string(name_buffer);
};

void myWriteData(std::ofstream &file_stream){
  file_stream << "x,"
              << "y,"
              << "heading" << std::endl;
};

int main(){
  std::ofstream my_log_file_;
  my_log_file_.open(myGetLogFileName());
  my_log_file_ << std::fixed;
  my_log_file_ << std::setprecision(6);
  myWriteData(my_log_file_);

  // 第一种写法
  // const std::string my_log_str = absl::StrCat(
  //   com.x(), ",", com.y(), ",", driving_orientation_
  // );
  // my_log_file_ << my_log_str << std::endl; 

  // 第二种写法
  my_log_file_ << com.x() << "," << com.y() << "," << driving_orientation_ << std::endl;

  my_log_file_.close();
}
```

# map unordered_map 的遍历方法
## 迭代器iterator
```