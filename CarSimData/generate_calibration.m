% v2=[v,vbr];
% a2=[a,abr];
% br2=[tr,br];
v2 = getCalibrationData('speed');
a2 = getCalibrationData('acceleration');
br2 = getCalibrationData('command')/100;

F=scatteredInterpolant(v2,a2,br2);%转成列向量
vubr=0:0.1:10;
aubr=-9.37:0.01:3.28;
tablebr=zeros(length(vubr),length(aubr));
for i=1:length(vubr)
    for j=1:length(aubr)
        tablebr(i,j)=F(vubr(i),aubr(j));
    end
end