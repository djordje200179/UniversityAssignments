using Common;
using Common.Models;
using System.Globalization;

namespace ClientApp.Converters; 

public class MapPinTitleConverter : IValueConverter {
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture) {
        if (value is not Bicycle bicycle)
            return null;

        return $"{bicycle.BType.ToReadableString()} ({bicycle.PricePerHour} РСД/h)";
	}

    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture) {
        throw new NotImplementedException();
    }
}