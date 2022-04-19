package banditi;

public class Pomeranje extends Akcija {
	private final Smer smer;

	public Pomeranje(Kompozicija kompozicija, Smer smer) {
		super(kompozicija);
		this.smer = smer;
	}

	public void izvrsi(Bandit bandit) {
		try {
			var vagon = kompozicija.dohvatiVagon(bandit);
			var noviVagon = kompozicija.dohvatiSusedniVagon(vagon, smer);

			vagon.ukloniBandita(bandit);
			noviVagon.dodajBandita(bandit);
		} catch (GNepostojeciVagon ignored) {}
	}

	@Override
	public String toString() {
		return "Pomeranje:" + smer;
	}
}
