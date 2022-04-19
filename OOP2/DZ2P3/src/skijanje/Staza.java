package skijanje;

import java.util.*;
import java.util.stream.Collectors;

public class Staza {
	private List<Deonica> deonice = new ArrayList<Deonica>();
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
		if (deonice.isEmpty())
			throw new GOznaka();

		return deonice
				.stream().collect(Collectors.groupingBy(Deonica::oznaka))
				.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey, e -> e.getValue().size()))
				.entrySet().stream().max(Map.Entry.comparingByValue()).get().getKey();
	}

	public double brzina(double pocetnaBrzina) {
		return deonice.stream().reduce(pocetnaBrzina, (trenutnaBrzina, deonica) -> deonica.brzina(trenutnaBrzina), (a, b) -> a);
	}

	public double vreme(double pocetnaBrzina) {
		var trenutnoVreme = 0.0;
		var trenutnaBrzina = pocetnaBrzina;

		for (var deonica : deonice) {
			trenutnoVreme += deonica.vreme(trenutnaBrzina);
			trenutnaBrzina = deonica.brzina(trenutnaBrzina);
		}

		return trenutnoVreme;
	}

	@Override
	public String toString() {
		var zaglavlje = naziv + '|' + broj() + '|' + duzina() + '|' + nagib();
		var lista = '[' + deonice.stream().map(Objects::toString).collect(Collectors.joining(",")) + ']';

		return zaglavlje + '\n' + lista;
	}
}