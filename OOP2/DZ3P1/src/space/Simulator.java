package space;

import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class Simulator extends Frame {
	private final Space space = new Space();
	private final Panel commands = new Panel();
	private final Generator generator = new Generator(space);

	public Simulator() {
		setSize(200, 400);

		setupCommands();

		add(space);
		add(commands, BorderLayout.SOUTH);

		addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e) {
				try {
					generator.stop();
					synchronized(generator) { generator.wait(); }

					space.stopActivity();
					synchronized(space) { space.wait(); }
				} catch(InterruptedException ignored) { }

				dispose();
			}
		});

		setVisible(true);
	}

	private void setupCommands() {
		var button = new Button("Pokreni!");
		button.addActionListener(e -> {
			generator.start();
			space.startActivity();
			button.setEnabled(false);
		});

		commands.add(button);
	}

	public static void main(String[] args) {
		new Simulator();
	}
}
