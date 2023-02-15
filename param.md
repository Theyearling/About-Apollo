<table border="0">
    <caption>Lat_controller</caption>
    <tr>
        <th>参数</th>
        <th>作用</th>
        <th>调节</th>
        <th>效果</th>
    </tr>
    <tr>
        <td>ts_</td>
        <td>采样时间，用于计算预览时间</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>cf_</td>
        <td>前轮侧偏刚度</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>cr_</td>
        <td>后轮侧偏刚度</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>preview_window_</td>
        <td>预览窗口（向前看的控制周期数），用于计算预览时间</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>lookahead_station_low_speed_</td>
        <td>纵向长度用于向前驱动期间的前瞻横向误差估计，默认1.4224</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>lookback_station_low_speed_</td>
        <td>纵向长度用于向后驱动期间的回顾横向误差估计，默认2.8448</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>lookahead_station_high_speed_</td>
        <td>纵向长度用于向前驱动期间的前瞻横向误差估计，默认2.8448</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>wheelbase_</td>
        <td>轴距，前后轮距离</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>steer_ratio_</td>
        <td>方向盘转数与车轮转数之间的比率</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>steer_single_direction_max_degree_</td>
        <td>最大转弯角度</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>max_lat_acc_</td>
        <td>将转向限制为最大理论横向加速度</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>low_speed_bound_</td>
        <td>低/高速控制器切换速度</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>low_speed_window_</td>
        <td>低速/高速切换转换窗口</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>mass_fl</td>
        <td>前左轮质量</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>mass_fr</td>
        <td>前右轮质量</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>mass_rl</td>
        <td>后左轮质量</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>mass_rr</td>
        <td>后右轮质量</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>lf_</td>
        <td>前轴到质心的距离，前悬长 = wheelbase_ * (1.0 - mass_front / mass_)</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>lr_</td>
        <td>后轴到质心的距离，后悬长 = wheelbase_ * (1.0 - mass_rear / mass_)</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>iz_</td>
        <td>转动惯量 = lf_ * lf_ * mass_front + lr_ * lr_ * mass_rear</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>Q</td>
        <td>半正定矩阵，Q值大，则状态x(t)就以更快的速度衰减到0</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
    <tr>
        <td>R</td>
        <td>正定矩阵，R值大，则更关注输入变量u(t)，状态衰减将减慢</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
    <tr>
        <td>lqr_eps_</td>
        <td>lqr 求解器的收敛阈值</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>lqr_max_iteration_</td>
        <td>lqr 求解的最大迭代</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>query_relative_time_</td>
        <td>查询的相对时间，用于查询其绝对时间最接近给定时间的轨迹点</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>minimum_speed_protection_</td>
        <td>最小速度保护</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>enable_look_ahead_back_control_</td>
        <td>前瞻控制器开关</td>
        <td></td>
        <td></td>
    </tr>
    <tr rowspan='2'>
        <td rowspan='2'>enable_leadlag</td>
        <td rowspan='2'>是否启用超前滞后开关，默认false</td>
        <td rowspan='1'>true</td>
        <td rowspan='2'>figure</td>
    </tr>
    <tr>
        <td rowspan='1'>false</td>
    </tr>
    <tr>
        <td>FLAGS_enable_feedback_augment_on_high_speed</td>
        <td>启用对高速横向误差的增强控制，在modules/control/conf/control.conf中真正赋值</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>FLAGS_set_steer_limit</td>
        <td>启用转角限制,默认false，在modules/control/conf/control.conf中真正赋值</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>FLAGS_enable_maximum_steer_rate_limit</td>
        <td>启用最大转角率限制，默认false，在modules/control/conf/control.conf中真正赋值</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>den，num</td>
        <td>转角角度滤波器系数，与ts和cutoff_freq相关</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>cutoff_freq</td>
        <td>截止频率，用于初始化滤波系数</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>FLAGS_lock_steer_speed</td>
        <td>默认0.081，若当前速度小于改值，则用上一次的转角角度进行输出</td>
        <td></td>
        <td></td>
    </tr>
</table>
<br>
<table border="0">
    <caption>Lon_controller</caption>
    <tr>
        <th>参数</th>
        <th>作用</th>
        <th>调节</th>
        <th>效果</th>
    </tr>
    <tr>
        <td>ts</td>
        <td>采样时间，用于计算预览时间</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>preview_time</td>
        <td>预览时间 = preview_window * ts</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>enable_leadlag</td>
        <td>是否启用超前滞后开关，默认false</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>station_error_limit</td>
        <td>位置误差限制，限制会影响位置误差速度补偿speed_offset</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>FLAGS_enable_speed_station_preview</td>
        <td>启用速度/位置 预览， 默认false，在modules/control/conf/control.conf中真正赋值</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>speed_controller_input_limit</td>
        <td>速度控制器输入限制，限制会影响闭环加速度反馈acceleration_cmd_closeloop，进而影响加速度输出acceleration_cmd</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>FLAGS_enable_slope_offset</td>
        <td>启用斜坡偏移补偿，会影响加速度输出acceleration_cmd，默认false</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>throttle_lowerbound</td>
        <td>油门下限，max(vehicle_param_.throttle_deadzone(),
               lon_controller_conf.throttle_minimum_action())</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>brake_lowerbound</td>
        <td>刹车下限，max(vehicle_param_.brake_deadzone(),
               lon_controller_conf.brake_minimum_action())</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>FLAGS_use_preview_speed_for_table</td>
        <td>启用预览速度查询标定表，影响标定表数据calibration_value，进而影响油门刹车，默认false</td>
        <td></td>
        <td></td>
    </tr>
    <tr rowspan="6">
        <td rowspan="6">station_pid</td>
        <td rowspan="2">kp</td>
        <td rowspan="1">0.3</td>
        <td rowspan="2">figure</td>
    </tr>
    <tr>
        <td rowspan="1">0.5</td>
    </tr>
    <tr>
        <td rowspan="2">ki</td>
        <td rowspan="1">0.3</td>
    </tr>
    <tr>
        <td rowspan="1">0.5</td>
    </tr>
    <tr>
        <td rowspan="2">kd</td>
        <td rowspan="1">0.0</td>
    </tr>
    <tr>
        <td rowspan="1">0.3</td>
    </tr>
    <tr rowspan="3">
        <td rowspan="3">speed_pid</td>
        <td rowspan="1">kp</td>
    </tr>
    <tr>
        <td rowspan="1">ki</td>
    </tr>
    <tr>
        <td rowspan="1">kd</td>
    </tr>
