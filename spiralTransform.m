function [y] = spiralTransform(matrix,firstDirection)
% firstDirection: 0 - UP, 1 - RIGHT, 2 - DOWN, 3 - LEFT
% default - UP

if nargin<2
    direction = 0;  %Up Right Down Left
else
    direction = firstDirection;
end
y = [];

while ~isempty(matrix)
    switch direction
        case 0  %UP
            y = [y; matrix(end:-1:1,1)];
            matrix(:,1) = [];
        case 1  %RIGHT
            y = [y; matrix(1,1:end)'];
            matrix(1,:) = [];
        case 2  %DOWN
            y = [y; matrix(1:end,end)];
            matrix(:,end) = [];
        case 3  %LEFT
            y = [y; matrix(end,end:-1:1)'];
            matrix(end,:) = [];
    end
    direction = mod(direction+1,4);  
end

y = fliplr(y');

end

