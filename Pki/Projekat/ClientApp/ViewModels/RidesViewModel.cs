using Common.Models;
using Common.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;

namespace ClientApp.ViewModels;

public sealed class RideChangedMessage { }

public partial class RidesViewModel(
	IMessenger messenger, Shell shell,
	IClientRepository clientRepository, IRideRepository rideRepository
) : BaseViewModel(messenger), IRecipient<RideChangedMessage> {
	private IEnumerable<Ride> GetRides() =>
			rideRepository
			.Fetch(clientRepository.LoggedIn!.Username)
			.OrderByDescending(r => r.StartTime)
			.ToArray();

	[ObservableProperty]
	public partial IEnumerable<Ride> Rides { get; set; } = [];

	[RelayCommand]
	private Task Logout() {
		clientRepository.Logout();
		return shell.GoToAsync("//login");
	}

    protected override void OnViewAppeared() => Rides = GetRides();
	public void Receive(RideChangedMessage message) => Rides = GetRides();
}
