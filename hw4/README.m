% DIP Homework Assignment #4
% 05 30, 2018
% Name: ¬x¯Eµ¾
% ID#: B04902028
% email: b04902028@ntu.edu.tw
%###############################################################################% 
% Problem 1: Optical Character Recognition (OCR)
% Implementation: binarize, median filter, segmentation, intapolation, similarity  
% .m file: binarize.m train_segment.m test_segment.m predict_char.m
%          median_filter.m resize_image.m
%###############################################################################% 

function README()
    G = readfile('../raw/TrainingSet.raw' , 450 , 248);
    S1 = readfile('../raw/sample1.raw' , 390 , 125);
    S2 = readfile('../raw/sample2.raw' , 390 , 125);
    S1 = median_filter(S1 , 390 , 125 , 3);
    S1 = binarize(S1 , 390 , 125);
    S2 = median_filter(S2 , 390 , 125 , 3);
    S2 = binarize(S2 , 390 , 125);
    correct_char = train_segment(G);
    prdict_char(S1 , correct_char , 390 , 125 , 1);
    prdict_char(S2 , correct_char , 390 , 125 , 2);
end