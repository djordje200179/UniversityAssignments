using ClientApp.ViewModels;

namespace ClientApp.Views;

public partial class RegisterView : ContentPage {
	public RegisterView(RegisterViewModel vm) {
		InitializeComponent();
		BindingContext = vm;
	}
}