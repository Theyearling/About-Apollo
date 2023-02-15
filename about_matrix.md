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
[参考](https://zhuanlan.zhihu.com/p/359975221)

        ·乘即向量对应位置的积的累和，从几何角度看即两个向量的长度与它们夹角的余弦值得积
$$
        \vec{a} = (x_1, y_1, z_1)\ \ \vec{b} = (x_2, y_2, z_2)\\ 
        \vec{a} \cdot \vec{b} = x_1*x_2\ +\ y_1*y_2\ +\ z_1*z_1 \\
        \vec{a} \cdot \vec{b} = |\vec{a}|\ *\ |\vec{b}|\ *\ cos{\theta} 
$$
### 几何意义
点乘的结果表示 $\vec{a}$ 在 $\vec{b}$ 方向上的投影与 $|\vec{b}|$ 的乘积，反映了两个向量在方向上的相似度，结果越大越相似，基于结果可以判断这两个向量是否在同一方向上，是否正交垂直，具体对应关系为：<br>
1. $\vec{a} \cdot \ \vec{b} > 0$ 则方向基本相同，夹角在 $0^\circ$ 到 $90^\circ$ 之间
2. $\vec{a} \cdot \ \vec{b} = 0$ 则正交，相互垂直 
3. $\vec{a} \cdot \ \vec{b} < 0$ 则方向基本相反，夹角在 $90^\circ$ 到 $180^\circ$ 之间 

        *乘即遵从 左行右列 原则

