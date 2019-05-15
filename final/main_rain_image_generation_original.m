
% -------------------------------------------------------------------------------------
% This program MUST be used for only research purpose.
% This code is just an example. You can upgrade this code.
% This program is released by Dr. Chang-Hwan Son
% In this code, boundary cut is implemented with open source code provided in [1] 
% If you use this code, you should cite our working paper: C.H. Son and X.-P. Zhang, "Rain structure transfer using an exemplar rain image for
% synthetic rain image generation", DOI: 10.13140/RG.2.2.36556.00641, Sept. 2016.
% --------------------------------------------------------------------------------------

clc
close all
clear all

% ----------------------------------------------------------------
% Input file names for running our algorithm
% ----------------------------------------------------------------

% Add your path
addpath(genpath('C:\Users\root\Pictures\rain removal')); 

% Target 'non-rain' image file
infilename = 'nonrain_1.tif'; 

% Exemplar 'rain' image file
filename_mask = './test/10rmask.jpg';
filename_image = './test/10r.jpg';

% ----------------------------------------------------------------
% Parameter setting
% ----------------------------------------------------------------

patch_size = 40; % patch size
border = round(patch_size/3); % or patch_size/3
overlap_size = patch_size + border; % patch size including border size
gain_factor = 1; % control the magnitude of rain structures in rain patch
threshold = 0.5;

% -----------------------------------------------------------
% Read the non-rain image
% -----------------------------------------------------------

display('1. Read input image');

original_image = imread(infilename);
%original_image = imresize(original_image , [321 , 481]);
imwrite( uint8(original_image), 'resize_image.jpg');
if size(original_image, 3) > 1
    %input_image = rgb2gray(original_image);
    [input_image , U , V] = rgb2yuv(original_image);
    input_image = double(input_image);   
else
     input_image = double(original_image);
end

%input_image = input_image./255;

imwrite( uint8(input_image), 'Y_image.tif');

[img_height img_width] = size(input_image);

% ------------------------------------------------------------
% Read the rain image and its masked image
% ------------------------------------------------------------

patch_num_for_image = 100;
mask_ratio = ceil(overlap_size*overlap_size*0.5);

mask = imread(filename_mask);
rain_image = imread(filename_image);

[mask_height mask_width mask_ch] = size(mask);

if mask_ch > 1
   [mask , Um , Vm] = rgb2yuv(mask);
   [rain_image , Ur , Vr] = rgb2yuv(rain_image);
   
   mask_image = double(mask);
   rain_image = double(rain_image);
   
else
   mask_image = double(mask);
   rain_image = double(rain_image);
end

 %mask_image = mask_image/255;
 %rain_image = rain_image/255;

[row_idx col_idx] = find(mask_image == 0);

rain_pixel_num = size(row_idx, 1);

if rain_pixel_num > patch_num_for_image
    sel_idx = randperm(rain_pixel_num,patch_num_for_image);
    rows = row_idx(sel_idx);
    cols = col_idx(sel_idx);
else
    rows = row_idx;
    cols = col_idx;
end

size(row_idx)
%{
figure;
subplot(1,2,1);
imshow(mask_image,[0 255]);
title('Masked rain image');
subplot(1,2,2);
imshow(rain_image,[0 255]);
title('Rain image');
%}
% ---------------------------------------------------------
% Rain patch extraction
% ---------------------------------------------------------

display('2. Rain patch extraction');

rain_patch_set = [];

 for j = 1:size(rows,1)

    patch_row = rows(j);
    patch_col = cols(j);

    if (patch_row < mask_height - overlap_size) && (patch_col < mask_width - overlap_size)

        mask_patch = mask_image(patch_row:patch_row+overlap_size-1,patch_col:patch_col+overlap_size-1);
        rain_patch = rain_image(patch_row:patch_row+overlap_size-1,patch_col:patch_col+overlap_size-1);
       
        patch_Ur = Ur(patch_row:patch_row+overlap_size-1,patch_col:patch_col+overlap_size-1);
        patch_Vr = Vr(patch_row:patch_row+overlap_size-1,patch_col:patch_col+overlap_size-1);
        RGB_patch = yuv2rgb(rain_patch , patch_Ur , patch_Vr);
        [mask_row_idx mask_col_idx] = find(mask_patch == 0);

        mask_pixel_num = size(mask_row_idx, 1);

        if mask_pixel_num >= mask_ratio       
            rain_patch_set = [rain_patch_set rain_patch(:)];
            
            imwrite(uint8(RGB_patch) , ['./patch3/' , int2str(j) , '.jpg']);
        end

    end

