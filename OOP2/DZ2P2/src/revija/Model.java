package revija;

public class Model {
	private static int idGenerator = 0;
	private final int id = idGenerator++;

	private final Velicina velicina;

	public Model(Velicina velicina) {
		this.velicina = velicina;
	}

	public int dohvId() {
		return id;
	}

	public Velicina dohvVelicinu() {
		return velicina;
	}

	@Override
	public String toString() {
		return "Model " + id + ' ' + '(' + velicina + ')';
	}
}