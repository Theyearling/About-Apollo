# matlab读取标定表数据         
                                                    
```                                                                         
function output = getCalibrationData(name)  %name表示要读取的数据，calibration文件包括speed、acceleration、command三种数据
    output = [];  
    fileID = fopen('路径\calibration_table.pb.txt', 'r');  
    while ~feof(fileID) %未到达文件末尾     
        str = fgetl(fileID); %逐行读取文件   
        if regexp(str, name) %是否匹配到name查询数据   
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

# matlab读取csv数据
```
localization_data = csvread("文件路径", 0, 0) % csv数据行列下标从0开始，后面两个参数限制开始取数据的行列
```                                                                         
# carsim计算侧偏刚度  

轮胎模型“魔术公式”:     
 
$$                                                                       
y = c\ \ast\ sin\ (b\ \ast\ atan\ (a\ \ast\ x\ -\ d\ \ast\ (a\ \ast\ x\ -\ atan\ (a\ \ast\ x))) )
$$                                                                          
                                                                            
## 处理数据、进行计算 

![vertical_force](/image/vertical_force.png)

![data_to_excel](/image/data_to_excel.png) 

![data_to_matlab](/image/data_to_matlab.png)  

![calculate](/image/calculate.png)  

![fit_options](/image/fit_options.png) 

                                                                            
                                                                            
# simulink的matlab function使用工作区变量   

![matlab_fuction](/image/matlab_function.png) 

                                                                            
输入端口的数据可以来自Simulink模型，Input <br>                                            
也可以来自Matlab工作区，Parameter     
                                                                     
### 设置Parameter，两种方法      

1. 手动添加函数参数变量，右击变量，选中Data Scope for "变量" -> Parameter                       
2.                                                               
![add_data](/image/add_data.png)       
