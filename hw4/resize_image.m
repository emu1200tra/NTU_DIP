function outputImage = resize_image(inputImage , cmp_size)
    oldSize = size(inputImage);                   
    scale = [cmp_size(1)/oldSize(1) cmp_size(2)/oldSize(2)];
    newSize = cmp_size;  
    rowIndex = min(round(((1:newSize(1))-0.5)./scale(1)+0.5),oldSize(1));
    colIndex = min(round(((1:newSize(2))-0.5)./scale(2)+0.5),oldSize(2));
    outputImage = inputImage(rowIndex,colIndex,:);
end