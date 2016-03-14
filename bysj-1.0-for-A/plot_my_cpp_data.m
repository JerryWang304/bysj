x=dlmread('c:\\opt_time_1s.txt',' ');
y=dlmread('c:\\tot_thr_1s.txt',' ');
scale = 100;
y = y*scale;

y(1) = y(2)

% 去掉最后一个0 。。。搞不懂为什么会有....txt文件里面明明没有
x(:,length(x))=[]
y(:,length(y))=[]
lw = 1.45;
ms = 8;
fs = 11.5;
figure;
plot(x,y,'-ro', 'LineWidth', lw, 'MarkerSize', ms, 'MarkerFaceColor', 'w');
axis([0 18 0 12]);
set(gca,'XTick',0:1:18);
set(gca,'YTick',0:1:12);
xlabel('Time','FontSize', fs, 'FontName', 'Arial');
ylabel('Total Throughput','FontSize', fs, 'FontName', 'Arial');
set(gca, 'FontSize', fs, 'FontName', 'Arial','YGrid','on');
%set(gcf,'position',[100 100 640 320]);
set(gcf,'PaperPositionMode','auto');
print('-r0','-depsc');