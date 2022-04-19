package main;

import skijanje.Teska;
import skijanje.Deonica;
import skijanje.GOznaka;
import skijanje.Staza;

public class MainDz {
	public static void main(String[] args) {
		// DZ main
		Deonica deo1 = new Teska(100., 10.), deo2 = new Teska(500., 5.);
		System.out.println(deo1);
		System.out.println(deo2);
		System.out.println("deo1 | oznaka: " + deo1.oznaka());
		System.out.println("deo1 | duzina: " + deo1.duzina());
		System.out.println("deo1 | nagib: " + deo1.nagib());
		System.out.println("deo1 | a: " + deo1.ubrzanje());
		System.out.println("deo1 | Vk: " + deo1.brzina(0));
		System.out.println("deo1 | t: " + deo1.vreme(0));
		Staza gradac = new Staza("Gradac");
		gradac.dodaj(deo1);
		gradac.dodaj(deo2);
		System.out.println(gradac);
		System.out.println("staza | broj delova: " + gradac.broj());
		System.out.println("staza | uk duzina: " + gradac.duzina());
		try {
			System.out.println("staza | oznaka: " + gradac.oznaka());
		} catch (GOznaka o) {
			System.out.println(o);
		}
		System.out.println("staza | maks. nagib: " + gradac.nagib());
		System.out.println("staza | Vk: " + gradac.brzina(0));
		System.out.println("staza | t: " + gradac.vreme(0));
	}
}