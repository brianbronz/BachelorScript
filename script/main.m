function main = readCoefficients(filename)
    %open the file
    fileID = fopen(filename,'r');
    if fileID == -1
        error('File is not found');
    end
    %read the file row by row
    newFileID = fopen('rootsMatlab.txt', 'w');
    while (~feof(fileID))
        %start the timer
        tic;
        line = fgetl(fileID);
        %split the line into a cell array
        line = strsplit(line, ' ');
        %find the zero polynomial using the roots
        line = str2double(line);
        %remove the last element of line if it is Nan
        if isnan(line(length(line)))
            line = line(1:length(line)-1);
        end
        %find the intervals and the roots of the polynomial
        r = roots(line);
        %write the roots to the file
        %get the interval of the polynomial
        fprintf(newFileID, 'The coefficients are: ');
        fprintf(newFileID, '%f ', line);
        fprintf(newFileID, '\n');
        for i = 1:length(r)
            fprintf(newFileID, 'The root is: %f ', r(i));
            fprintf(newFileID, '\n');
        end
        fprintf(newFileID, '\n');
        %stop the timer
        toc;
        %write the time to the file
        fprintf(newFileID, 'Time taken by program is : %f sec\n', toc);
        fprintf(newFileID, '\n');
    end
end
