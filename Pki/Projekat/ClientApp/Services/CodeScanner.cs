using Common.Models;
using Common.Services;
using CommunityToolkit.Maui.Extensions;
using System.Text.Json;
using ZXing.Net.Maui;
using ZXing.Net.Maui.Controls;

namespace ClientApp.Services;

public interface ICodeScanner {
	Task<Bicycle?> ScanAsync();
}

public class CodeScanner(Shell shell, IPrompter prompter) : ICodeScanner {
	private readonly CameraBarcodeReaderView view = new() {
		Options = new BarcodeReaderOptions {
			Formats = BarcodeFormats.TwoDimensional
		}
	};

	private void OnBarcodeDetected(object? sender, BarcodeDetectionEventArgs e) {
		var code = e.Results[0];
		MainThread.BeginInvokeOnMainThread(() => shell.ClosePopupAsync(code.Value));
	}

	public async Task<Bicycle?> ScanAsync() {
		try {
			view.BarcodesDetected += OnBarcodeDetected;
			var result = await shell.ShowPopupAsync<string>(view);
			if (result.Result is null || result.WasDismissedByTappingOutsideOfPopup)
				return null;

			return JsonSerializer.Deserialize<Bicycle>(result.Result);
		} catch (JsonException) {
			await prompter.ShowAlertAsync("Грешка", "Невалидан QR код.");
			return null;
		} finally {
			view.BarcodesDetected -= OnBarcodeDetected;
		}
	}
}
