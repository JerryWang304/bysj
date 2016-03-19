size = 6;
data1=zeros(size,1);%c
data2=zeros(size,1);%ic

temp=zeros(size*2,1);

temp(1,1) = prctile(dlmread('C:\\d_2_c.txt',' '),50);
temp(2,1) = prctile(dlmread('C:\\d_2_ic.txt',' '),50);
temp(3,1) = prctile(dlmread('C:\\d_1_c.txt',' '),50);
temp(4,1) = prctile(dlmread('C:\\d_1_ic.txt',' '),50);
temp(5,1) = prctile(dlmread('C:\\d_20_c.txt',' '),50);
temp(6,1) = prctile(dlmread('C:\\d_20_ic.txt',' '),50);
temp(7,1) = prctile(dlmread('C:\\d_4_c.txt',' '),50);
temp(8,1) = prctile(dlmread('C:\\d_4_ic.txt',' '),50);
temp(9,1) = prctile(dlmread('C:\\d_6_c.txt',' '),50);
temp(10,1) = prctile(dlmread('C:\\d_6_ic.txt',' '),50);
temp(11,1) = prctile(dlmread('C:\\d_8_c.txt',' '),50);
temp(12,1) = prctile(dlmread('C:\\d_8_ic.txt',' '),50);

for i=1:12
    if mod(i,2) == 0
        data2(i/2,1) = temp(i,1);
    else
        data1((i+1)/2,1) = temp(i,1);
    end
    
end

x = 0:1:(size-1);

lw = 1;
ms = 8;
fs = 16;

%XTick = {'\sigma=0.2, complete','\sigma=0.2, incomplete','\sigma=0.4, complete','\sigma=0.4, incomplete','\sigma=0.6, complete','\sigma=0.6, incomplete','\sigma=0.8, complete','\sigma=0.8, incomplete','\sigma=1.0, complete','\sigma=1.0, incomplete'};
%XTickk = ['\sigma=0.2, complete','\sigma=0.2, incomplete','\sigma=0.4, complete';'\sigma=0.4, incomplete';'\sigma=0.6, complete';'\sigma=0.6, incomplete';'\sigma=0.8, complete';'\sigma=0.8, incomplete';'\sigma=1.0, complete';'\sigma=1.0, incomplete'];
%XTick = {'0.2','0.2,incomplete','0.4,complete','0.4,incomplete','0.6,complete','0.6,incomplete','0.8,complete','0.8,incomplete','1.0,complete','1.0,incomplete'};
XTick = {0.2, 1.0, 2.0, 4.0, 6.0, 8.0, 10.0};
legendkey = {'complete','incomplete'};

figure;
plot(x, data1, '-ro', 'LineWidth', lw, 'MarkerSize', ms, 'MarkerFaceColor', 'w');
hold on;
plot(x, data2, '-ro', 'LineWidth', lw, 'MarkerSize', ms, 'MarkerFaceColor', 'w', 'LineStyle','-.', 'color', 'b');
xlabel('\sigma','FontSize', fs, 'FontName', 'Arial');
ylabel('50 Percentile','FontSize', fs, 'FontName', 'Arial');
axis([0 5 0 60]);
legend(legendkey,'Location', 'NorthWest');
% title('D=1, C=100, U=1, load=2.0', 'FontSize', fs, 'FontName', 'Helvetica');
% axis([0 1 0 600]);
set(gca, 'FontSize', fs, 'FontName', 'Arial','YGrid','on');
% set(gca, 'xscale', 'log');
set(gca,'XTick',0:1:(size-1));
set(gca,'XTickLabel',XTick);
set(gcf,'position',[100 100 640 320]);
set(gcf,'PaperPositionMode','auto');

% print('-r0','-depsc','result_temporal_final_50.eps');
print('-r0','-depsc','d_2_mean.eps');
