package banditi;

public abstract class Akcija {
	protected final Kompozicija kompozicija;

	public Akcija(Kompozicija kompozicija) {
		this.kompozicija = kompozicija;
	}

	public abstract void izvrsi(Bandit bandit);
}
