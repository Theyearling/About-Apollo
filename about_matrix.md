# 访问规则
        一般是列优先，若行优先则会在名字中包含row

# Apollo 中 using Matrix = Eigen::MatrixXd;
        typedef Matrix< double, Dynamic, Dynamic > 	Eigen::MatrixXd：double类型的矩阵，Dynamic * Dynamic

# 函数
        rows()/cols() //返回矩阵的行数/列数
        transpose() //对矩阵装置
        inverse() //逆矩阵
        maxCoeff()/minCoeff //矩阵的最大元素/最小元素
        Identity() //以单位矩阵初始化
        block() //块操作，获取矩阵中的一块

## block()
        有两种定义形式：
        block(i, j, p, q) // 表示从[i,j]开始，每行取q个元素，每列取p个元素，即p行q列
        block<p, q>(i, j) //  表示p行q列的矩阵，从[i, j]开始，取一个p行q列的矩阵块  
        

# * 与 ·
        ·乘即向量对应位置的积的累和
        *乘即遵从 左行右列 原则
