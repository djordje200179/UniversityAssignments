package skijanje;

import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

public class Staza {
	private LinkedList<Deonica> deonice = new LinkedList<Deonica>();
	private final String naziv;

	public Staza(String naziv) {
		this.naziv = naziv;
	}

	public void dodaj(Deonica deonica) {
		deonice.add(deonica);
	}

	public int broj() {
		return deonice.size();
	}

	public double duzina() {
		return deonice.stream().mapToDouble(Deonica::duzina).sum();
	}

	public double nagib() {
		return deonice.stream().mapToDouble(Deonica::nagib).max().getAsDouble();
	}

	public char oznaka() throws GOznaka {
		if(deonice.isEmpty())
			throw new GOznaka();

		var brojaci = new HashMap<Character, Integer>();

		for (var deonica : deonice) {
			var oznaka = deonica.oznaka();

			brojaci.put(oznaka, brojaci.containsKey(oznaka) ? brojaci.get(oznaka) + 1 : 1);
		}

		return brojaci.entrySet().stream().max(Comparator.comparingInt(Map.Entry::getValue)).get().getKey();
	}

	public double brzina(double pocetnaBrzina) {
		var trenutnaBrzina = pocetnaBrzina;
		for(var deonica : deonice)
			trenutnaBrzina = deonica.brzina(trenutnaBrzina);

		return trenutnaBrzina;
	}

	public double vreme(double pocetnaBrzina) {
		var trenutnoVreme = 0.0;
		var trenutnaBrzina = pocetnaBrzina;

		for(var deonica : deonice) {
			trenutnoVreme += deonica.vreme(trenutnaBrzina);
			trenutnaBrzina = deonica.brzina(trenutnaBrzina);
		}

		return trenutnoVreme;
	}

	@Override
	public String toString() {
		var sb = new StringBuilder();

		sb.append(naziv).append('|').append(broj()).append('|').append(duzina()).append('|').append(nagib());
		sb.append('\n');

		sb.append('[');
		for(var deonica : deonice)
			sb.append(deonica).append(',');
		sb.deleteCharAt(sb.length() - 1);
		sb.append(']');

		return sb.toString();
	}
}