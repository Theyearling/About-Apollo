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