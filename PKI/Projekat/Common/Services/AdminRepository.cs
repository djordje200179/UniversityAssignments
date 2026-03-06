using Common.Models;
using CommunityToolkit.Mvvm.ComponentModel;
using System.ComponentModel;

namespace Common.Services;

public interface IAdminRepository : INotifyPropertyChanged {
	bool TryLogIn(string username, string password);
	void LogOut();
	bool ChangePassword(string currentPassword, string newPassword);

	Administrator? LoggedIn { get; }
}

public partial class AdminRepository : ObservableObject, IAdminRepository {
	private readonly Administrator[] administrators = [
		new("admin", "password123"),
	];

	[ObservableProperty]
	public partial Administrator? LoggedIn { get; private set; }


	public bool TryLogIn(string username, string password) {
		LoggedIn = administrators.FirstOrDefault(a => a.Username == username && a.Password == password);
		return LoggedIn is not null;
	}

	public void LogOut() => LoggedIn = null;

	public bool ChangePassword(string currentPassword, string newPassword) {
		if (LoggedIn is null || LoggedIn.Password != currentPassword)
			return false;

		LoggedIn.Password = newPassword;
		return true;
	}
}