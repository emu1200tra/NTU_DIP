function correct_char = train_segment(train_set)
    train_set = train_set(11:240 , 8:441);
    [m , n] = size(train_set);
    blockx = floor(m/5);
    blocky = floor(n/14);
    train_char = zeros(46 , 31);
    for i = 1:70
        train_char(: , : , i) = zeros(46 , 31);
    end
    index = 1;
    for i = 1:5
        for j = 1:14
            train_char(: , : , index) = train_set(((i-1)*blockx+1):i*blockx , ((j-1)*blocky+1):j*blocky);
            %imwrite(uint8(train_char(: , : , index)), ['./fuck_binary', num2str(index) , '.png']);
            count=writefile(train_char(: , : , index), ['binary', num2str(index) , '.raw']);
            index = index + 1;
        end
    end
    correct_char = {};
    for i = 1:70
        record = find(train_char(: , : , i)' == 0);
        up = ceil(min(record) / blocky);
        down = ceil(max(record) / blocky);
        record2 = find(train_char(: , : , i) == 0);
        left = ceil(min(record2) / blockx);
        right = ceil(max(record2) / blockx);
        correct_char{end+1} = train_char(up:down , left:right , i);
        %imwrite(uint8(train_char(up:down , left:right , i)), ['./fuck_binary', num2str(i) , '_fix.png']);
        count=writefile(train_char(up:down , left:right , i), ['binary', num2str(i) , 'fix.raw']);
    end
end