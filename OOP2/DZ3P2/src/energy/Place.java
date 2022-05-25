package energy;

import java.awt.*;

public class Place extends Panel {
	public Place(int rows, int columns) {
		super(new GridLayout(rows, columns, 5, 5));

		for(int i = 0; i < rows; i++) {
			for(int j = 0; j < columns; j++) {
				var parcel = Math.random() > 0.7 ? new GrassSurface() : new WaterSurface();

				add(parcel);
			}
		}
	}

	public void chooseParcel(Parcel parcel) {

	}

	public boolean addProducer(Producer producer) {
		return true;
	}

	public void stopAllProducers() {

	}
}
