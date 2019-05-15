function count=writeraw(G, filename)
 %disp([' Write image data to'  filename ' ...']);
 fid=fopen(filename,'wb');
 if (fid==-1)
    error('can not open output image filem press CTRL-C to exit \n');
    pause
 end
 G=permute(G, [2,1]); 
 count=fwrite(fid,G, 'uchar');
 fclose(fid);
end