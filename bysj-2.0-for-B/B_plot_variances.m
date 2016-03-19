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
data = zeros(1,size);
data(1,1) = var(d1);
data(1,2) = var(d2);
data(1,3) = var(d3);
data(1,4) = var(d4);
data(1,5) = var(d5);
data(1,6) = var(d6);
data(1,7) = var(d7);
data(1,8) = var(d8);
data(1,9) = var(d9);
data(1,10) = var(d10);
data(1,11) = var(d11);

x = 0:1:10;

lw = 1;
ms = 8;
fs = 16;

figure;
plot(x, data, '-ro', 'LineWidth', lw, 'MarkerSize', ms, 'MarkerFaceColor', 'w');
xlabel('Optimization Interval (0 means per flow)','FontSize', fs, 'FontName', 'Arial');
ylabel('Variance','FontSize', fs, 'FontName', 'Arial');
axis([0 10 0 2]);
% legend(legendkey,'Location', 'NorthEast');
% title('D=1, C=100, U=1, load=2.0', 'FontSize', fs, 'FontName', 'Helvetica');
% axis([0 1 0 600]);
set(gca, 'FontSize', fs, 'FontName', 'Arial','YGrid','on');
% set(gca, 'xscale', 'log');
set(gca,'XTick',0:1:10);
set(gcf,'position',[100 100 640 320]);
set(gcf,'PaperPositionMode','auto');

print('-r0','-depsc','result_temporal_final_var.eps');
