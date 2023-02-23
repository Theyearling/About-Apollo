ref_line = csvread('C:\Users\yilin.li\Desktop\坐标数据\20230210\循迹_old\garage.csv', 1, 0);
mid_x = ref_line(1,1);
mid_y = ref_line(1,2);
ref_line(:,1) = ref_line(:,1) - mid_x;
ref_line(:,2) = ref_line(:,2) - mid_y;

% plot(ref_line(:,1), ref_line(:,2));

% 查看是否有回跳点
% for i = 2:length(ref_line)-1
%     if (ref_line(i,1) - ref_line(i-1,1)) * (ref_line(i+1,1) - ref_line(i,1)) < 0
%         i
%     end
% end