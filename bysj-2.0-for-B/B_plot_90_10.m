d1 = dlmread('C:\\b_T0.txt',' ');
d2 = dlmread('C:\\b_T1.txt',' ');
d3 = dlmread('C:\\b_T2.txt',' ');
d4 = dlmread('C:\\b_T3.txt',' ');
d5 = dlmread('C:\\b_T4.txt',' ');
d6 = dlmread('C:\\b_T5.txt',' ');
d7 = dlmread('C:\\b_T6.txt',' ');
d8 = dlmread('C:\\b_T7.txt',' ');
d9 = dlmread('C:\\b_T8.txt',' ');
d10 = dlmread('C:\\b_T9.txt',' ');
d11 = dlmread('C:\\b_T10.txt',' ');
size = 11;
data_90 = zeros(1,size);
data_90(1,1) = prctile(d1,90);
data_90(1,2) = prctile(d2,90);
data_90(1,3) = prctile(d3,90);
data_90(1,4) = prctile(d4,90);
data_90(1,5) = prctile(d5,90);
data_90(1,6) = prctile(d6,90);
data_90(1,7) = prctile(d7,90);
data_90(1,8) = prctile(d8,90);
data_90(1,9) = prctile(d9,90);
data_90(1,10) = prctile(d10,90);
data_90(1,11) = prctile(d11,90);

data_10 = zeros(1,size);
data_10(1,1) = prctile(d1,10);
data_10(1,2) = prctile(d2,10);
data_10(1,3) = prctile(d3,10);
data_10(1,4) = prctile(d4,10);
data_10(1,5) = prctile(d5,10);
data_10(1,6) = prctile(d6,10);
data_10(1,7) = prctile(d7,10);
data_10(1,8) = prctile(d8,10);
data_10(1,9) = prctile(d9,10);
data_10(1,10) = prctile(d10,10);
data_10(1,11) = prctile(d11,10);

data = zeros(1,size);
data = data_90 - data_10;
x = 0:1:10;

lw = 1;
ms = 8;
fs = 16;

figure;
plot(x, data, '-ro', 'LineWidth', lw, 'MarkerSize', ms, 'MarkerFaceColor', 'w');
xlabel('Optimization Interval (0 means per flow)','FontSize', fs, 'FontName', 'Arial');
ylabel('90 Percentile - 10 Pencentile','FontSize', fs, 'FontName', 'Arial');
axis([0 10 0 4]);
% legend(legendkey,'Location', 'NorthEast');
% title('D=1, C=100, U=1, load=2.0', 'FontSize', fs, 'FontName', 'Helvetica');
% axis([0 1 0 600]);
set(gca, 'FontSize', fs, 'FontName', 'Arial','YGrid','on');
% set(gca, 'xscale', 'log');
set(gca,'XTick',0:1:10);
set(gcf,'position',[100 100 640 320]);
set(gcf,'PaperPositionMode','auto');

print('-r0','-depsc','B_90_10.eps');


