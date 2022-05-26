package space;

import java.awt.*;
import java.util.Random;
import java.util.stream.IntStream;

public class Comet extends CelestialBody {
	private final int startAngle = new Random().nextInt(360);

	public Comet(int x, int y, int radius) { super(x, y, Color.GRAY, radius); }

	@Override
	public void draw(Graphics g) {
		var oldColor = g.getColor();
		g.setColor(getColor());

		var pentagon = new Polygon();
		IntStream.range(0, 5)
				.map(i -> startAngle + i * (360 / 5))
				.mapToDouble(Math::toRadians)
				.limit(5)
				.forEach(angle -> pentagon.addPoint(
						getX() + (int)(getRadius() * Math.cos(angle)),
						getY() + (int)(getRadius() * Math.sin(angle))
				));
		g.fillPolygon(pentagon);

		g.setColor(oldColor);
	}
}
