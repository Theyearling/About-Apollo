# LonController::ComputeControlCommand()
        更新trajectory_analyzer_对象 维护的轨迹点信息
        调用LonController::ComputeLongitudinalErrors()函数计算纵向误差，存储在debug中

        调用PIDController::SetPID()配置 位置PID控制器和 速度PID控制器
#

# LonController::ComputeLongitudinalErrors()