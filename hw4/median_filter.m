function output_image = median_filter(input_image , size2 , size1 , box_size)
    for i = 1:size1
        for j = 1:size2
            record = zeros(box_size*box_size);
            counter = 0;
            for k = -floor(box_size/2):box_size/2
                for l = -floor(box_size/2):box_size/2
                    
                    if(i+k <=0 || i+k > size1 || j+l <= 0 || j+l > size2)
                        continue;
                    else
                        counter = counter + 1;
                        record(counter) = input_image(i+k , j+l);
                    end
                end
            end
            record = sort(record(1:counter));
            place = floor(length(record)/2);
            if(mod(length(record) , 2) == 0)
                output_image(i , j) = (record(place-1) + record(place))/2;
            else
                output_image(i , j) = record(place);
            end
        end
    end
end
