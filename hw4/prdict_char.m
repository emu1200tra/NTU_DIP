function prdict_char(input_image , correct_char , size1 , size2 , flag)
    %fprintf('check\n');
    table = {'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' , 'I' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' , 'Q' , 'R' , 'S' , 'T' , 'U' , 'V' , 'W' , 'X' , 'Y' , 'Z' , 'a' , 'b' , 'c' , 'd' , 'e' , 'f' , 'g' , 'h' , 'i' , 'j' , 'k' , 'l' , 'm' , 'n' , 'o' , 'p' , 'q' , 'r' , 's' , 't' , 'u' , 'v' , 'w' , 'x' , 'y' , 'z' , '0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '!' , '@' , '#' , '$' , '%' , '^' , '&' , '*'};
    test_seg = test_segment(input_image ,{}, 0 , size1 , size2);
    num = length(test_seg);
    %fprintf('seg:%d\n' , num);
    for i = 1:num
        %imwrite(uint8(test_seg{i}), ['fuckyou_char_in_test_', num2str(i), '.png']);
        count=writefile(test_seg{i}, ['char_in_test_', num2str(i), '.raw']);
    end
    for i = 1:num
        overlap = 0;
        for j = 1:70
            %cmpsize1 = size(correct_char{j});
            cmpsize1 = size(test_seg{i});
            %scaling = imresize(test_seg{i} , cmpsize1);
            %scaling = imresize(correct_char{j} , cmpsize1);
            scaling = resize_image(correct_char{j} , cmpsize1);
            scaling_size = size(scaling);
            for k = 1:scaling_size(1)
                for l = 1: scaling_size(2)
                    if(scaling(k , l) >= 128)
                        scaling(k , l) = 255;
                    else
                        scaling(k , l) = 0;
                    end
                end
            end
            %imwrite(uint8(scaling), ['fuckyou_char_scaling_', num2str(i) , num2str(j), '.png']);
            count=writefile(scaling, ['scaling_', num2str(i) , num2str(j),'.raw']);
            cmpoverlap = 0;
            for k = 1:cmpsize1(1)
                for l = 1:cmpsize1(2)
                    test = test_seg{i};
                    if(test(k , l) == scaling(k , l))
                        cmpoverlap = cmpoverlap + 1;
                    end
                end
            end
            %cmpoverlap = cmpoverlap / (cmpsize1(1)*cmpsize1(2));
            %fprintf('check:%d\n' , cmpoverlap);
            if(cmpoverlap > overlap)
                overlap = cmpoverlap;
                index = j;
            end
        end
        %fprintf('WTF?\n');
        if(flag == 1 && i ~= 2)
            fprintf('S1 predict:%d:%s:%d , overlap:%d\n' , i , char(table(index)) , index , overlap);
        end
        if(flag == 2 && i ~= 1)
            fprintf('S2 predict:%d:%s:%d , overlap:%d\n' , i , char(table(index)) , index , overlap);
        end
    end
end