package energy;

import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class EnergySystem extends Frame {
	private final Place place;
	private final Battery battery;

	public EnergySystem(int placeRows, int placeColumns, int batteryCapacity) {
		setSize(500, 500);

		place = new Place(placeRows, placeColumns);
		battery = new Battery(batteryCapacity);

		add(place);
		addButton();

		addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e) {
				place.stopAllProducers();

				dispose();
			}
		});

		setResizable(false);
		setVisible(true);
	}

	private void addButton() {
		var button = new Button("Dodaj");
		button.addActionListener(e -> {
			place.addProducer(new Hydroplant(battery));
		});

		add(button, BorderLayout.NORTH);
	}

	public static void main(String[] args) {
		new EnergySystem(5, 5, 100);
	}
}
