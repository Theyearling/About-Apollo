# canbus::Chassis *chassis_
        在modules/canbus/proto/chassis.proto中定义
#
# ControlComponent::Proc()
## Reader::Observe()
        获取Blocker存储的数据，即维护reader的blocker成员的私有变量
        void Reader<MessageT>::Observe() 调用Blocker的Observe()
        得到存储的消息队列： observed_msg_queue_ = published_msg_queue_;

- ### Blocker::published_msg_queue_
        std::__cxx11::list<std::shared_ptr<T>>类型
        Blocker::Publish()-->Enqueue()-->push_front(MessagePtr& msg)插入数据

- ### Blocker::Equeue()
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

- ### .trajectory() 
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
        

- ### control::ControlCommand *control_command
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

- ### control_list_
        std::vector<std::unique_ptr<Controller>> controller_list_;
        在ControllerAgent::InitializeConf()中
            control_conf_->active_controllers()提取激活的控制器
            压入对应的Controller对象（通过工厂模式创建对象）

- ### active_controllers
        在modules/control/proto/control_conf.proto中定义
        在modules/control/conf/control_conf.pb.txt中赋值：默认有俩：
        active_controllers: LAT_CONTROLLER
        active_controllers: LON_CONTROLLER
#

# LatController::Init()
        初始化横向控制器
        调用LatController::LoadControlConf()获取相应的参数

        初始化A矩阵、B矩阵、state状态矩阵、k矩阵、r矩阵、q矩阵

        调用LatController::InitializeFilters()初始化滤波器

        调用LatController::LoadLatGainScheduler()加载横向误差增益调度器，里面有曲线样条拟合，在control中并没有启用该增益调度器   

        enable_leadlag_，是否启用 向前/滞后控制器， true
        调用LeadlagController::Init()初始化滞后控制器
                

- ### basic_state_size_ == 4
- ### preview_window_ == 0
- ### matrix_size = basic_state_size_ + preview_window_ == 4
- ### Eigen::MatrixXd matrix_a_coeff_ //对应着具有V的位置的系数

## LatController::LoadControlConf()
        vehicle_param_，车辆参数，在modules/common/configs/proto/vehicle_config.proto定义，/data/vehicle_param.pb.txt初始化
        ts_，control_conf->lat_controller_conf().ts()，0.01，控制时间间隔
        cf_，control_conf->lat_controller_conf().cf()，155494.663，前轮转角刚度
        cr_，control_conf->lat_controller_conf().cr()，155494.663，后轮转角刚度
        preview_window_，前览窗口，0
        lookahead_station_low_speed_ = lookahead_station_high_speed_，前向预览的横向误差的纵向长度，1.4224
        lookback_station_low_speed_ = lookback_station_high_speed_，后向预览的横向误差的纵向长度，2.8448
        wheelbase_，前后轮中心距离，2.8448
        steer_ratio_，方向盘转数和车轮转数之间的比率，16
        steer_single_direction_max_degree_，车轮最大转角，= vehicle_param_.max_steer_angle() / M_PI * 180 = 8.20304748437 / M_PI * 180
        max_lat_acc_，最大理论横向加速度限制转向，5.0
        low_speed_bound_，纵向低速边界，3.0
        low_speed_window_，纵向低速/高速切换窗口，1.0
        mass_fl/mass_fr/mass_rl/mass_rr，前后左右轮质量，520
        mass_front，前轮重量，mass_rear，后轮重量，mass_，车轮总重量，2080
        lf_，前悬长度（质心到前轴中心的长度） = wheelbase_ * (1.0 - mass_front / mass_)
        lr_，后悬长度（质心到后轴中心的长度） = wheelbase_ * (1.0 - mass_rear / mass_)
        iz_，转动惯量，= lf_ * lf_ * mass_front + lr_ * lr_ * mass_rear

        lqr_eps，LQR求解器的参数，计算阈值，0.01
        lqr_max_iteration_，LQR求解器的参数，迭代次数，150
        query_relative_time，查询相对时间，用于求解target_point，0.8
        minimum_speed_protection_，最小速度保护，保证车辆有>=最小的线速度，0.1
##

## LatController::InitializeFilters()
        std::vector<double> den(3, 0.0); //denominators滤波器分母系数
        std::vector<double> num(3, 0.0); //numerators滤波器分子系数
        调用common::LpfCoefficients()填充滤波器系数
                cutoff_frep，频率截止参数，以滤除高频信号，10
        调用DigitalFilter::set_coefficients()传递上一步填充的滤波器系数

        调用common::MeanFilter()初始化横向误差、航向误差滤波器，后续并没有用到


### common::LpfCoefficients()
        denominators = [1.0, 
                        2.0 * (0.01pi^2 - 1.0) / (1.0 + sqrt(2.0) * 0.1pi + 0.01pi^2),
                        (1.0 - sqrt(2.0) * 0.1pi + 0.01pi^2) / (1.0 + sqrt(2.0) * 0.1pi + 0.01pi^2)]
        numeriators = [0.01pi / (1.0 + sqrt(2.0) * 0.1pi + 0.01pi^2),
                       2.0 * 0.01pi / (1.0 + sqrt(2.0) * 0.1pi + 0.01pi^2),
                       0.01pi / (1.0 + sqrt(2.0) * 0.1pi + 0.01pi^2)]