end    

% --------------------------------------------------------
% Rain patch transfer and stitching
% --------------------------------------------------------

rain_patch_num = ceil(img_height/overlap_size*img_width/overlap_size*0.2);
synthetic_rain_image = zeros([img_height img_width]);
synthetic_patch_stitching = zeros([img_height img_width]);

zoomx = round(img_height/overlap_size);
zoomy = round(img_width/overlap_size);

for j = 1:overlap_size:zoomx*overlap_size
    for i=1:overlap_size:zoomy*overlap_size
  
        input_patch = input_image(j:j+overlap_size-1,i:i+overlap_size-1);
        input_patch_mean = mean(input_patch(:));
        input_mean_vec = repmat(input_patch_mean,overlap_size*overlap_size,1);
        input_mean_subtract = input_patch(:) - input_mean_vec;

        rain_patch_idx = randi( [1, size(rain_patch_set,2)] );
        rain_patch_vec = rain_patch_set(:,rain_patch_idx);
        rain_patch = reshape(rain_patch_vec, overlap_size, overlap_size);
        rain_mean_subtract = rain_patch_vec - repmat(mean(rain_patch_vec(:)),overlap_size*overlap_size,1);

        %synthetic_patch = input_mean_subtract;
        %synthetic_patch(rain_edge_idx) = rain_mean_subtract(rain_edge_idx);

        synthetic_patch = input_patch(:) + rain_mean_subtract.*gain_factor;
        
        synthetic_patch_stitching(j:j+overlap_size-1,i:i+overlap_size-1) = reshape(synthetic_patch,overlap_size, overlap_size);
     
    end 
end
%{
figure;
subplot(1,2,1);
imshow(synthetic_patch_stitching,[0 255]);
title('Stitching rain image');
subplot(1,2,2);
imshow(input_image,[0 255]);
title('Rain image');
%}
imwrite( uint8(synthetic_patch_stitching),'rain_patch_stitching.tif');

% ---------------------------------------------------------------
% Rain image generation via rain struture transfer
% ---------------------------------------------------------------

filt_w = 4;
smooth_filt = binomialFilter(filt_w)*binomialFilter(filt_w)';
cur_dest_im_map = repmat(NaN, [img_height img_width]);
cur_residual_rain_map = repmat(NaN, [img_height img_width]);
cur_error = cur_dest_im_map;
work_im = zeros([img_height img_width]);

gain_map = rand(img_height,img_width);
%gain_map = ones(img_height,img_width);

zoomx = round((img_height - border)/patch_size);
zoomy = round((img_width - border)/patch_size);

%for i = 1:patch_size:img_height-overlap_size
%    for j=1:patch_size:img_width-overlap_size

