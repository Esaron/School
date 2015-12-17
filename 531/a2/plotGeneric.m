function[] = plotGeneric(xdata, ydata, name)

    f = figure();
    scatter(xdata, ydata);
    axis([0 5 0 150]);
    xlabel('Iteration');
    ylabel('Error');
    saveas(f, name);
    
end