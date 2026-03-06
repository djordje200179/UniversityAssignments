using Microsoft.Maui.Controls.Maps;

namespace ClientApp.Views.Map;

public abstract class CustomPin : Pin {
	public abstract ImageSource ImageSource { get; }
}

public class BicyclePin : CustomPin {
	public override ImageSource ImageSource => "bicycle.png";
}

public class ParkingPin : CustomPin {
	public override ImageSource ImageSource => "parking.png";
}