### DigitalFilter::set_coefficients()
        设置滤波器分子、分母的系数，并resize()分子、分母变量的size
        denominators_存储分母系数，y_values_分母变量，resize(3,0.0)
        numerators_存储分子系数，x_values_分子变量，resize(3,0.0)
##

## LeadlagController::Init()
        previous_output_ = 0.0，上一个输出
        previous_innerstate_ = 0.0，上一个内部状态
        innerstate_ = 0.0，内部状态
        innerstate_saturation_high_ = 3000，高内部状态饱和度（边界）
        innerstate_saturation_low_ = -3000，低内部状态饱和度（边界）

#

# LatController::ComputeControlCommand()

        调用DependencyInjector::vhicle_state()获取对象
        调用trajectory_analyzer_ = std::move(TrajectoryAnalyzer())获取对象

        满足条件(((FLAGS_trajectory_transform_to_com_reverse &&vehicle_state->gear() == canbus::Chassis::GEAR_REVERSE) ||(FLAGS_trajectory_transform_to_com_drive && vehicle_state->gear() == canbus::Chassis::GEAR_DRIVE)) && enable_look_ahead_back_control_)时——>
            调用TrajectoryAnalyzer::TrajectoryTransformToCOM()-->TrajectoryAnalyzer::ComputeCOMPosition() 将规划轨迹的坐标从后轴中心转换为质心(一些矩阵运算，难顶)

        构造A矩阵：4 x 4（前进、倒挡两种状态的A矩阵有所不同）

        构造B矩阵：4 x 1

        构造Q矩阵：4 x 4

        构造R矩阵 R = [1]

        调用LatController::UpdateState()构造状态矩阵：4 x 1

        调用apollo::common::math::SolveLQRProblem()计算K矩阵：1 x 4

        计算反馈误差：feedback = - K * state --> u：1 x 1 

        调用LatController::ComputeFeedForward()计算前向误差

        计算车辆转角steer_angle：反馈误差 + 前向误差 + 反馈误差增强
        用给定的最大横向加速度限制steer_angle

        设置一些debug参数，控制器不再使用

        设置ControlCommand *cmd：
        steering_target，目标转向角，以满刻度的百分比表示 [-100, 100]，设置为common::math::Clamp(steer_angle, pre_steer_angle_ - steer_diff_with_max_rate, pre_steer_angle_ + steer_diff_with_max_rate)
        steer_diff_with_max_rate，默认100
        steering_rate，目标非定向转向率，以每秒满量程的百分比表示 [0, 100]，设置为 100

- ### enable_gain_scheduler 置为true

<br>

- ### SimpleLateralDebug *debug
        在modules/control/proto/control_cmd.proto里定义


## DependencyInjector::vhicle_state()
        获得apollo::common::VehicleStateProvider对象 return &vehicle_state_;
        存有车辆状态的基本信息

- ### VehicleStateProvider类的私有成员VehicleState vehicle_state_
        VehicleState在modules/common/vehicle_state/proto/vehicle_state.proto中定义

## TrajectoryAnalyzer::TrajectoryAnalyzer()构造函数
        将apollo::planning::ADCTrajectory的轨迹点（repeated apollo.common.TrajectoryPoint trajectory_point）拷贝到成员变量std::vector<common::TrajectoryPoint> trajectory_points_中

## LatController::UpdateDrivingOrientation()
- ### reverse_heading_control 默认false
        反向驱动模式的逻辑
        driving_orientation_ = vehicle_state->heading(); //赋值为车辆航向角，作为theta参数供给LatController::ComputeLateralErrors()使用
        driving_orientation_在LatController::ComputeControlCommand()中会更新赋值给debug的heading

## LatController::UpdateState
        use_navigation_mode //在导航模式下使用相对位置，默认false
        调用ComputeLateralErrors()计算横向误差、横向误差率、航向误差、航向误差率，记录在debug中
        enable_look_ahead_back_control_--true
        借用debug构造状态矩阵[横向误差、横向误差率、航向误差、航向误差率]^T

- ### LatController::enable_look_ahead_back_control_ == true
        enable_look_ahead_back_control_ = control_conf_->lat_controller_conf().enable_look_ahead_back_control();
        // 在/modules/control/proto/定义
        // 在/modules/control/conf/中被设置为true

- ### VehicleStateProvider::ComputeCOMPosition
        state_transform_to_com_reverse //默认false
        state_transform_to_com_drive //默认true
        将坐标从后轴中心转到车辆质心

## LatController::UpdateMatrix()
        结合矩阵A的系数矩阵matrix_a_coeff_构造矩阵A
        A[0,2]因前进或倒车两种模式有所不同
        构造matrix_ad_，供UpdateMatrixCompound()使用

## LatController::UpdateMatrixCompound()
        预览模型的复合型矩阵赋值和更新
        构造matrix_adc_，matrix_bdc_，真正供LQR所使用的A、B矩阵

