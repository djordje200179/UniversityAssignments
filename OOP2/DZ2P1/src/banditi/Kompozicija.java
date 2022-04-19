package banditi;

import java.util.LinkedList;

public class Kompozicija {
	private LinkedList<Vagon> vagoni = new LinkedList<Vagon>();

	public void dodajVagon(Vagon vagon) {
		vagoni.add(vagon);
	}

	public Vagon dohvatiVagon(Bandit bandit) throws GNepostojeciVagon {
		for (Vagon vagon : vagoni)
			if (vagon.sadrziBandita(bandit))
				return vagon;

		throw new GNepostojeciVagon();
	}

	public Vagon dohvatiSusedniVagon(Vagon vagon, Smer smer) throws GNepostojeciVagon {
		var index = vagoni.indexOf(vagon);

		if(smer == Smer.ISPRED) {
			if(index == 0)
				throw new GNepostojeciVagon();

			return vagoni.get(index - 1);
		} else if(smer == Smer.IZA) {
			if(index == vagoni.size() - 1)
				throw new GNepostojeciVagon();

			return vagoni.get(index + 1);
		}

		return null;
	}

	@Override
	public String toString() {
		if(vagoni.isEmpty())
			return "";

		var sb = new StringBuilder();

		for (var vagon : vagoni)
			sb.append(vagon).append('_');

		sb.deleteCharAt(sb.length() - 1);

		return sb.toString();
	}
}
