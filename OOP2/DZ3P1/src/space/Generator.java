package space;

import java.util.Random;

public class Generator {
	private final Space space;
	private final Thread thread = new Thread(this::generateComets);

	public Generator(Space space) {
		this.space = space;
	}

	public void start() { thread.start(); }
	public void stop() {
		if(!thread.isAlive())
			return;

		thread.interrupt();
		try {
			synchronized(this) { wait(); }
		} catch(InterruptedException ignored) { }
	}

	private void generateComets() {
		var randomGenerator = new Random();

		try {
			while(!Thread.interrupted()) {
				var x = randomGenerator.nextInt(200);
				var y = 0;
				var radius = randomGenerator.nextInt(10, 30);

				var comet = new Comet(x, y, radius);
				space.addBody(comet);

				Thread.sleep(900);
			}
		} catch(InterruptedException ignored) { }

		synchronized(this) { notify(); }
	}
}
