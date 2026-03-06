using Common.Models;
using Common.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;

namespace ClientApp.ViewModels; 

public partial class SettingsViewModel(
	IMessenger messenger, 
	IClientRepository clientRepository, IRideRepository rideRepository,
	IPrompter prompter, Shell shell
) : BaseViewModel(messenger) {
	private Client client = null!;

	[ObservableProperty]
	public partial string FirstName { get; set; }
	[ObservableProperty]
	public partial string LastName { get; set; }
	[ObservableProperty]
	public partial string Username { get; set; }

	[ObservableProperty]
	public partial string PhoneNumber { get; set; }
	[ObservableProperty]
	public partial string EmailAddress { get; set; }

	[ObservableProperty]
	[NotifyCanExecuteChangedFor(nameof(ChangePasswordCommand))]
	public partial string OldPassword { get; set; }
	[ObservableProperty]
	[NotifyCanExecuteChangedFor(nameof(ChangePasswordCommand))]
	public partial string NewPassword { get; set; }
	[ObservableProperty]
	[NotifyCanExecuteChangedFor(nameof(ChangePasswordCommand))]
	public partial string ConfirmedNewPassword { get; set; }

    protected override void OnViewAppeared() {
		client = clientRepository.LoggedIn!;

		FirstName = client.FirstName;
		LastName = client.LastName;
		Username = client.Username;
		PhoneNumber = client.PhoneNumber;
		EmailAddress = client.EmailAddress;
	}

	[RelayCommand]
	private Task SaveChanges() {
		if (client.Username != Username) {
			if (clientRepository.TryChangeUsername(Username))
				rideRepository.UpdateClientUsername(client.Username, Username);
			else
				return prompter.ShowAlertAsync("Грешка", "Корисничко име је заузето");
		}

		client.FirstName = FirstName;
		client.LastName = LastName;
		client.PhoneNumber = PhoneNumber;
		client.EmailAddress = EmailAddress;

		return prompter.ShowMessageAsync("Успјех", "Промјене су сачуване");
	}

	[RelayCommand(CanExecute = nameof(CanChangePassword))]
	private Task ChangePassword() {
		if (NewPassword != ConfirmedNewPassword) {
			return prompter.ShowAlertAsync("Грешка", "Нова лозинка није потврђена");
		} else if (OldPassword != client.Password) {
			return prompter.ShowAlertAsync("Грешка", "Стара лозинка није исправна");
		}
		
		client.Password = NewPassword;
		OldPassword = NewPassword = ConfirmedNewPassword = string.Empty;
		return prompter.ShowMessageAsync("Успјех", "Промјене су сачуване");
	}

	private bool CanChangePassword() =>
		!string.IsNullOrWhiteSpace(OldPassword) &&
		!string.IsNullOrWhiteSpace(NewPassword) &&
		!string.IsNullOrWhiteSpace(ConfirmedNewPassword);

	[RelayCommand]
	private Task Logout() {
		clientRepository.Logout();
		return shell.GoToAsync("//login");
	}
}
