package revija;

public class Odeca implements Nosivo {
	private final String naziv;
	private final Velicina velicina;

	public Odeca(String naziv, Velicina velicina) {
		this.naziv = naziv;
		this.velicina = velicina;
	}

	public boolean odgovara(Model model) {
		return !velicina.manja(model.dohvVelicinu());
	}

	public String dohvNaziv() {
		return naziv;
	}

	public Velicina dohvVelicinu() {
		return velicina;
	}

	@Override
	public String toString() {
		return "odeća" + ' ' + naziv + ' ' + velicina;
	}
}
