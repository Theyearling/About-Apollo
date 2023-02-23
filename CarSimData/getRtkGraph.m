tiledlayout(3,1)

% rtk_recorder 回放数据时记录的轨迹数据
garage_data_1 = csvread("C:\Users\yilin.li\Desktop\坐标数据\20230217\rtk\garage_2.csv", 1, 0);
% 用于数据归一化
mid_data = [mid_x,mid_y];

garage_data_1(:,1) = garage_data_1(:,1) - mid_data(1,1);
garage_data_1(:,2) = garage_data_1(:,2) - mid_data(1,2);

nexttile([2 1]);
plot(garage_data_1(:,1), garage_data_1(:,2),'r');
hold on;

% rtk_recorder 回放数据时记录的轨迹数据
garage_data_2 = csvread("C:\Users\yilin.li\Desktop\坐标数据\20230217\rtk\garage_1.csv", 1, 0);
garage_data_2(:,1) = garage_data_2(:,1) - mid_data(1,1);
garage_data_2(:,2) = garage_data_2(:,2) - mid_data(1,2);
% plot(garage_data_2(:,1), garage_data_2(:,2),'k');

% rtk_recorder 人工控制下车辆行进的数据
garage_data = csvread("C:\Users\yilin.li\Desktop\坐标数据\20230217\rtk\garage.csv", 1, 0);
garage_data(:,1) = garage_data(:,1) - mid_data(1,1);
garage_data(:,2) = garage_data(:,2) - mid_data(1,2);
plot(garage_data(:,1), garage_data(:,2),'m');

% legend('boxoff');
% legend('first\_diff','second\_diff','first\_mean','second\_mean');

hold off;

% 画误差曲线 及 均值
nexttile;
diff = [];
%sec_diff = [];
for i=1:length(garage_data_1(:,1))
    min_diff = 100;
    %sec_min = 100;
    for j=1:length(garage_data(:,1))
        mid_diff = (garage_data_1(i,1)-garage_data(j,1))^2 + (garage_data_1(i,2)-garage_data(j,2))^2;
        % sec_mid = (routing_data(i,1)-localization_data(j,1))^2 + (routing_data(i,2)-localization_data(j,2))^2;
%         if sec_mid < sec_min
%            sec_min = sec_mid;
%         end
        if mid_diff < min_diff
            min_diff = mid_diff;
        end
    end
    diff = [diff,sqrt(min_diff)];
    %sec_diff = [sec_diff,sqrt(sec_min)];
end
plot(diff);
hold on;
% plot(sec_diff);
plot(mean(diff)*ones(1,length(garage_data(:,1))));
%vplot(mean(sec_diff)*ones(1,length(routing_data(:,1))));

% legend('boxoff');
% legend('first\_diff','second\_diff','first\_mean','second\_mean');
hold off;
