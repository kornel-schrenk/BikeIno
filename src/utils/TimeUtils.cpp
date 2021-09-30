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

String TimeUtils::setDateTime(int year, int month, int day, int hour, int minute, int second)
{
	//Set time in ezTime
	setTime(hour, minute, second, day, month, year);

	//Return with the current time
	return ez.clock.tz.dateTime("Y-m-d H:i:s");
}
