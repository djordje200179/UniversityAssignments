using CommunityToolkit.Mvvm.ComponentModel;

namespace ClientApp.Behaviors;

public class PageActivitySetterBehavior : Behavior<ContentPage> {
	public static readonly BindableProperty AttachBehaviorProperty = BindableProperty.CreateAttached(
		"AttachBehavior", typeof(bool), typeof(PageActivitySetterBehavior),
		defaultValue: false, propertyChanged: OnAttachBehaviorChanged
	);

	public static bool GetAttachBehavior(BindableObject view) => (bool)view.GetValue(AttachBehaviorProperty);

	public static void SetAttachBehavior(BindableObject view, bool value) => view.SetValue(AttachBehaviorProperty, value);

	private static void OnAttachBehaviorChanged(BindableObject bindable, object oldValue, object newValue) {
		if (bindable is not ContentPage page)
			return;

		if ((bool)newValue)
			page.Behaviors.Add(new PageActivitySetterBehavior());
		else
			page.Behaviors.Remove(page.Behaviors.FirstOrDefault(b => b is PageActivitySetterBehavior));
	}

	protected override void OnAttachedTo(ContentPage bindable) {
		base.OnAttachedTo(bindable);

		bindable.Appearing += OnAppearing;
		bindable.Disappearing += OnDisappearing;
	}

	protected override void OnDetachingFrom(ContentPage bindable) {
		base.OnDetachingFrom(bindable);

		bindable.Appearing -= OnAppearing;
		bindable.Disappearing -= OnDisappearing;
	}

	private void OnAppearing(object? sender, EventArgs e) {
		if (sender is not ContentPage page) 
			return;

		if (page.BindingContext is ObservableRecipient vm)
			vm.IsActive = true;
	}

	private void OnDisappearing(object? sender, EventArgs e) {
		if (sender is not ContentPage page)
			return;

		if (page.BindingContext is ObservableRecipient vm)
			vm.IsActive = false;
	}
}
