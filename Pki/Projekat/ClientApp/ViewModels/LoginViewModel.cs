using Common.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;

namespace ClientApp.ViewModels;

public partial class LoginViewModel(
	IMessenger messenger, IClientRepository clientRepository,
	IPrompter prompter, Shell shell
) : BaseViewModel(messenger) {
	[ObservableProperty]
	public partial string Username { get; set; } = string.Empty;

	[ObservableProperty]
	public partial string Password { get; set; } = string.Empty;

    [RelayCommand]
    private Task Login() =>
		clientRepository.TryLogin(Username, Password)
        ? shell.GoToAsync("//bicycle-map")
        : prompter.ShowAlertAsync("Грешка", "Подаци нису исправни. Покушајте поново.");

	[RelayCommand]
	private Task Register() => shell.GoToAsync("register");
}
