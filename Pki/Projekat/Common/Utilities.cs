using Common.Models;
using Common.Services;
using Microsoft.Extensions.DependencyInjection;

namespace Common;

public static class Utilities {
	extension(IServiceCollection sc) {
        public IServiceCollection AddCommonServices() => sc
            .AddSingleton<IClientRepository, ClientRepository>()
            .AddSingleton<IRideRepository, RideRepository>()
			.AddSingleton<IAdminRepository, AdminRepository>()
            .AddSingleton<IBicycleRepository, BicycleRepository>()
            .AddSingleton<IParkingRepository, ParkingRepository>()
            .AddSingleton<IReportsRepository, ReportsRepository>();
	}

    extension(Bicycle.Type type) {
        public string ToReadableString() => type switch {
            Bicycle.Type.Mountain => "Брдско",
            Bicycle.Type.Hybrid => "Хибридно",
            Bicycle.Type.Road => "Друмско",
            Bicycle.Type.Electric => "Електрично",
            _ => "Непознато"
        };
    }

    extension(Bicycle.Status status) {
        public string ToReadableString() => status switch {
            Bicycle.Status.Free => "Слободно",
            Bicycle.Status.Rented => "Изнајмљено",
			Bicycle.Status.Maintenance => "На одржавању",
			_ => "Непознато"
        };
	}
}