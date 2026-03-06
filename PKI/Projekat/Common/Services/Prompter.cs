namespace Common.Services;

public interface IPrompter {
	Task ShowAlertAsync(string title, string message);
	Task ShowMessageAsync(string title, string message);
}
