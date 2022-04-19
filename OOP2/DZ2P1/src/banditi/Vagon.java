package banditi;

import java.util.LinkedList;

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
		var sb = new StringBuilder();

		sb.append('[');
		for (var bandit : banditi)
			sb.append(bandit).append(',');
		if(!banditi.isEmpty())
			sb.deleteCharAt(sb.length() - 1);
		sb.append(']');

		return sb.toString();
	}
}
