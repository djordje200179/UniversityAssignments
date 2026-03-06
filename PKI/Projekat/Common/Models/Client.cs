namespace Common.Models;

public class Client(string firstName, string lastName, string phoneNumber, string emailAddress, string username, string password) {
    public string FirstName { get; set; } = firstName;
    public string LastName { get; set; } = lastName;
    public string PhoneNumber { get; set; } = phoneNumber;
    public string EmailAddress { get; set; } = emailAddress;
    public string Username { get; set; } = username;
    public string Password { get; set; } = password;
}