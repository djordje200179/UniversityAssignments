using ClientApp.ViewModels;

namespace ClientApp.Views;

public partial class LoginView : ContentPage {
	public LoginView(LoginViewModel vm) {
		InitializeComponent();
		BindingContext = vm;
	}
}