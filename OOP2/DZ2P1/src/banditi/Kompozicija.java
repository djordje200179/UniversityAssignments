package banditi;

import java.util.LinkedList;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

public class Kompozicija {
	private List<Vagon> vagoni = new LinkedList<Vagon>();

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

		try {
			return vagoni.get(index + (smer == Smer.ISPRED ? -1 : 1));
		} catch (IndexOutOfBoundsException e) {
			throw new GNepostojeciVagon();
		}
	}

	@Override
	public String toString() {
		return vagoni.stream().map(Objects::toString).collect(Collectors.joining("_"));
	}
}
