namespace Common.Models;

public class Ride(int id, string clientUsername, int bicycleId, double pricePerHour, DateTime startTime, DateTime? endTime = null, byte[]? photo = null) {
    public int Id { get; } = id;
    public string ClientUsername { get; set; } = clientUsername;
    public int BicycleId { get; } = bicycleId;
	public double PricePerHour { get; } = pricePerHour;
    public DateTime StartTime { get; } = startTime;
    public DateTime? EndTime { get; set; } = endTime;
    public byte[]? Photo { get; set; } = photo;

	public double TotalPrice => ((EndTime ?? DateTime.Now) - StartTime).TotalHours * PricePerHour;
}
