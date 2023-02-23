tiledlayout(3,1)

% 提取车辆质心轨迹 + 规划的轨迹点
% mass_and_trajectory = csvread("C:\Users\yilin.li\Desktop\坐标数据\20230210\第二圈\mass_center.csv",1,0); % 下标从0开始
% real_vehicle_data = [];
% trajectory = {};
% trajectory_data = [];
% for i = 1:length(mass_and_trajectory(:,1))
%     if mass_and_trajectory(i,3) ~= 0
%         real_vehicle_data = [real_vehicle_data; mass_and_trajectory(i,:)];
%         if ~isempty(trajectory_data)
%             trajectory(end+1) = {trajectory_data};
%             trajectory_data = [];
%         end
%     elseif mass_and_trajectory(i,1) == -1 && mass_and_trajectory(i,2) == -1 || mass_and_trajectory(i,1) == 0 && mass_and_trajectory(i,2) == 0 || mass_and_trajectory(i,1) > 1.0e+10
%         continue;
%     else
%         trajectory_data = [trajectory_data;mass_and_trajectory(i,:)];
%     end
% end
% if ~isempty(trajectory_data)
%     trajectory(end+1) = {trajectory_data};
% end

% 实车质心坐标数据
% 用于数据归一化
mid_data = [mid_x,mid_y];

% real_vehicle_data(:,1) = real_vehicle_data(:,1) - mid_data(1,1);
% real_vehicle_data(:,2) = real_vehicle_data(:,2) - mid_data(1,2);

nexttile([2 1]);
% plot(real_vehicle_data(:,1), real_vehicle_data(:,2),'r');
% hold on;

% 定位数据
localization_data = csvread("C:\Users\yilin.li\Desktop\坐标数据\20230223\no_change\garage.csv", 1, 0);
localization_data(:,1) = localization_data(:,1) - mid_data(1,1);
localization_data(:,2) = localization_data(:,2) - mid_data(1,2);
plot(localization_data(:,1), localization_data(:,2),'k');
hold on;
% routing数据 
routing_data = csvread("C:\Users\yilin.li\Desktop\坐标数据\20230210\routing.csv", 1, 0);
routing_data(:,1) = routing_data(:,1) - mid_data(1,1);
routing_data(:,2) = routing_data(:,2) - mid_data(1,2);
plot(routing_data(:,1), routing_data(:,2),'m');

% garage_data = csvread("C:\Users\yilin.li\Desktop\坐标数据\20230202\localization.csv", 1, 0);
% garage_data(:,1) = garage_data(:,1) - mid_data(1,1);
% garage_data(:,2) = garage_data(:,2) - mid_data(1,2);
% plot(garage_data(:,1), garage_data(:,2),'r');
% 
% legend('boxoff');
% legend('real\_Point', 'routing\_Point','last\_Point');

% 画规划的轨迹点（取了50个点）
% color_arr = ['y','m'];
% for i=1:length(trajectory)
%     trajectory{i}(:,1) = trajectory{i}(:,1) - mid_data(1,1);
%     trajectory{i}(:,2) = trajectory{i}(:,2) - mid_data(1,2);
%     hold on;
%     plot(trajectory{i}(:,1), trajectory{i}(:,2), color_arr(1,mod(i,2)+1));
% end

hold off;

% 画误差曲线 及 均值
nexttile;
diff = [];
for i=1:length(routing_data(:,1))
    min_diff = 100;
    for j=1:length(localization_data(:,1))
        mid_diff = (routing_data(i,1)-localization_data(j,1))^2 + (routing_data(i,2)-localization_data(j,2))^2;
        if mid_diff < min_diff
            min_diff = mid_diff;
        end
    end
    diff = [diff,sqrt(min_diff)];
end
plot(diff);
hold on;
plot(mean(diff)*ones(1,length(routing_data(:,1))));

hold off;
