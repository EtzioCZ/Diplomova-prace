close all;
clear variables; 


%p1 = linspace(1.1,2,10);
%p2 = linspace(2,5,5);

% sklon = linspace(1,5,15);

% p1 = 1.1 : 0.025 :1.3;
% p2 = 1.3 :0.1:2;
% p3 = 2.5 : 0.5: 4;
% p4 = 5 : 1 : 8;
% p5 = 8 : 3 : 14;
p1 = 1.3 :0.05 : 2;    p2 = 2.25 : 0.25 : 3; p3 = 3.5:0.5:4;    p4 = 5 : 1 : 14;
sklon = [p1 p2 p3 p4];

y = linspace (0,29,30);

for i = 1: length(sklon)
    
x = sklon(i).^(-y) + 0.003; 

plot(y+1,x);
grid on;
xlim([1 30]);
yticks(fliplr([1 0.5 0.4 0.3 0.2 0.09 0.07 0.05 0.03 0.01]));
%xticks(linspace(0,30,31));
ylim([0,1])
xlabel('Poèet harmonických \rightarrow [-]');
ylabel('Amplituda \rightarrow [-]')
hold on;

end


% delkaNot = [2^-2 2^-1 (2^-1)*1.5 2^0 (2^0)*1.5 2^1 (2^1)*1.5 2^2]; 
