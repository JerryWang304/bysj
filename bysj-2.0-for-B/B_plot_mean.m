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
mean = zeros(1,size);
mean(1,1) = prctile(d1,50);
mean(1,2) = prctile(d2,50);
mean(1,3) = prctile(d3,50);
mean(1,4) = prctile(d4,50);
mean(1,5) = prctile(d5,50);
mean(1,6) = prctile(d6,50);
mean(1,7) = prctile(d7,50);
mean(1,8) = prctile(d8,50);
mean(1,9) = prctile(d9,50);
mean(1,10) = prctile(d10,50);
mean(1,11) = prctile(d11,50);
x = 0:1:(size-1);
lw = 1;
ms = 8;
fs = 16;
figure;
plot(x, mean, '-ro', 'LineWidth', lw, 'MarkerSize', ms, 'MarkerFaceColor', 'w');
xlabel('Optimization Interval (0 means per flow)','FontSize', fs, 'FontName', 'Arial');
ylabel('50 Percentile','FontSize', fs, 'FontName', 'Arial');
axis([0 10 0 20]);
% legend(legendkey,'Location', 'NorthEast');
% title('D=1, C=100, U=1, load=2.0', 'FontSize', fs, 'FontName', 'Helvetica');
% axis([0 1 0 600]);
set(gca, 'FontSize', fs, 'FontName', 'Arial','YGrid','on');
% set(gca, 'xscale', 'log');
set(gca,'XTick',0:1:10);
% set(gca,'XTickLabel',XTickk);
set(gcf,'position',[100 100 640 320]);
set(gcf,'PaperPositionMode','auto');

print('-r0','-depsc','B_mean.eps');
% print('-r0','-depsc','result_knock_50.eps');




