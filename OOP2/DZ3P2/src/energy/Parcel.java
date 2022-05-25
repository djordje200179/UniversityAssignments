package energy;

import java.awt.*;

public class Parcel extends Label {
	public Parcel(char label, Color backgroundColor) {
		super(String.valueOf(label));

		setForeground(Color.WHITE);
		setBackground(backgroundColor);
		setFont(new Font("Serif", Font.BOLD, 14));
	}

	public void setBackgroundColor(Color color) { setBackground(color); }
}
