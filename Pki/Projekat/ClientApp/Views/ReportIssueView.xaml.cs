using ClientApp.ViewModels;

namespace ClientApp.Views;

public partial class ReportIssueView : ContentView {
	public ReportIssueView(ReportIssueViewModel vm) {
		InitializeComponent();
		BindingContext = vm;
	}
}