using AdminApp;
using AdminApp.Services;
using Common;
using Common.Services;
using GoogleApi.Extensions;
using GoogleMapsComponents;
using Microsoft.AspNetCore.Components.Web;
using Microsoft.AspNetCore.Components.WebAssembly.Hosting;
using MudBlazor;
using MudBlazor.Services;

var builder = WebAssemblyHostBuilder.CreateDefault(args);

builder.RootComponents.Add<App>("#app");
builder.RootComponents.Add<HeadOutlet>("head::after");

builder.Services
	.AddScoped(sp => new HttpClient { BaseAddress = new Uri(builder.HostEnvironment.BaseAddress) })
	.AddSingleton<IDialogService, DialogService>()
	.AddSingleton<IPrompter, Prompter>()
	.AddCommonServices()
	.AddGoogleApiClients(false)
	.AddMudServices()
	.AddBlazorGoogleMaps("AIzaSyAPo0cmsN7_LxYk7bS-pgUiTWdNMNY8atQ");

await builder.Build().RunAsync();