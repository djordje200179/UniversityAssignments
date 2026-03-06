using Common.Models;

namespace Common.Services;

public interface IParkingRepository {
	IEnumerable<Parking> Fetch();
}

public class ParkingRepository : IParkingRepository {
    private readonly Parking[] parkings = [
        new("Славија", new(44.804541, 20.478452)),
        new("Трг Републике", new(44.812345, 20.465789)),
        new("Калемегдан", new(44.798765, 20.489123)),
	];

	public IEnumerable<Parking> Fetch() => parkings;
}
