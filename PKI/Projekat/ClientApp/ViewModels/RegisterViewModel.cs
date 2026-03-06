using Common.Models;
using Common.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;

namespace ClientApp.ViewModels;

public partial class RegisterViewModel(
	IMessenger messenger, IClientRepository clientRepository,
	IPrompter prompter, Shell shell
) : BaseViewModel(messenger) {
	[ObservableProperty]
	public partial string FirstName { get; set; } = string.Empty;
	[ObservableProperty]
	public partial string LastName { get; set; } = string.Empty;
	[ObservableProperty]
	public partial string PhoneNumber { get; set; } = string.Empty;
	[ObservableProperty]
	public partial string EmailAddress { get; set; } = string.Empty;
	[ObservableProperty]
	public partial string Username { get; set; } = string.Empty;
	[ObservableProperty]
	public partial string Password { get; set; } = string.Empty;

	[RelayCommand]
	private Task Register() {
		var client = new Client(FirstName, LastName, PhoneNumber, EmailAddress, Username, Password);

		if (clientRepository.TryRegister(client))
			return shell.GoToAsync("//bicycle-map");
		else
			return prompter.ShowAlertAsync("Грешка", "Корисник већ постоји.");
	}
}