## LatController::ComputeFeedForward()
        参数：
        lr_ / lf_：后 / 前 悬长度
        cr_ / cf_：后 / 前 轮侧偏力
        wheelbase：车轴长度
        mass_：车辆重量
        ref_curvature：参考曲率
        matrix_k_(0, 2)：K3
            
        分为前进和倒车两种情况：
        kv = lr_ * mass_ / 2 / cf_ / wheelbase_ - lf_ * mass_ / 2 / cr_ / wheelbase_;
        倒车：
        wheelbase_ * ref_curvature
        前进：
        wheelbase_ * ref_curvature + kv * v * v * ref_curvature - matrix_k_(0, 2) * (lr_ * ref_curvature - lf_ * mass_ * v * v * ref_curvature / 2 / cr_ / wheelbase_))

#


# LatController::ComputeLateralErrors()
        计算横向误差、横向误差率、航向误差、航向误差率，记录在debug中
        调用TrajectoryAnalyzer::QueryNearestPointByPosition()获取最接近给定位置的轨迹点，作为参考点
$$
\left \{ 
\begin{array}{c}
e_1=d_y \ast cos\ \theta_{des} - d_x \ast sin\ \theta_{des}  \\
{\dot e_1}=V_x \ast sin\ \Delta\theta = V_x \ast sin\ e_2  \\ 
e_2=\theta - \theta_{des} \\ 
{\dot e_2} = {\dot \theta} - {\dot \theta_{des}}
\end{array}
\right.
$$
        target_point 最接近车辆当前位置的参考路径的路径点
        target_point 的x, y, theta 设置debug当前参考路径点x, y, ref_heading
        dx / dy 参考点与车辆当前位置的差值
        cos_target_heading / sin_target_heading 参考点朝向
        lateral_error = cos_target_heading * dy - sin_target_heading * dx 设置debug横向误差
        heading_error 车辆速度V方向 与 参考轨迹点 朝向 的夹角，设置debug的heading_error
        lookahead_station 前视距离，默认1.4224
        lookback_station 后视距离，默认2.8448
        heading_error_feedback 考虑前/后视的航向误差，状态矩阵真正使用的航向误差，设置debug的heading_error_feedback
        lateral_error_feedback 考虑前/后视的横向误差，状态矩阵真正使用的横向误差，设置debug的lateral_error_feedback
        设置debug的lateral_error_rate，横向误差率 = 车辆V * sin(heading_error)
        设置debug的lateral_acceleration，横向误差率的时间导数 = 车辆加速度 a * sin(heading_error)
        设置debug的lateral_jert，横向误差率的二阶导数 = （当前误差率导数 - 上一个误差率导数）/ t
        设置debug的heading_rate，= 车辆角速度
        设置debug的ref_heading_rate, 期望的车辆转角速度 = 参考点线速度 * 路径曲率kappa
        设置debug的heading_error_rate，航向误差率 = heading_rate - ref_heading_rate
        设置debug的heading_acceleration，航向加速度（航向速度对于时间的导数） = （heading_rate - 前一个heading_rate）/ t
        设置debug的ref_heading_acceleration，参考的航向加速度 = （ref_heading_rate - 前一个ref_heading_rate）/ t
         设置debug的heading_error_acceleration，航向加速度误差 = heading_acceleration - ref_heading_acceleration
         设置debug的heading_jerk，车辆角速度的二阶导数 = (heading_acceleration - ref_heading_acceleration) / t
         设置debug的ref_heading_jerk， 车辆参考角速度的二阶导数 = (ref_heading_acceleration - ref_heading_acceleration) / t
         设置debug的heading_error_jerk，二阶导数误差 = (heading_error_jerk - ref_heading_error_jerk)
         设置debug的curvature，路径曲率 = 参考路径点记录的曲率


- ### query_time_nearest_point_only 默认false
- ### enable_navigation_mode_position_update 默认true
- ### enable_navigation_mode_error_filter 默认false
        
## TrajectoryAnalyzer::QueryNearestPointByPosition()
        遍历trajectory_points_中元素，计算其与当前位置的欧氏距离，选取最短的轨迹点
# 

# apollo::common::math::SolveLQRProblem()
        解黎卡提方程，求出K矩阵
        加了一个M（混合）矩阵 M.rows() == Q.rows() && M.cols() == R.cols() 被初始化为0矩阵，感觉是没有作用
        初始 P = Q
$$
{\dot P} = A^T \ast \ P\ \ast \ A\ - (A^T \ast \ P\ \ast \ B\ +\ M)\ \ast (R\ +\ B^{T}\ \ast\ P\ \ast\ B)^{-1}\ \ast\ (B^T\ \ast\ P\ \ast\ A\ +\ M^T)\ +\ Q \\
P\ =\ {\dot P}\; \;\;\;\;\;\; 更新矩阵P
$$
$$
K\ =\ (R\ +\ B^{T}\ \ast\ P\ \ast\ B)^{-1}\ \ast\ (B^T\ \ast\ P\ \ast\ A\ +\ M^T)
$$
#