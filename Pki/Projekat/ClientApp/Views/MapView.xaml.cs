using ClientApp.ViewModels;

namespace ClientApp.Views;

public partial class MapView : ContentPage {
	public MapView(MapViewModel vm) {
		InitializeComponent();
		BindingContext = vm;
	}
}