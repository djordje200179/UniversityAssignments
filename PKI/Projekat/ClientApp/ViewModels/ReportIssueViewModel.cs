using Common.Models;
using Common.Services;
using CommunityToolkit.Maui;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;

namespace ClientApp.ViewModels;

[QueryProperty(nameof(Bicycle), "bicycle")]
public partial class ReportIssueViewModel(
	IMessenger messenger, IMediaPicker mediaPicker,
	IPopupService popupService, Shell shell,
	IReportsRepository reportsRepository
) : BaseViewModel(messenger) {
	[ObservableProperty]
	public partial Bicycle Bicycle { get; set; }

	[ObservableProperty]
	public partial string Description { get; set; } = string.Empty;

	[ObservableProperty]
	public partial byte[]? Photo { get; set; }

    [RelayCommand]
    private void RemovePhoto() => Photo = null;

    [RelayCommand]
	private async Task AddPhoto() {
		var result = await mediaPicker.CapturePhotoAsync();
		if (result is null)
			return;

		using var stream = await result.OpenReadAsync();
		
		Photo = new byte[stream.Length];
		await stream.ReadExactlyAsync(Photo.AsMemory());
	}

	[RelayCommand]
	private Task Submit() {
		var report = new Report(reportsRepository.NextId, Description, Bicycle.Id, Photo);
		reportsRepository.Add(report);
		return popupService.ClosePopupAsync(shell);
	}

	[RelayCommand]
	private Task Cancel() => popupService.ClosePopupAsync(shell);
}
