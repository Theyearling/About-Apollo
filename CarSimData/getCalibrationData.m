function output = getCalibrationData(name)
    output = [];
    fileID = fopen('calibration.txt', 'r');
    while ~feof(fileID)
        str = fgetl(fileID);
        if regexp(str, name)
            midID = fopen('mid.dat','w');
            fprintf(midID,'%s',str);
            fclose(midID);
            midID = fopen('mid.dat','r');
            mid_output = textscan(midID, '%*s %f');
            output = [output; mid_output{1}];
            fclose(midID);
        end
    end
    fclose(fileID);
end

