using Common.Services;
using CommunityToolkit.Maui.Alerts;
using CommunityToolkit.Maui.Core;

namespace ClientApp.Services;

public class Prompter : IPrompter {
    public Task ShowAlertAsync(string title, string message) {
		var snackbar = Snackbar.Make(message, null, "ОК", TimeSpan.FromSeconds(3), new SnackbarOptions {
			BackgroundColor = Colors.Red,
			ActionButtonTextColor = Colors.Yellow,
			CornerRadius = new CornerRadius(10),
			CharacterSpacing = 0.2
		});

		return snackbar.Show();
	}

	public Task ShowMessageAsync(string title, string message) {
		var snackbar = Snackbar.Make(message, null, "ОК", TimeSpan.FromSeconds(3), new SnackbarOptions {
			BackgroundColor = Colors.Green,
			ActionButtonTextColor = Colors.White,
			CornerRadius = new CornerRadius(10),
			CharacterSpacing = 0.2
		});
		return snackbar.Show();
	}
}
