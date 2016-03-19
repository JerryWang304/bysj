d1 = dlmread('C:\\d_0.txt',' ');
d2 = dlmread('C:\\d_2.txt',' ');
d3 = dlmread('C:\\d_4.txt',' ');
d4 = dlmread('C:\\d_6.txt',' ');
d5 = dlmread('C:\\d_8.txt',' ');
d6 = dlmread('C:\\d_10.txt',' ');

linestyle = ['--','-','-.','.'];
color = ['r','b','g','c','m'];
legendpos = 'Best';
marker = ['s','o','^','v','d'];
lw = 4.0;
ms = 10;
fs = 16;
% legendkey = {'Per Flow','T=1','T=2','T=4','T=6', 'T=12'};
% legendkey = {'N=12','N=6','N=4','N=3','N=2', 'N=1'};
% legendkey = {'Per Flow','T=1','T=5','T=10'};
legendkey = {'\sigma=0','\sigma=2','\sigma=4','\sigma=6','\sigma=8','\sigma=10'};


[h1, stat1] = cdfplot(d1);
% set(h1, 'LineStyle','-','color','r','LineWidth',lw, 'Marker','o','MarkerSize',4);
set(h1, 'LineStyle','-','color','r','LineWidth',lw);
hold on;
[h2, stat2] = cdfplot(d2);
set(h2, 'LineStyle','-','color','b','LineWidth',lw);
hold on;
[h3, stat3] = cdfplot(d3);
set(h3, 'LineStyle','-.','color','g','LineWidth',lw);
hold on;
[h4, stat4] = cdfplot(d4);
set(h4, 'LineStyle','-.','color','c','LineWidth',lw);
hold on;
[h5, stat5] = cdfplot(d5);
set(h5, 'LineStyle','--','color','m','LineWidth',lw);
hold on;
[h6, stat6] = cdfplot(d6);
set(h6, 'LineStyle','--','color','y','LineWidth',lw);

hold off;
title('');
xlabel('Total Completion Time','FontSize', fs, 'FontName', 'Arial');
ylabel('');
legend(legendkey,'Location', 'SouthEast');
set(gcf,'position',[100 100 640 320]);
set(gca, 'FontSize', fs, 'FontName', 'Arial','YGrid','on');
%set(gca, 'xscale', 'log');
set(gcf,'PaperPositionMode','auto');
%print('-r0','-depsc', strcat(filename, '.eps'));
% ps2pdf -dEPSCrop result_GP_per.eps
clear;
