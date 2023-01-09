# 仿真：
1. 连接服务器
2. cd apollo_simulate/apollo
3. sudo git checkout develop
4. ./build.sh 选择d1
5. ./build.sh 选择1
6. ./scripts/bootstrap.sh 启动dreamview

# 默认都是docker里面

# 实车：
1. 连接服务器 
   ssh houmo@192.168.10.133 密码：houmo
2. 编译（进入docker）
    cd /data/houmo/apollo-houmo-dev/
    ./build.sh 选择d3（orin）
3. 启动gnss和lidar
    cd /home/caros/
    ./start-gnss.sh
    ./start-lidar.sh
4. houmo服务器中启动set30fps....
    cd /home/houmo/
    ./set30fps_SyncSignal.sh
5. 启动dreamview
    cd /apollo/
    ./scripts/bootstrap.sh


# 拉取代码：
1. 设置代理地址
    cd /data/houmo/apollo-houmo-dev/
    sudo vi setproxy.sh
2. 拉取代码
    git pull
3. 编译
    (docker里面) cd /apollo/
    ./build.sh 选择2


# 启用csv，修改参数：
- ### enable_csv_debug: true
- ### enable_input_timestamp_check: true
- ### max_localization_miss_num: 30
- ### max_planning_miss_num: 10
    1. cd /data/houmo/apollo-houmo-dev/
    2. vi modules/control/common/control_gflags.cc
    3. vi modules/control/conf/control_conf.pb.txt
    4. vi modules/calibration/data/neolix_x3p/control_conf.pb.txt
- ### 修改之后需要重新编译
- ### (docker里面) cd /tmp/ 可以查看相关的csv文件


# record记录数据：
- ### (docker里面) cd /apollo/data/bag
- ### ./record_PNC.sh


# 回放数据：
1. 链接服务器
    ssh yilin@10.65.32.18  密码：yilin
2. 进入自己目录下的Apollo，进入docker
    cd /data/yilin/apollo_simulate/apollo
    ./build.sh  选择d1
3. 启动dreamview
    ./scripts/bootstrap.sh
    浏览器访问10.65.32.18.8888
4. 
    source cyber/setup.sh
    cd data/bag/
5. ls查看相关的回放文件
    cd 进入该文件
    cyber_recorder play -f 对应数据


# 相关指令：
1. 查看monitor
    (docker里面)source cyber/setup.bash
    cyber_monitor
2. 查看启动的模块
    ps aux | grep mainboard
    sudo kill 对应id //杀死进程
3. git log 限制提交者
    git log --author 对应提交者
4. 查看dreamview
    docker外面
    ps aux | grep dreamview
    sudo kill 进程id
5. 命令行启动control调试
    docker里面
    mainboard -d modules/control/dag/control.dag

# problem
1. 自动驾驶模式下，杀掉了planning模块，车辆会继续前进到目标位置，此时介入人工模式，倒车一段距离后，切入自动驾驶模式，其仍会继续前进到此前的目标位置
    - 问题：由于control有一个writer、reader缓冲，会记录路径信息
    - 解决：对路径点加入时间戳判断

2. 自动驾驶模式下，车辆会出现猛打方向盘、猛加速的情况
    - 问题：根本原因是找到的纵向误差匹配点是反向路径点，导致航向误差较大，导致沿参考线的轨迹速度为反向，在计算速度误差时 由参考点的v - 该速度 得到一个较大的速度误差，所以会供给较大的油门；横向误差则是计算航向误差反馈时出现问题，其在选取前向预览点时，基于当前相对时间+预览时间 选取到不小于该时间的最小轨迹点，该轨迹点的朝向为反向，在相减时导致航向误差反馈较大，因此会给出较大的转向角度，根本原因是在下发的参考轨迹点上
    - 解决：对plannnig下发的路径点进行判断，对于出现角度或是速度跳变情况的参考轨迹，直接触发estop，认为这是一种错误情况，因为参考轨迹中连续的轨迹点之间应该是缓慢渐变的；对于纵向控制器，则在选取匹配点时，加入一个与车辆当前航向角的diff角度的判断，若角度差较大，则不能选取该路径点，会出现这种情况是因为有时候道路会有迂回的情况，就是车辆当前位置会有向前，向后两条轨迹，当车辆较靠近于向后的轨迹时，其不加反向限制则会选取到向后轨迹上的轨迹点

# work
