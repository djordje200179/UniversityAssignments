package energy;

import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Arrays;

public class Land extends Panel {
	private Plot selectedPlot = null;
	private final int rows, columns;

	public Land(int rows, int columns) {
		super(new GridLayout(rows, columns, 5, 5));

		this.rows = rows;
		this.columns = columns;

		for(var i = 0; i < rows; i++)
			for(var j = 0; j < columns; j++)
				add(generatePlot());

		addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				var source = e.getSource();
				if(source instanceof Plot)
					chooseParcel((Plot)source);
			}
		});
	}

	private Plot generatePlot() {
		var rand = Math.random();

		if(rand > 0.7)
			return new GrassSurface();
		else
			return new WaterSurface();
	}

	public void chooseParcel(Plot newPlot) {
		var oldPlot = selectedPlot;

		if(newPlot == oldPlot)
			return;

		selectedPlot = newPlot;

		for(var i = 0; i < getComponentCount(); i++) {
			var component = (Plot)getComponent(i);
			if(component == oldPlot)
				component.setFont(getFont().deriveFont(14f));
			else if(component == newPlot)
				component.setFont(getFont().deriveFont(20f));
		}
	}

	public boolean addProducer(Producer producer) {
		if(selectedPlot == null || selectedPlot instanceof Producer)
			return false;

		int index;
		for(index = 0; index < getComponentCount(); index++) {
			var component = (Plot)getComponent(index);
			if(component == selectedPlot)
				break;
		}

		remove(selectedPlot);
		if(selectedPlot instanceof Producer)
			((Producer)selectedPlot).stop();

		add(producer, index);

		revalidate();

		updateSurroundingWaterSurfaces();

		selectedPlot = null;

		return true;
	}

	private void updateSurroundingWaterSurfaces() {
		for(var i = 0; i < getComponentCount(); i++) {
			var component = getComponent(i);

			if(!(component instanceof HydroelectricPlant))
				continue;

			var row = i / columns;
			var column = i % columns;

			var surroundingWaterSurfaces = 0;
			if(row > 0 && getPlot(row - 1, column) instanceof WaterSurface)
				surroundingWaterSurfaces++;
			if(row < rows - 1 && getPlot(row + 1, column) instanceof WaterSurface)
				surroundingWaterSurfaces++;
			if(column > 0 && getPlot(row, column - 1) instanceof WaterSurface)
				surroundingWaterSurfaces++;
			if(column < columns - 1 && getPlot(row, column + 1) instanceof WaterSurface)
				surroundingWaterSurfaces++;

			((HydroelectricPlant)component).setSurroundingWaterSurfaces(surroundingWaterSurfaces);
		}
	}

	private Plot getPlot(int row, int column) {
		var index = row * columns + column;
		return (Plot)getComponent(index);
	}

	public void stopAllProducers() {
		Arrays.stream(getComponents())
		      .filter(component -> component instanceof Producer)
		      .map(component -> (Producer)component)
		      .forEach(Producer::stop);
	}
}
