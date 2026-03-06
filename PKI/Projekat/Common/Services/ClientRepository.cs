using Common.Models;

namespace Common.Services;

public interface IClientRepository {
	IEnumerable<Client> Fetch();
	Client Fetch(string username) => Fetch().First(c => c.Username == username);

	bool TryLogin(string username, string password);
	bool TryRegister(Client client);
	void Logout();

	Client? LoggedIn { get; }
	bool TryChangeUsername(string newUsername);
}

public class ClientRepository : IClientRepository {
	private readonly List<Client> clients = [
		new("John", "Doe", "123-456-7890", "johndoe@gmail.com", "johndoe", "password123"),
		new("Jane", "Smith", "987-654-3210", "janesmith@gmail.com", "janesmith", "nosecurity"),
	];

	public Client? LoggedIn { get; private set; }

	public IEnumerable<Client> Fetch() => clients;

	public bool TryLogin(string username, string password) {
		LoggedIn = clients.FirstOrDefault(c => c.Username == username && c.Password == password);
		return LoggedIn is not null;
	}
	public bool TryRegister(Client client) {
		if (clients.Any(c => c.Username == client.Username))
			return false;
		
		clients.Add(client);
		LoggedIn = client;
		return true;
	}

	public void Logout() => LoggedIn = null;

	public bool TryChangeUsername(string newUsername) {
		if (LoggedIn is null || clients.Any(c => c.Username == newUsername))
			return false;

		LoggedIn.Username = newUsername;
		return true;
	}
}
