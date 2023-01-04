# LonController::ComputeControlCommand()
        更新trajectory_analyzer_对象 维护的轨迹点信息

        ts：0.01 
        preview_time = preview_window * ts = 20 * 0.01 = 0.2
        调用LonController::ComputeLongitudinalErrors()函数计算纵向误差，存储在debug中

        station_error_limit 位置误差限制 //默认配置2.0
        station_error_limited = common::math::Clamp(debug->preview_station_error(), -station_error_limit, station_error_limit); 前览位置误差，被 位置PID使用

        调用PIDController::SetPID()配置 位置PID控制器和 速度PID控制器
        当前线速度 <= 3 ? (low_speed_pid_conf  // kp = 0.2 // ki = 0.3 // kd = 0.0) : (high_speed_pid_conf // kp = 0.1 // ki = 0.3 // kd = 0.0)

        speed_offset，= PIDController::Control()，调用 位置PID控制器 计算获得的速度补偿

        speed_controller_input，速度控制器输入 = speed_offset + debug->preview_speed_error();
        speed_controller_input_limit，速度控制器输入限制，默认2.0
        speed_controller_input_limited，速度控制器输入限制，被 速度PID使用，= clamp(speed_controller_input, -2, 2)

        acceleration_cmd_closeloop，cmd加速度闭环控制，调用 速度PID控制器，= PIDController::Control()
        通过速度PID的对象speed_pid_controller_设置debug的pid_saturation_status参数，（实际并没有使用这个参数？！）

        slope_offset_compensation，斜坡偏移角补偿，DigitalFilter::Filter()，设置debug的slope_offset_compensation()参数，实际并没有用到

        acceleration_cmd，加速度控制指令，= acceleration_cmd_closeloop + debug->preview_acceleration_reference() + FLAGS_enable_slope_offset * debug->slope_offset_compensation();
        FLAGS_enable_slope_offset = false，即0 * 斜坡偏移角补偿

        throttle_lowerbound，油门下限，默认0.154
        brake_lowerbound，刹车下限，默认0.145

        calibration_vaule，查表校准值，调用Interpolation2D::Interpolate()进行插值（好难）
        acceleration_lookup，用于查询的加速度，= (+/-)acceleration_cmd 

        throttle_cmd，油门控制指令，= std::max(calibration_value, throttle_lowerbound) 或者 throttle_lowerbound

        brake_cmd，刹车控制指令，= std::max(-calibration_value, brake_lowerbound) 或者 brake_lowerbound

        设置一些debug参数，控制器不再使用

        设置ControlCommand *cmd：
        throttle
        brake
        acceleration
#

# LonController::ComputeLongitudinalErrors()
        vehicle_state获得车辆的状态信息
        matched_point：调用TrajectoryAnalyzer::QueryMatchedPathPoint()获得与车辆当前位置最接近的参考轨迹点，作为匹配点
        s_matcheded、s_dot_matcheded、d_matched、d_dot_matched：调用TrajectoryAnalyzer::ToTrajectoryFrame()计算获得
        current_control_time = Time::Now().ToSecond();当前时间(秒)
        preview_control_time = current_control_time + preview_time; 前预览时间
        reference_point参考点，调用TrajectoryAnalyzer::QueryNearestPointByAbsoluteTime()函数 基于当前时间 获得的路径点
        preview_point前预览点，调用TrajectoryAnalyzer::QueryNearestPointByAbsoluteTime()函数 基于前预览时间 获得的路径点

        设置debug的当前匹配点matched_point，当前参考点current_reference_point，前预览参考点preview_reference_point

        heading_error，航向误差，即车辆当前航向角 - 匹配点的朝向 （夹角）
        lon_speed：纵向速度 = 车辆线速度 * cos(heading_error)
        lon_acceleration：纵向加速度 = 车辆加速度 * cos(heading_error)

        设置debug：
        station_reference，参考位置，设置为参考点的s
        current_station，当前位置，设置为s_matcheded
        station_error，位置误差，设置为参考点的s - s_matcheded （纵向s上的位置误差）
        speed_reference，参考速度，设置为参考点的速度v
        current_speed，当前速度，设置为lon_speed
        speed_error，速度误差，设置为参考点的速度v - s_dot_matcheded
        acceleration_reference，参考加速度，设置为参考点的加速度a
        current_acceleration，当前加速度，设置为lon_acceleration
        acceleration_error，加速度误差，设置为参考点的加速度误差a - lon_acceleration / one_minus_kappa_lat_error
        preview_station_error，前览位置误差，设置为前预览点的s - s_matcheded
        preview_speed_error，前预览速度误差，设置为前预览点的v - s_dot_matcheded
        preview_speed_reference，前预览参考速度，设置为前预览点的v
        preview_speed_acceleration，前预览参考加速度，设置为前预览点的a

        jerk_reference，参考加速度的时间导数，设置为(debug->acceleration_reference() - previous_acceleration_reference_) / ts;
        lon_jerk，当前加速度的时间导数，设置为(debug->current_acceleration() - previous_acceleration_) / ts;


## TrajectoryAnalyzer::ToTrajectoryFrame()
        dx = 车辆当前x - 匹配点x
        dy = 车辆当前y - 匹配点y
        cos_ref_theta/sin_ref_theta：匹配点的朝向的余弦/正弦值
        d_matched = cross_rd_nd = cos_ref_theta * dy - sin_ref_theta * dx ：dx、dy分解后横向上的分量，两个分解分量方向相反 | //向量 (cos_ref_theta, sin_ref_theta) 和 (dx, dy) 之间差角的正弦值
        dot_rd_nd = dx * cos_ref_theta + dy * sin_ref_theta; dx，dy分解后在纵向上的分量，两个分量方向相同 | //向量 (cos_ref_theta, sin_ref_theta) 和 (dx, dy) 之间差角的 cos
        s_matcheded = 匹配点的s + dot_rd_nd
        delta_theta = 车辆当前朝向 - 匹配点的朝向
        d_dot_matched = 车辆线速度 * sin(delta_theta)；横向距离变化率 d(d)/d(t)；即速度
        one_minus_kappa_r_d = 1 - ref_point.kappa() * (*ptr_d); 和曲率相关，具体不太清楚
        s_dot_matcheded = 车辆线速度 * cos(delta_theta) / one_minus_kappa_r_d，沿参考轨迹的纵向速度
#

# PIDController::Control()

#

# Interpolation2D::Interpolate()
        

#
