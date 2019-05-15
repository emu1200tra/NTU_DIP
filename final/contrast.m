in = [0.0  0.0  0.0; 1.0 1.0 1.0];
out = [0.2 0.2 0.2; 0.8 0.8 0.8];

for iteration = 1:15
    infilename = ['./test/' , int2str(iteration) , '.png'];
    original_image = imread(infilename);
    output = imadjust(original_image,in,out);
    imwrite( uint8(output), ['./test/contrast' , int2str(iteration) ,'.jpg']);
end
