function out_value = map2values(in_value, in_min, in_max, out_min, out_max)
%MAP this function maps values from a set in min & min to a set out min & max

% in_min = (-1) * in_max;
in_range  = in_max  - in_min;

% out_min = 1;
% out_max = 0;
out_range = out_max - out_min;

out_value = (in_value - in_min)*(out_range)/(in_range);
end