for i=[1:patch_size:img_height-patch_size-border, img_height-patch_size-border+1]
    for j=[1:patch_size:img_width-patch_size-border, img_width-patch_size-border+1]
  
        % rain patch stitcing
        input_patch = input_image(i:i+overlap_size-1,j:j+overlap_size-1);
        input_patch_mean = mean(input_patch(:));
        input_mean_vec = repmat(input_patch_mean,overlap_size*overlap_size,1);
        input_mean_subtract = input_patch(:) - input_mean_vec;

        template_rain = cur_residual_rain_map(i:i+overlap_size-1,j:j+overlap_size-1);
        
        if (i==1 & j==1)
            rain_patch_idx = randi( [1, size(rain_patch_set,2)] );
        else
            rain_patch_idx = find_patch_min(template_rain, rain_patch_set, overlap_size, threshold);
        end
        
        rain_patch_vec = rain_patch_set(:,rain_patch_idx);
        rain_patch = reshape(rain_patch_vec, overlap_size, overlap_size);
        rain_mean_subtract = rain_patch_vec - repmat(mean(rain_patch_vec(:)),overlap_size*overlap_size,1);
        
        gain_factor = mean2(gain_map(i:i+overlap_size-1,j:j+overlap_size-1));
        synthetic_patch = input_patch(:) + rain_mean_subtract.*gain_factor;
        
        % rain image generation based on rain patch blending, which finds minimum energy based on dynmaic programming
        template = cur_dest_im_map(i:i+overlap_size-1,j:j+overlap_size-1);
        sq = reshape(synthetic_patch,overlap_size,overlap_size);
        err_sq = (sq - template).^2;
        blend_mask = logical(zeros(size(err_sq)));
        
        if (i > 1 & j > 1),
            blend_mask = dpmain(err_sq,border);
        elseif (i == 1 & j == 1),
        elseif (i == 1),
            blend_mask(:,1:border) = dp(err_sq(:,1:border));
        else 
            blend_mask(1:border,:) = blend_mask(1:border,:) ...
            | dp(err_sq(1:border,:)')';
        end;
      
        cur_error(i:i+overlap_size-1,j:j+overlap_size-1) = err_sq.*~blend_mask;

        blend_mask = rconv2(double(blend_mask),smooth_filt); % Do blending   
        
        template(isnan(template)) = 0;
        
        work_im(i:i+overlap_size-1,j:j+overlap_size-1) ...
          = work_im(i:i+overlap_size-1,j:j+overlap_size-1).*blend_mask...
          + sq.*(1-blend_mask);
        
        cur_dest_im_map(i:i+overlap_size-1,j:j+overlap_size-1) ...
          = template.*blend_mask + sq.*(1-blend_mask);
      
        cur_residual_rain_map(i:i+overlap_size-1,j:j+overlap_size-1) ...
          = template.*blend_mask + rain_patch.*(1-blend_mask);
         
    end 
end

synthetic_rain_image = work_im;

% lightness normaliztion
max_L = max(synthetic_rain_image(:));
min_L = min(synthetic_rain_image(:));

synthetic_rain_image = 255.*(synthetic_rain_image - min_L)./(max_L - min_L);
final_image = yuv2rgb(synthetic_rain_image , U , V);
%{
figure;
subplot(1,3,1);
imshow(synthetic_rain_image,[0 255]);
title('Synthetic rain image');
subplot(1,3,3);
imshow(input_image,[]);
title('Rain image');
%}
imwrite( uint8(final_image),'rain_image_generation.tif');
test_final = uint8(final_image);
test_original = uint8(original_image);
rain_layer = test_final - test_original;
rain_layer2 = 30 - (test_original - test_final);
%{
rain_layer = uint8(final_image(:,:,1)) - uint8(original_image(:,:,1));
rain_layer(:,:,end+1) = uint8(final_image(:,:,2)) - uint8(original_image(:,:,2));
rain_layer(:,:,end+1) = uint8(final_image(:,:,3)) - uint8(original_image(:,:,3));
%}
imwrite( uint8(rain_layer + rain_layer2),'rain_layer.jpg');
%imwrite( uint8(rain_layer2),'rain_layer2.jpg');

% ---------------------------------------------------------------
% Rain image generation withi low visibility
% ---------------------------------------------------------------

transmission_map = 0.8.*ones(img_height,img_width);
airlight = 230; % [1-255]
temp = ones(img_height,img_width);

synthetic_rain_image = synthetic_rain_image.*transmission_map + airlight.*(temp-transmission_map);
final_image = yuv2rgb(synthetic_rain_image , U , V);
imwrite( uint8(final_image), 'rain_image_generation_light.tif');
%{
subplot(1,3,2);
imshow(synthetic_rain_image,[0 255]);
title('Synthetic rain image with low visibility');
%}