# ControlComponent::Proc()
## Reader::Observe()
        获取Blocker存储的数据，即维护reader的blocker成员的私有变量
        void Reader<MessageT>::Observe() 调用Blocker的Observe()
        得到存储的消息队列： observed_msg_queue_ = published_msg_queue_;

### Blocker::published_msg_queue_
        std::__cxx11::list<std::shared_ptr<T>>类型
        Blocker::Publish()-->Enqueue()-->push_front(MessagePtr& msg)插入数据

### Blocker::Equeue()
        （加锁）前插后出
        BlockerAttr维护着一个通道容量，若当前published_msg_queue_的长度大于通道容量，则队尾出队pop_back()

## Reader::GetLatestObserved()
        std::shared_ptr<MessageT> Reader<MessageT>::GetLatestObserved() const
        -->Blocker::GetlatestObservedPtr()
        由于observed_msg_queue_被定义为私有成员变量，类外不能够直接访问，因此借助该函数，得到存储链表的头部元素，observed_msg_queue_.front();
        存储为空时返回nullptr

## ControlComponent::OnChassis() / OnPlanning() / OnLocallization()
        （加锁）更新最新的chassis / trajectory / localization数据
        latest_chassis_.CopyFrom(*chassis);
        latest_trajectory_.CopyFrom(*trajectory);
        latest_localization_.CopyFrom(*localization);

## LocalView local_view_
        ControlComponent::Proc()中构造local_view_数据，包括OnChassis()-->chassis、OnPlanning()-->trajectory、OnLocalization()-->localization

### .trajectory() 
            //ReferenceLineInfo::SetTrajectory(const DiscretizedTrajectory& trajectory) 构造一个离散轨迹对象
                ——在planner中调用，进行离散轨迹赋值

                · class DiscretizedTrajectory : public std::vector<common::TrajectoryPoint>
                    TrajectoryPoint、ADCTrajectory等相关的数据在modules/common/porto里面定义

            //const DiscretizedTrajectory& ReferenceLineInfo::trajectory() const 返回一个离散轨迹的引用

## Writer::Write()
        Writer::Write(const MessageT& msg)-->Writer::Write(const std::shared_ptr<MessageT>& msg_ptr)-->Transmitter::Transmit(const MessagePtr& msg)-->Transmitter::Transmit(const MessagePtr& msg, const MessageInfo& msg_info) = 0;(纯虚函数不知道具体在哪个类里实现)

# ControlComponent::ProduceControlCommand()
        ControlComponent::Proc()-->ControlComponent::ProduceControlCommand()-->ControllerAgent::ComputeControlCommand--> /Cotroller::ComputeControlCommand()定义的纯虚函数，在子类LatController覆盖实现/ --> LatController::ComputeControlCommand()
        获得控制指令control_command

### control::ControlCommand *control_command
        在modules/control/proto/control_cmd.proto中定义

## ControlComponent::CheckInput()
        检查local_view_的trajectory：
            是否有路径点，是否触发estop
            线速度 和 线性加速度

## CheckTimestamp()
        检查localization、chassis、trajectory的时间戳
        与miss_num * period比较，若时间差超过这个值，则说明丢失数据，超时

## ControllerAgent::ComputeControlCommand()
        从control_list_中读取Controller对象，调用对应的ComputeControlCommand()获取控制指令

### control_list_
        std::vector<std::unique_ptr<Controller>> controller_list_;
        在ControllerAgent::InitializeConf()中
            control_conf_->active_controllers()提取激活的控制器
            压入对应的Controller对象（通过工厂模式创建对象）

### active_controllers
        在modules/control/proto/control_conf.proto中定义
        在modules/control/conf/control_conf.pb.txt中赋值：默认有俩：
        active_controllers: LAT_CONTROLLER
        active_controllers: LON_CONTROLLER

# LatController::ComputeControlCommand

        调用DependencyInjector::vhicle_state()获取对象
        调用std::move(TrajectoryAnalyzer())获取对象

        满足条件(((FLAGS_trajectory_transform_to_com_reverse &&vehicle_state->gear() == canbus::Chassis::GEAR_REVERSE) ||(FLAGS_trajectory_transform_to_com_drive && vehicle_state->gear() == canbus::Chassis::GEAR_DRIVE)) && enable_look_ahead_back_control_)时——>
            调用TrajectoryAnalyzer::TrajectoryTransformToCOM()-->TrajectoryAnalyzer::ComputeCOMPosition() 将规划轨迹的坐标从后轴中心转换为质心(一些矩阵运算，难顶)

        构造A矩阵（前进、倒挡两种状态的A矩阵有所不同）

### Eigen::MatrixXd matrix_a_coeff_;
        对应着具有V的位置的系数 

## DependencyInjector::vhicle_state()
        获得apollo::common::VehicleStateProvider对象 return &vehicle_state_;
        存有车辆状态的基本信息

### VehicleStateProvider类的私有成员VehicleState vehicle_state_
        VehicleState在modules/common/vehicle_state/proto/vehicle_state.proto中定义

## TrajectoryAnalyzer::TrajectoryAnalyzer()构造函数
        将apollo::planning::ADCTrajectory的轨迹点（repeated apollo.common.TrajectoryPoint trajectory_point）拷贝到成员变量std::vector<common::TrajectoryPoint> trajectory_points_中

## LatController::UpdateDrivingOrientation()

## 

## LatController::UpdateState

# LatController::Init()
        初始化横向控制器

