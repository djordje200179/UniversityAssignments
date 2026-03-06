using Common.Models;
using Common.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using Location = Microsoft.Maui.Devices.Sensors.Location;

namespace ClientApp.ViewModels;

public partial class RideInfoViewModel(
	IMessenger messenger, IMediaPicker mediaPicker, IPrompter prompter,
	IBicycleRepository bicycleRepository, IRideRepository rideRepository,
	IClientRepository clientRepository, Shell shell, IGeolocation geolocation,
	IParkingRepository parkingRepository
) : BaseViewModel(messenger) {
	public Ride Ride { get; } =
		rideRepository
		.Fetch(clientRepository.LoggedIn!.Username)
		.First(r => r.EndTime is null);

	[ObservableProperty]
	[NotifyCanExecuteChangedFor(nameof(EndRideCommand))]
	public partial bool CanEndRide { get; set; } = false;

    protected override async void OnViewAppeared() {
		var currentLocation = await geolocation.GetLastKnownLocationAsync() ?? await geolocation.GetLocationAsync();
		
		CanEndRide =
			parkingRepository
			.Fetch()
			.Select(p => new Location(p.Location.Latitude, p.Location.Longitude))
			.Select(l => Location.CalculateDistance(currentLocation, l, DistanceUnits.Kilometers))
			.Any(d => d <= 0.05);
	}

	[RelayCommand(CanExecute = nameof(CanEndRide))]
	private async Task EndRide() {
		var result = await mediaPicker.CapturePhotoAsync();
		await Task.Delay(1000);

		if (result is null) {
			await prompter.ShowAlertAsync("Грешка", "Није могуће завршити вожњу без фотографије.");
			return;
		}

		using var stream = await result.OpenReadAsync();
		Ride.Photo = new byte[stream.Length];
		await stream.ReadExactlyAsync(Ride.Photo.AsMemory());

		Ride.EndTime = DateTime.Now;
		var bicycle = bicycleRepository.Fetch(Ride.BicycleId);
		bicycle.CurrentStatus = Bicycle.Status.Free;

		await prompter.ShowMessageAsync("Завршена вожња", $"Укупна цијена: {Ride.TotalPrice:0.00} РСД");
		Messenger.Send(new RideChangedMessage());
		await shell.GoToAsync("..");
	}
}
