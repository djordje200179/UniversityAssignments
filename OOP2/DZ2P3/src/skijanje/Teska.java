package skijanje;

public class Teska extends Deonica{
	public Teska(double duzina, double nagib) {
		super(duzina, nagib);
	}

	public char oznaka() {
		return 'T';
	}

	public double ubrzanje() {
		return 9.81 * Math.sin(Math.toRadians(nagib));
	}
}
