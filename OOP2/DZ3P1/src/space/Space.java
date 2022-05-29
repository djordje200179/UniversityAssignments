package space;

import java.awt.*;
import java.util.Vector;

public class Space extends Canvas {
	private final Vector<CelestialBody> bodies = new Vector<>();
	private final Thread thread = new Thread(this::loop);

	public Space() {
		setBackground(Color.BLACK);
	}

	public void addBody(CelestialBody body) {
		synchronized(bodies) {
			bodies.add(body);
		}
	}

	public void startActivity() { thread.start(); }

	public void stopActivity() {
		if(!thread.isAlive())
			return;

		thread.interrupt();
		try {
			thread.join();
		} catch(InterruptedException ignored) { }
	}

	@Override
	public void paint(Graphics g) {
		synchronized(bodies) {
			for(var body : bodies)
				body.draw(g);
		}
	}

	private void move() {
		synchronized(bodies) {
			for(var body : bodies)
				body.moveY(5);
		}
	}

	private void loop() {
		try {
			while(!Thread.interrupted()) {
				repaint();
				move();

				Thread.sleep(100);
			}
		} catch(InterruptedException ignored) { }
	}
}
