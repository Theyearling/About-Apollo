%% 读取数据
data=csvread('routing.csv',1,0);%csvread只能读取纯数据

%% 数据归一化
data1=data;
data1(:,1)=data(:,1)-data(1,1);
data1(:,2)=data(:,2)-data(1,2);
% plot(data1(:,1),data1(:,2));
% hold on;

%% 重采样，平滑参考线
x=data1(:,1);
y=data1(:,2);
theta=data1(:,3);
len=length(data1(:,1));
s(1,1)=0;
for i=2:len
   dis=sqrt((x(i)-x(i-1)) * (x(i)-x(i-1))+(y(i)-y(i-1))*(y(i)-y(i-1))); 
   s(i,1)=dis+s(i-1);
end

ss=0:0.01:s(end);
xx=fitx(ss);
yy=fity(ss);
theta2=fittheta(ss);
% figure( 'Name', '拟合结果对比' );
% scatter(x,y);
% hold on
% plot(xx,yy);
data2(:,1)=xx;
data2(:,2)=yy;
data2(:,3)=theta2;
% data2=data1;

%% 拟合二维的5次多项式，效果差弃用
%  p1 = polyfit(s,x,5);
%  p2 = polyfit(s,y,5);
%  
% ss=0:0.1:s(end);
% xx=polyval(p1, s);
% yy=polyval(p2, s);
% figure( 'Name', '拟合结果对比' );
% scatter(x,y);
% hold on
% plot(xx,yy);
 
%% 数据长度判断
%拟合前后向搜索个数
kFindNum=5;

if(len<kFindNum*2+1)
    return;
end

%% 曲率计算
kappa_arr = [];
posi_arr = [];
norm_arr = [];

len2=length(data2(:,1));
kFindNum=5;
for i=1+kFindNum:len2-kFindNum
    x1(1,1) = data2(i-kFindNum,1);
    x1(1,2) = data2(i,1);
    x1(1,3) = data2(i+kFindNum,1);   
    y1(1,1) = data2(i-kFindNum,2);
    y1(1,2) = data2(i,2);
    y1(1,3) = data2(i+kFindNum,2);
    [kappa,norm_l] = PJcurvature(x1,y1);
    data2(i,4)=kappa;
end

% plot(data2(:,4));
% axis ([0 len2 -1 1]);
% hold on;

%% 曲率滤波处理
%中值滤波
data3=data2;
t_medfilter(:,1) = medfilt1(data2(:,4),30);
% plot(t_medfilter);
data3(:,4)=t_medfilter;

%% x,y,theta,v,a,kappa
target_speed=5;
target_a=1;
target_speed_up_time=target_speed/target_a;

s_speed_up=0.5*target_a*target_speed_up_time^2;
target_speed_average_time=(s(end)-2*s_speed_up)/target_speed;
target_time=target_speed_average_time+target_speed_up_time*2;

t_step=0.01;
% for t_current_time=0:0.01:target_time
%     if(t_current_time<=target_speed_up_time)
%        %加速路段
%        s_current=0.5*target_a* t_current_time^2;
%        index=round(s_current*100);
%        if(index<=0)
%            index=1;
%        end
%        xo=data3(index,1);
%        yo=data3(index,2);
%        thetao=data3(index,3);
%        kappao=data3(index,4);       
%        vo=target_a*t_current_time;
%        ao=target_a;
%     elseif(t_current_time<=target_speed_up_time+target_speed_average_time)
%        %匀速路段
%        s_current=s_speed_up+target_speed*(t_current_time-target_speed_up_time);
%        index=round(s_current*100);
%        xo=data3(index,1);
%        yo=data3(index,2);
%        thetao=data3(index,3);
%        kappao=data3(index,4);       
%        vo=target_speed;
%        ao=0;            
%     else
%        %减速路段
%        time_use=t_current_time-target_speed_up_time-target_speed_average_time;
%        s_current=s(end)-s_speed_up+(target_speed^2-(target_speed-target_a*time_use)^2)/(2*target_a);
%        index=round(s_current*100);
%        xo=data3(index,1);
%        yo=data3(index,2);
%        thetao=data3(index,3);
%        kappao=data3(index,4);       
%        vo=target_speed-target_a*time_use;
%        ao=-target_a;        
%     end
% end

i=0;
for t_current_time=0:0.01:target_time
    i=i+1;
    if(t_current_time<=target_speed_up_time)
       %加速路段
       s_current=0.5*target_a* t_current_time^2;
       index=round(s_current*100);
       if(index<=0)
           index=1;
       end
       data4(i,1)=data3(index,1);
       data4(i,2)=data3(index,2);
       data4(i,3)=data3(index,3);
       data4(i,4)=data3(index,4);       
       data4(i,5)=target_a*t_current_time;
       data4(i,6)=target_a;
    elseif(t_current_time<=target_speed_up_time+target_speed_average_time)
       %匀速路段
       s_current=s_speed_up+target_speed*(t_current_time-target_speed_up_time);
       index=round(s_current*100);
       data4(i,1)=data3(index,1);
       data4(i,2)=data3(index,2);
       data4(i,3)=data3(index,3);
       data4(i,4)=data3(index,4);       
       data4(i,5)=target_speed;
       data4(i,6)=0;            
    else
       %减速路段
       time_use=t_current_time-target_speed_up_time-target_speed_average_time;
       s_current=s(end)-s_speed_up+(target_speed^2-(target_speed-target_a*time_use)^2)/(2*target_a);
       index=round(s_current*100);
       data4(i,1)=data3(index,1);
       data4(i,2)=data3(index,2);
       data4(i,3)=data3(index,3);
       data4(i,4)=data3(index,4);       
       data4(i,5)=target_speed-target_a*time_use;
       data4(i,6)=-target_a;        
    end
end

plot(data4(:,1),data4(:,2));
% plot(data4(:,3));
% plot(data4(:,4));
% data4(:,4)
% axis ([0 length(data4(:,4)) -1 1]);
% plot(data4(:,5));
% plot(data4(:,6));

