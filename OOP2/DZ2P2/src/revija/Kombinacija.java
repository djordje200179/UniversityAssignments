package revija;

public class Kombinacija {
	private final Nosivo[] stvari;
	private int brojDodatihStvari = 0;

	public Kombinacija(int brojNosivihStvari) {
		stvari = new Nosivo[brojNosivihStvari];
	}

	public void dodaj(Nosivo stvar) throws GDodavanje {
		if(brojDodatihStvari == stvari.length)
			throw new GDodavanje();

		stvari[brojDodatihStvari++] = stvar;
	}

	public int dohvBrStvari() {
		return brojDodatihStvari;
	}

	public int dohvMaxBrStvari() {
		return stvari.length;
	}

	public Nosivo dohvStvar(int index) throws GIndeks {
		if(index < 0 || index >= brojDodatihStvari || stvari[index] == null)
			throw new GIndeks();
		else
			return stvari[index];
	}

	@Override
	public String toString() {
		var sb = new StringBuilder();

		sb.append('[');
		for(int i = 0; i < brojDodatihStvari; i++) {
			if(i != 0)
				sb.append(", ");

			sb.append(stvari[i]);
		}
		sb.append(']');

		return sb.toString();
	}
}