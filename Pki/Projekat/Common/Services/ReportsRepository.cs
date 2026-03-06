using Common.Models;

namespace Common.Services;

public interface IReportsRepository {
	IEnumerable<Report> Fetch();
	int NextId => Fetch().Select(report => report.Id).DefaultIfEmpty(0).Max() + 1;

	void Add(Report report);
	void Remove(Report report);
}

public class ReportsRepository : IReportsRepository {
    private readonly IList<Report> reports = [
        new(1, "Пробушена гума", 1, null),
        new(2, "Заглављен ланац", 2, null),
        new(3, "Празна батерија", 1, null),
        new(4, "Пробушена гума", 2, null),
        new(5, "Огуљен сиц за седење", 1, null),
        new(6, "Издувана гума", 3, null),
	];

	public IEnumerable<Report> Fetch() => reports;

	public void Add(Report report) => reports.Add(report);

	public void Remove(Report report) => reports.Remove(report);
}
