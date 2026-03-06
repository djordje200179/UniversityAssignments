namespace Common.Models;

public record Report(int Id, string Description, int BicycleId, byte[]? Photography) {
}
