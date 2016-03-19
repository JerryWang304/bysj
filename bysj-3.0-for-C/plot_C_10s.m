d1 = dlmread('C:\\c_10s_N1.txt',' ');
d2 = dlmread('C:\\c_10s_N2.txt',' ');
d3 = dlmread('C:\\c_10s_N3.txt',' ');
d4 = dlmread('C:\\c_10s_N4.txt',' ');
d6 = dlmread('C:\\c_10s_N6.txt',' ');
d12 = dlmread('C:\\c_10s_N12.txt',' ');

lw = 4.0;
ms = 10;
fs = 16;

legendkey = {'N=12','N=6','N=4','N=3','N=2','N=1'};

[h1 start1] = cdfplot(d1);
set(h1,'LineStyle','-','color','r','LineWidth',lw);
hold on;
[h2 start2] = cdfplot(d2);
set(h2,'LineStyle','--','color','b','LineWidth',lw);
hold on;
[h3 start3] = cdfplot(d3);
set(h3,'LineStyle','-.','color','g','LineWidth',lw);
hold on;
[h4 start4] = cdfplot(d4);
set(h4, 'LineStyle','-.','color','c','LineWidth',lw);
hold on;
[h6 start6] = cdfplot(d6);
set(h6, 'LineStyle','-.','color','m','LineWidth',lw);
hold on;
[h12 start12] = cdfplot(d12);
set(h12, 'LineStyle','--','color','y','LineWidth',lw);
hold off;

title('');
xlabel('Total Completion Time','FontSize', fs, 'FontName', 'Arial');
ylabel('');
legend(legendkey,'Location', 'NorthEast');
set(gcf,'position',[100 100 640 320]);
set(gca, 'FontSize', fs, 'FontName', 'Arial','YGrid','on');
%set(gca, 'xscale', 'log');
set(gcf,'PaperPositionMode','auto');


