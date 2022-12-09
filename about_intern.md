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

