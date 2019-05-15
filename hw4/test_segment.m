function test_seg = test_segment(input_image , test_seg, flag , size1 , size2)
    [size1 , size2] = size(input_image);
    if(~isempty(find(input_image(1,:) == 0 , 1)) && ~isempty(find(input_image(end , :) == 0 , 1))>0 && ~isempty(find(input_image(:,1) == 0 , 1)) > 0 && ~isempty(find(input_image(: , end) == 0 , 1)) > 0)
        test_seg{end+1} = input_image;
    else
        if(flag)
            tmp = 1;
            while(1)
                while(tmp <= size2 && isempty(find(input_image(: , tmp) == 0 , 1)))
                    tmp = tmp + 1;
                end
                left = tmp;
                while(tmp <= size2 && ~isempty(find(input_image(: , tmp) == 0 , 1)))
                    tmp = tmp + 1;
                end
                right = tmp - 1;
                if(left > right)
                    break;
                end
                test_seg = test_segment(input_image(: , left:right) , test_seg , 0 , size1 , size2);
            end
        else
            tmp = 1;
            while(1)
                while(tmp <= size1 && isempty(find(input_image(tmp , :) == 0 , 1)))
                    tmp = tmp + 1;
                end
                top = tmp;
                while(tmp <= size1 && ~isempty(find(input_image(tmp , :) == 0 , 1)))
                    tmp = tmp + 1;
                end
                down = tmp - 1;
                if(top > down)
                    break;
                end
                test_seg = test_segment(input_image(top:down , :) , test_seg , 1 , size1 , size2);
            end        
        end
    end
end