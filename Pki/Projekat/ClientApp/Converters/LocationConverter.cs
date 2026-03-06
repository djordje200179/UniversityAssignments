using System.Globalization;

namespace ClientApp.Converters;

public class LocationConverter : IValueConverter {
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture) {
        if (value is not Common.Models.Location location)
            return null;

        return new Location(location.Latitude, location.Longitude);
	}

    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture) {
        if (value is not Location location)
            return null;

        return new Common.Models.Location(location.Latitude, location.Longitude);
	}
}
