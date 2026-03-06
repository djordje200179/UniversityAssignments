using ClientApp.ViewModels;
using Common.Models;

namespace ClientApp.Views.Map;

public class PinTemplateSelector : DataTemplateSelector {
	public DataTemplate BicycleTemplate { get; set; } = null!;
	public DataTemplate ParkingTemplate { get; set; } = null!;

	protected override DataTemplate OnSelectTemplate(object item, BindableObject container) => item switch {
		Parking => ParkingTemplate,
		MapViewModel.BicyclePin => BicycleTemplate,
		_ => throw new ArgumentException(nameof(item))
	};
}
