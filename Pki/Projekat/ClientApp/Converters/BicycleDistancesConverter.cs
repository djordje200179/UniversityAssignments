using System.Globalization;

namespace ClientApp.Converters;

public class BicycleDistancesConverter : IValueConverter {
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture) {
        if (value is not IDictionary<string, double> distances)
            return null;

        return string.Join("\n", distances.Select(kv => $"{kv.Key}: {kv.Value:F2} km"));
	}

    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture) {
        throw new NotImplementedException();
    }
}
