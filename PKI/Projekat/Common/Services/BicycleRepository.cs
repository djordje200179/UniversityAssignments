using Common.Models;

namespace Common.Services;

public interface IBicycleRepository {
	IEnumerable<Bicycle> Fetch();
	int NextId => Fetch().Select(bicycle => bicycle.Id).DefaultIfEmpty(0).Max() + 1;
	Bicycle Fetch(int id) => Fetch().First(x => x.Id == id);

	void Add(Bicycle bicycle);
}

public class BicycleRepository : IBicycleRepository {
	private readonly IList<Bicycle> bicycles = [
		new(1, Bicycle.Type.Road, 200, new(44.804541, 20.478452), Bicycle.Status.Free),
		new(2, Bicycle.Type.Mountain, 300, new(44.812345, 20.465789), Bicycle.Status.Rented),
		new(3, Bicycle.Type.Electric, 250, new(44.798765, 20.489123), Bicycle.Status.Maintenance),
		new(4, Bicycle.Type.Road, 400, new(44.810123, 20.470456), Bicycle.Status.Free),
		new(5, Bicycle.Type.Mountain, 500, new(44.805678, 20.482345), Bicycle.Status.Free),
	];

	public IEnumerable<Bicycle> Fetch() => bicycles;

	public void Add(Bicycle bicycle) => bicycles.Add(bicycle);
}
