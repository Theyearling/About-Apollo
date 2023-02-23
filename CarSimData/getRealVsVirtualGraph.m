tiledlayout(3,1)

nexttile([2 1]);
plot(real_vehicle_data(:,1),real_vehicle_data(:,2),'k');
hold on;
plot(ScopeRealX(:,2),ScopeRealY(:,2),'b');
plot(routing_data(:,1), routing_data(:,2),'r');
legend('boxoff');
legend('real','virtual','routing');
hold off;

nexttile;
diff = [];
for i=1:length(ScopeRealX(:,2))
    min_diff = 100;
    for j=1:length(real_vehicle_data(:,1))
        mid_diff = (ScopeRealX(i,2)-real_vehicle_data(j,1))^2 + (ScopeRealY(i,2)-real_vehicle_data(j,2))^2;
        if mid_diff < min_diff
            min_diff = mid_diff;
        end
    end
    diff = [diff,sqrt(min_diff)];
end
plot(diff);
hold on;
plot(mean(diff)*ones(1,length(ScopeRealX(:,2))));
hold off;