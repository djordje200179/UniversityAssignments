package banditi;

public class Bandit {
	public enum Tim {A, B}

	private final Tim tim;
	private int brojZlatnika = 50;

	public Bandit(Tim tim) {
		this.tim = tim;
	}

	public Tim dohvatiTim() {
		return tim;
	}

	public int dohvatiBrojZlatnika() {
		return brojZlatnika;
	}

	public void promeniBrojZlatnika(int brojZlatnika) {
		this.brojZlatnika += brojZlatnika;
	}

	@Override
	public String toString() {
		return tim.name() + brojZlatnika;
	}
}
