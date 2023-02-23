tiledlayout(3,1);

nexttile([2 1]);
plot(ScopeMatchedX(:,2), ScopeMatchedY(:,2),'k');
hold on;
plot(ScopeRealX(:,2), ScopeRealY(:,2),'m');
legend('boxoff');
legend('matched\_Point', 'real\_Point');
hold off;

nexttile;
diff = [];
for i=1:length(ScopeRealX(:,2))
    min_diff=100;
    for j=1:length(ScopeMatchedX(:,2))
        mid_diff = (ScopeRealX(i,2)-ScopeMatchedX(j,2))^2 + (ScopeRealY(i,2)-ScopeMatchedY(j,2))^2;
        if mid_diff < min_diff
            min_diff = mid_diff;
        end
    end
    diff = [diff,sqrt(min_diff)];
end
plot(diff);
hold on;
plot(mean(diff)*ones(1,length(ScopeRealX(:,2))));
legend('boxoff');
legend('error', 'mean\_error');
hold off;