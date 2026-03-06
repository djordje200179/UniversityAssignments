namespace Common.Models;

public record Bicycle(
	int Id, Bicycle.Type BType, double PricePerHour,
	Location CurrentLocation,
	Bicycle.Status CurrentStatus = Bicycle.Status.Free
) {
	public Type BType { get; set; } = BType;
	public double PricePerHour { get; set; } = PricePerHour;
	public Status CurrentStatus { get; set; } = CurrentStatus;

	public enum Status {
		Free,
		Rented,
		Maintenance
	}

	public enum Type {
		Mountain,
		Road,
		Hybrid,
		Electric
	}
}
