package banditi;

import java.util.LinkedList;
import java.util.Objects;
import java.util.stream.Collectors;

public class Vagon {
	private LinkedList<Bandit> banditi = new LinkedList<Bandit>();

	public void dodajBandita(Bandit bandit) {
		banditi.add(bandit);
	}

	public boolean sadrziBandita(Bandit bandit) {
		return banditi.contains(bandit);
	}

	public void ukloniBandita(Bandit bandit) {
		banditi.remove(bandit);
	}

	public Bandit dohvatiBandita(int index) {
		return banditi.get(index);
	}

	public int dohvatiBrojBandita() {
		return banditi.size();
	}

	@Override
	public String toString() {
		return '[' + banditi.stream().map(Objects::toString).collect(Collectors.joining(",")) + ']';
	}
}
