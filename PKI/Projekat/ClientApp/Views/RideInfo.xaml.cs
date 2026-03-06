using ClientApp.ViewModels;

namespace ClientApp.Views;

public partial class RideInfo : ContentPage {
	public RideInfo(RideInfoViewModel vm) {
		InitializeComponent();
		BindingContext = vm;
	}
}