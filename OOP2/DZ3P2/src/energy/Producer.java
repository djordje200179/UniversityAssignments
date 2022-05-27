package energy;

import java.awt.*;
import java.util.Random;

public abstract class Producer extends Plot {
	private final int producingTime;
	private final Battery battery;
	private final Thread thread = new Thread(this::loop);

	public Producer(char label, Color backgroundColor, int basicTime, Battery battery) {
		super(label, backgroundColor);

		this.producingTime = basicTime + new Random().nextInt(301);
		this.battery = battery;

		thread.start();
	}

	protected abstract int producingEnergyUnits();
	protected abstract boolean isProductionSuccessful();

	private void loop() {
		try {
			while(!Thread.interrupted()) {
				Thread.sleep(producingTime);

				battery.addEnergy(producingEnergyUnits());

				var originalColor = getForeground();
				if(isProductionSuccessful())
					setForeground(Color.RED);

				Thread.sleep(300);

				setForeground(originalColor);
			}
		} catch(InterruptedException ignored) { }
	}

	public void stop() {
		if(!thread.isAlive())
			return;

		thread.interrupt();
		try {
			thread.join();
		} catch(InterruptedException ignored) { }
	}
}
