package energy;

import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class EnergySystem extends Frame {
	private final Land land;
	private final Battery battery;

	public EnergySystem(int placeRows, int placeColumns, int batteryCapacity) {
		setSize(500, 500);

		land = new Land(placeRows, placeColumns);
		battery = new Battery(batteryCapacity);

		add(land);
		addButton();

		setTitle("Energetski sistem");

		addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e) {
				land.stopAllProducers();

				dispose();
			}
		});

		setResizable(false);
		setVisible(true);
	}

	private void addButton() {
		var panel = new Panel();

		var button = new Button("Dodaj");
		button.addActionListener(e -> {
			land.addProducer(new HydroelectricPlant(battery));
		});

		panel.add(button);

		add(panel, BorderLayout.NORTH);
	}

	public static void main(String[] args) {
		new EnergySystem(5, 5, 100);
	}
}
