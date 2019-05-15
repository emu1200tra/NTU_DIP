function binary = binarize(image , size1 , size2)
    for i = 1:size2
        for j = 1:size1
            if (image(i , j) >= 128)
                binary(i , j) = 255;
            else
                binary(i , j) = 0;
            end
        end
    end
end