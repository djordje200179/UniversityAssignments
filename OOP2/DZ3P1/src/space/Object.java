package space;

import java.awt.*;

public abstract class Object {
	private int x, y;
	private final Color color;

	public Object(int x, int y, Color color) {
		this.x = x;
		this.y = y;
		this.color = color;
	}

	public int getX() {	return x; }
	public int getY() {	return y; }
	protected Color getColor() { return color; }

	public void moveX(int x) { this.x += x; }
	public void moveY(int y) { this.y += y; }

	public abstract void draw(Graphics g);
}