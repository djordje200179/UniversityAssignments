using Common.Services;
using MudBlazor;

namespace AdminApp.Services;

public class Prompter(IDialogService dialogService) : IPrompter {
	public Task ShowAlertAsync(string title, string message) => dialogService.ShowMessageBox(new MessageBoxOptions {
		Title = title,
		Message = message,
		YesText = "ОК",
	}, new DialogOptions {

	});

    public Task ShowMessageAsync(string title, string message) => dialogService.ShowMessageBox(new MessageBoxOptions {
		Title = title,
		Message = message,
		YesText = "ОК",
	}, new DialogOptions {
		
	});
}
