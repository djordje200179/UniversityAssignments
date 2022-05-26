package energy;

import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class Plot extends Label {
	public Plot(char label, Color backgroundColor) {
		super(String.valueOf(label));

		setForeground(Color.WHITE);
		setBackground(backgroundColor);
		setFont(new Font("Serif", Font.PLAIN, 14));
		setAlignment(Label.CENTER);

		addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				getParent().dispatchEvent(e);
			}
		});
	}
}
