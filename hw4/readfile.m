function G = readfile(filename , size1 , size2)
    disp(['	Retrieving Image ' filename ' ...']);
	fid=fopen(filename,'rb');
	if (fid==-1)
	  	error('can not open imput image filem press CTRL-C to exit \n');
	  	pause
	end
	pixel=fread(fid,inf, 'uchar');
	fclose(fid);
    [Y, X]=size(pixel);
    Size=(Y*X);
    G=zeros(size1 , size2);
    G(1:Size)=pixel(1:Size);
    G=permute(G, [2,1]);
end