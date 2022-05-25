package energy;

import java.awt.*;
import java.util.Random;

public abstract class Producer extends Parcel {
	private final int producingTime;
	private final Battery battery;
	private final Color originalColor;
	private final Thread thread = new Thread(this::loop);

	public Producer(char label, Color backgroundColor, int basicTime, Battery battery) {
		super(label, backgroundColor);

		this.producingTime = basicTime + new Random().nextInt(301);
		this.battery = battery;
		this.originalColor = backgroundColor;

		thread.start();
	}

	protected abstract int producingEnergyUnits();
	protected abstract boolean isProductionSuccessful();

	private void loop() {
		try {
			while(!Thread.interrupted()) {
				Thread.sleep(producingTime);

				battery.addEnergy(producingEnergyUnits());

				if(isProductionSuccessful())
					setBackgroundColor(Color.RED);

				Thread.sleep(300);

				setBackgroundColor(originalColor);
			}
		} catch(InterruptedException ignored) { }
	}

	public void stop() { thread.interrupt(); }
}
