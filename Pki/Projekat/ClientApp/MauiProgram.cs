using ClientApp.Services;
using ClientApp.ViewModels;
using ClientApp.Views;
using Common;
using Common.Services;
using CommunityToolkit.Maui;
using CommunityToolkit.Mvvm.Messaging;
using Microsoft.Extensions.Logging;
using Microsoft.Maui.Maps.Handlers;
using ZXing.Net.Maui.Controls;

namespace ClientApp;

public static class MauiProgram {
    public static MauiApp CreateMauiApp() {
        var builder = MauiApp.CreateBuilder();
        builder
            .UseMauiApp<App>()
			.UseBarcodeReader()
#if ANDROID
			.UseMauiMaps()
#endif
			.ConfigureFonts(fonts => {
				fonts.AddFont("Font Awesome 7 Free-Regular-400.otf", "FARegular");
				fonts.AddFont("Font Awesome 7 Free-Solid-900.otf", "FASolid");
			})
			.ConfigureMauiHandlers(handlers => {
#if ANDROID
				handlers.AddHandler<Microsoft.Maui.Controls.Maps.Map, Platforms.Android.CustomMapHandler>();
#endif
			})
            .UseMauiCommunityToolkit(options => {
				options.SetShouldEnableSnackbarOnWindows(true);
			})
			.RegisterServices()
			.RegisterViews();

#if DEBUG
		builder.Logging.AddDebug();
		builder.Services.AddLogging(configure => configure.AddDebug());
#endif

        return builder.Build();
    }

	extension(MauiAppBuilder builder) {
		private MauiAppBuilder RegisterServices() {
			builder.Services
				.AddSingleton<IMessenger>(WeakReferenceMessenger.Default)
				.AddCommonServices()
				.AddSingleton<IPrompter, Prompter>()
				.AddSingleton<ICodeScanner, CodeScanner>()
				.AddSingleton(MediaPicker.Default)
				.AddSingleton(Geolocation.Default)
				.AddSingleton(sp => Shell.Current);

			return builder;
		}

		private MauiAppBuilder RegisterViews() {
			builder.Services
				.AddSingleton<LoginView, LoginViewModel>()
				.AddSingletonWithShellRoute<RegisterView, RegisterViewModel>("register")
				.AddSingleton<MapView, MapViewModel>()
				.AddSingleton<SettingsView, SettingsViewModel>()
				.AddSingleton<RidesView, RidesViewModel>()
				.AddTransientWithShellRoute<RideInfo, RideInfoViewModel>("ride-info")
				.AddTransientPopup<ReportIssueView, ReportIssueViewModel>();

			return builder;
		}
	}
}