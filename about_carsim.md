# matlab读取标定表数据
```
function output = getCalibrationData(name)  %name表示要读取的数据，calibration文件包括speed、acceleration、command三种数据
    output = [];
    fileID = fopen('路径\calibration_table.pb.txt', 'r');
    while ~feof(fileID) %未到达文件末尾
        str = fgetl(fileID); %逐行读取文件
        if regexp(str, name) %是否匹配到name查询数据
            midID = fopen('mid.dat', 'w+'); %将该行数据存到mid.dat临时文件中
            fprintf(midID, '%s', str);
            fclose(midID);
            midID = fopen('mid.dat', 'r');
            mid_output = textscan(midID, '%*s %f'); %过滤掉字符，读取浮点数数据，得到的是cell数据
            output = [output; mid_output{1}]; %拼接成double类型的矩阵
        end
    end
    fclose(fileID);
end
```

# carsim计算侧偏刚度
轮胎模型“魔术公式”: 
$$ y = c\ \ast\ sin\ (b\ \ast\ atan\ (a\ \ast\ x\ -\ d\ \ast\ (a\ \ast\ x\ -\ atan\ (a\ \ast\ x))) ) $$
![vertical_force](/image/vertical_force.png)
![data_to_excel](/image/data_to_excel.png)
![data_to_matlab](/image/data_to_matlab.png)
![calculate](/image/calculate.png)
![fit_options](/image/fit_options.png)