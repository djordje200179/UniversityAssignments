using Common.Services;
using MudBlazor;

namespace AdminApp.Views;

public partial class LoginView(IPrompter prompter, IAdminRepository adminRepository) {
	MudForm form = default!;
	bool isValid = false;
	string[] errors = [];

	private string username = string.Empty;
	private string password = string.Empty;

	public async Task Login() {
		if (adminRepository.TryLogIn(username, password))
			Console.Write("test");
		else
			await prompter.ShowAlertAsync("Грешка", "Подаци нису исправни. Покушајте поново.");
	}
}
