package space;

import java.awt.*;

public abstract class CelestialBody extends Object {
	private final int radius;

	public CelestialBody(int x, int y, Color color, int radius) {
		super(x, y, color);

		this.radius = radius;
	}

	protected int getRadius() {	return radius; }
}