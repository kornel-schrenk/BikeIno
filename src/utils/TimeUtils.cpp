#include "utils/TimeUtils.h"

String TimeUtils::_getTimezoneLocation()
{
	Preferences prefs;
	prefs.begin("M5ez", true); // read-only
	String savedTimezone = prefs.getString("timezone", "GeoIP");
	prefs.end();
	return savedTimezone;
}

bool TimeUtils::updateTimeFromNTPServer()
{
	updateNTP();
	if (timeStatus() == timeSet)
	{
		//Update timezone based on Preferences
		String storedTimezone = _getTimezoneLocation();
		Serial.println("Stored timezone: " + storedTimezone);
		ez.clock.tz.setLocation(storedTimezone);
		Serial.println("New timezone was set to " + storedTimezone);

		return true;
	}
	return false;
}

String TimeUtils::createUTCTime(int year, int month, int day, int hour, int minute, int second)
{
	//UTC (in ISO 8601), like this: <time>2018-06-30T05:44:13Z</time>
	return String(year, 10) + "-" + _zeroPad(month) + "-" + _zeroPad(day) + "T" + _zeroPad(hour) + ":" + _zeroPad(minute) + ":" + _zeroPad(second) + "Z";
}

String TimeUtils::_zeroPad(int number)
{
	return number < 10 ? "0" + String(number, 10) : String(number, 10);
}