</table>
<br>
<table style="text-align: center;">
    <caption>Simulink</caption>
    <tbody>
        <tr aria-rowspan="18">
            <td rowspan="3">station_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.2</td>
            <td rowspan="17"><img src="/image/param/simulink_3.png"></td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">speed_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">2.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.3</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">lateral_err_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="5">LQR</td>
            <td rowspan="4">Q</td>
            <td rowspan="1">0.05</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">R</td>
            <td rowspan="1">1</td>
        </tr>
    </tbody>
    <br>
    <tbody>
        <tr aria-rowspan="18">
            <td rowspan="3">station_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.2</td>
            <td rowspan="17"><img src="/image/param/change_R_0.5.png"></td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">speed_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">2.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.3</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">lateral_err_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="5">LQR</td>
            <td rowspan="4">Q</td>
            <td rowspan="1">0.05</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">R</td>
            <td rowspan="1">0.5</td>
        </tr>
    </tbody>
    <br>
    <tbody>
        <tr aria-rowspan="18">
            <td rowspan="3">station_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.2</td>
            <td rowspan="17"><img src="/image/param/change_R_0.1.png"></td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">speed_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">2.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.3</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">lateral_err_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="5">LQR</td>
            <td rowspan="4">Q</td>
            <td rowspan="1">0.05</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">R</td>
            <td rowspan="1">0.1</td>
        </tr>
    </tbody>
    <br>
    <tbody>
        <tr aria-rowspan="18">
            <td rowspan="3">station_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.2</td>
            <td rowspan="17"><img src="/image/param/change_R_5.png"></td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">speed_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">2.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.3</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">lateral_err_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="5">LQR</td>
            <td rowspan="4">Q</td>
            <td rowspan="1">0.05</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">R</td>
            <td rowspan="1">5</td>
        </tr>
    </tbody>
    <br>
    <tbody>
        <tr aria-rowspan="18">
            <td rowspan="3">station_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.2</td>
            <td rowspan="17"><img src="/image/param/change_R_10.png"></td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">speed_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">2.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.3</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">lateral_err_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="5">LQR</td>
            <td rowspan="4">Q</td>
            <td rowspan="1">0.05</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">R</td>
            <td rowspan="1">10</td>
        </tr>
    </tbody>
    <br>
    <tr aria-rowspan="18">
            <td rowspan="3">station_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.2</td>
            <td rowspan="17"><img src="/image/param/simulink_4.png"></td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">speed_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">2.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.3</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">lateral_err_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="5">LQR</td>
            <td rowspan="4">Q</td>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">R</td>
            <td rowspan="1">1</td>
        </tr>
    </tbody>
    <br>
    <tbody>
        <tr aria-rowspan="18">
            <td rowspan="3">station_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.6</td>
            <td rowspan="17"><img src="/image/param/simulink_2.png"></td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">speed_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">1.2</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.8</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.8</td>
        </tr>
        <tr>
            <td rowspan="3">lateral_err_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="5">LQR</td>
            <td rowspan="4">Q</td>
            <td rowspan="1">0.05</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">R</td>
            <td rowspan="1">1</td>
        </tr>
    </tbody>
    <br>
    <tbody>
        <tr aria-rowspan="18">
            <td rowspan="3">station_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.6</td>
            <td rowspan="17"><img src="/image/param/simulink_1.png"></td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="3">speed_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">1.6</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.8</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">1.8</td>
        </tr>
        <tr>
            <td rowspan="3">lateral_err_pid</td>
            <td rowspan="1">kp</td>
            <td rowspan="1">0.8</td>
        </tr>
        <tr>
            <td rowspan="1">ki</td>
            <td rowspan="1">0.5</td>
        </tr>
        <tr>
            <td rowspan="1">kd</td>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="5">LQR</td>
            <td rowspan="4">Q</td>
            <td rowspan="1">0.05</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">1.0</td>
        </tr>
        <tr>
            <td rowspan="1">0.0</td>
        </tr>
        <tr>
            <td rowspan="1">R</td>
            <td rowspan="1">1</td>
        </tr>
    </tbody>
    
</table>