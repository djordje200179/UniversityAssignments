namespace Common.Models;

public record Administrator(string Username) {
    public string Password { get; set; }

    public Administrator(string Username, string Password) : this(Username) {
        this.Password = Password;
    }

    public override string ToString() => Username;
}
