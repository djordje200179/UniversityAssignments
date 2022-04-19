package revija;

public class Velicina {
	public enum Oznaka {
		S, M, L
	};

	private final Oznaka oznaka;

	public Velicina(Oznaka oznaka) {
		this.oznaka = oznaka;
	}

	public Oznaka dohvOznaku() {
		return oznaka;
	}

	public boolean manja(Velicina velicina) {
		return this.oznaka.ordinal() < velicina.oznaka.ordinal();
	}

	@Override
	public String toString() {
		return oznaka.name();
	}
}
