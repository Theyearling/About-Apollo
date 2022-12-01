# 数据定义
        message 类似于struct，可以定义数据结构

        enum 定义具有预定义值列表的字段
        
        message可以嵌套message，enum；enum可以定义在message中

# 字段修饰符
        optional：可选择的，具有默认值，若字段没有设置值，则赋值为定义的默认值，若没有定义默认值，则设置为系统的默认值

        repeated：该字段可以重复任意多次（包含0次），可以视为一个动态size的数组

        required：必须给出字段值，否则解析会出错

# 标识符
        "=" + 不同数字 来标识，并不是赋值
        eg：optional double num = 1;

# 编译
        为每个message创建一个类，为类内的每个字段创建对应的访问函数

        name：对应字段的名字
        inline bool has_name() const;
        inline void clear_name();
        inline (不同类型，不同返回值) name() const;
        inline void set_name(不同类型，不同参数);

        对应repeated修饰的字段，有所不同
        inline int name_size() const;
        inline void clear_name();
        inline (不同类型，不同返回值) name() const;
        inline (不同类型，不同返回值) add_name();

