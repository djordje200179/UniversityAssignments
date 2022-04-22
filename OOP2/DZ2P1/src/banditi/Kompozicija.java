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
		return vagoni.stream()
				.filter(vagon -> vagon.sadrziBandita(bandit)).findFirst()
				.orElseThrow(GNepostojeciVagon::new);
	}

	public Vagon dohvatiSusedniVagon(Vagon vagon, Smer smer) throws GNepostojeciVagon {
		var index = vagoni.indexOf(vagon);

		if (index == -1)
			throw new GNepostojeciVagon();

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
