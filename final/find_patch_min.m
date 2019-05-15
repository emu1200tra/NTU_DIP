
function [rain_patch_index] = find_patch_min(template, rain_patch_set, overlap_size, scale)


    % Find NaNs and clear them
    nan_mask = isnan(template);
    template(nan_mask) = 0;

    % Make mask and weigh it
    mask = ones(size(template));
    mask(nan_mask) = 0;

    %scale = 0.5; % e.g., 1

    rain_patch_num = size(rain_patch_set,2);

    [dim num] = size(rain_patch_set);

    for i=1:num

        rain_patch = reshape(rain_patch_set(:,i),overlap_size,overlap_size);
        diff = abs(rain_patch.*mask - template).^2;
        error(i) = mean2(diff);

    end


    [min_v,rain_patch_index] = min(error);
    
    candidate = find(error < min_v+ scale*min_v);

    if ~isempty(candidate)
        index = randi( size(candidate,2) );
        rain_patch_index = candidate(index);
    end

end

