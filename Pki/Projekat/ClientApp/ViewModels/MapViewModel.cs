using Common.Models;
using Common.Services;
using CommunityToolkit.Maui;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using ClientApp.Services;
using Location = Microsoft.Maui.Devices.Sensors.Location;
using CommunityToolkit.Mvvm.ComponentModel;

namespace ClientApp.ViewModels;

public partial class MapViewModel(
	IMessenger messenger, IClientRepository clientRepository,
	IBicycleRepository bicycleRepository, IParkingRepository parkingRepository,
	IRideRepository rideRepository, Shell shell, IPopupService popupService,
	ICodeScanner codeScanner
) : BaseViewModel(messenger), IRecipient<RideChangedMessage> {
	public record struct BicyclePin(Bicycle Bicycle, IDictionary<string, double> ParkingDistances);

	private readonly Parking[] Parkings =
		parkingRepository
		.Fetch()
		.ToArray();

	private Ride? GetActiveRide() =>
		rideRepository
		.Fetch(clientRepository.LoggedIn!.Username)
		.FirstOrDefault(r => r.EndTime is null);

	[ObservableProperty]
	public partial Ride? ActiveRide { get; set; }

    protected override void OnViewAppeared() {
        base.OnViewAppeared();
		ActiveRide = GetActiveRide();
	}

	private static Location ToMauiLocation(Common.Models.Location location) => new(location.Latitude, location.Longitude);

	private BicyclePin[] Bicycles =>
		bicycleRepository
		.Fetch()
		.Where(b => b.CurrentStatus == Bicycle.Status.Free)
		.Select(b => new BicyclePin(b,
			Parkings
			.Select(p => (p.Name, Distance: Location.CalculateDistance(
				ToMauiLocation(b.CurrentLocation), ToMauiLocation(p.Location),
				DistanceUnits.Kilometers
			)))
			.OrderBy(x => x.Distance)
			.Take(3)
			.ToDictionary(x => x.Name, x => x.Distance)
		))
		.ToArray();

	public IEnumerable<object> MapPins =>
		Parkings
		.Cast<object>()
		.Concat(Bicycles.Cast<object>());

	[RelayCommand]
	private Task Logout() {
		clientRepository.Logout();
		return shell.GoToAsync("//login");
	}

	[RelayCommand]
	private async Task ReportIssue() {
		var bicycle = await codeScanner.ScanAsync();
		if (bicycle is null)
			return;

		await popupService.ShowPopupAsync<ReportIssueViewModel, string>(
			shell, PopupOptions.Empty, 
			new Dictionary<string, object> {
				["bicycle"] = bicycle
			}
		);
	}

	[RelayCommand]
	private async Task StartRide() {
		var bicycle = await codeScanner.ScanAsync();
		if (bicycle is null)
			return;

		bicycle.CurrentStatus = Bicycle.Status.Rented;
		ActiveRide = new Ride(
			rideRepository.NextId,
			clientRepository.LoggedIn!.Username,
			bicycle.Id,
			bicycle.PricePerHour,
			DateTime.Now
		);

		rideRepository.Add(ActiveRide);
		Messenger.Send(new RideChangedMessage());
	}

	[RelayCommand]
	private Task ViewRideInfo() {
		ArgumentNullException.ThrowIfNull(ActiveRide);

		return shell.GoToAsync($"ride-info");
	}

	public void Receive(RideChangedMessage message) => ActiveRide = GetActiveRide();
}
