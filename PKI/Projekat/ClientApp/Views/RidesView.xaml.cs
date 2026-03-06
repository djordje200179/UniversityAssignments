using ClientApp.ViewModels;

namespace ClientApp.Views;

public partial class RidesView : ContentPage {
	public RidesView(RidesViewModel vm) {
		InitializeComponent();
		BindingContext = vm;
	}
}