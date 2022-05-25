package space;

import java.awt.*;
import java.util.Random;
import java.util.stream.IntStream;

public class Comet extends CelestialBody {
	private final int startAngle = new Random().nextInt(360);

	public Comet(int x, int y, int radius) {
		super(x, y, Color.GRAY, radius);
	}

	@Override
	public void paint(Graphics g) {
		var oldColor = g.getColor();
		g.setColor(color);

		var pentagon = new Polygon();
		IntStream.range(0, 5)
				.map(i -> startAngle + i * (360 / 5))
				.mapToDouble(Math::toRadians)
				.limit(5)
				.forEach(angle -> pentagon.addPoint(
						getX() + (int)(radius * Math.cos(angle)),
						getY() + (int)(radius * Math.sin(angle))
				));
		g.fillPolygon(pentagon);

		g.setColor(oldColor);
	}
}
