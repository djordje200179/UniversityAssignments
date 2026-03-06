using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Messaging;

namespace ClientApp.ViewModels;

public class BaseViewModel(IMessenger messenger) : ObservableRecipient(messenger) {
	protected virtual void OnViewAppeared() { }
	protected virtual void OnViewDisappeared() { }

	protected sealed override void OnActivated() {
		base.OnActivated();
		OnViewAppeared();
	}

	protected sealed override void OnDeactivated() {
		base.OnDeactivated();
		OnViewDisappeared();
	}
}
