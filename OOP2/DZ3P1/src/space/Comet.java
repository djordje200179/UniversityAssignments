package space;

import java.awt.*;
import java.util.Random;
import java.util.function.ObjDoubleConsumer;
import java.util.stream.IntStream;

public class Comet extends CelestialBody {
	private final int startAngle = new Random().nextInt(360);

	public Comet(int x, int y, int radius) { super(x, y, Color.GRAY, radius); }

	private Polygon createPentagon() {
		ObjDoubleConsumer<Polygon> addPoint = (polygon, angle) -> {
			var x = getX() + (getRadius() * Math.cos(angle));
			var y = getY() + (getRadius() * Math.sin(angle));

			polygon.addPoint((int)x, (int)y);
		};

		return IntStream.range(0, 5)
		                .map(i -> startAngle + i * (360 / 5))
		                .mapToDouble(Math::toRadians)
		                .collect(Polygon::new, addPoint, (a, b) -> { });
	}
	@Override
	public void draw(Graphics g) {
		var oldColor = g.getColor();
		g.setColor(getColor());

		g.fillPolygon(createPentagon());

		g.setColor(oldColor);
	}
}
