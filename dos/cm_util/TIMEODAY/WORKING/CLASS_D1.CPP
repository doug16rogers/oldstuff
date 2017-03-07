#include "timeoday.h"


   char* TIME_OF_DAY::default_format = "%02h:%02m:%02s"; // 23:59:59

   char TIME_OF_DAY::local_image[0x80] = "";

   const UINT32 TIME_OF_DAY::milliseconds_per_second = 1000L;

   const UINT32 TIME_OF_DAY::seconds_per_minute = 60L;

   const UINT32 TIME_OF_DAY::minutes_per_hour = 60L;

   const UINT32 TIME_OF_DAY::hours_per_day = 24L;

   const UINT32 TIME_OF_DAY::minutes_per_day =
        TIME_OF_DAY::minutes_per_hour * TIME_OF_DAY::hours_per_day;

   const UINT32 TIME_OF_DAY::seconds_per_hour =
        TIME_OF_DAY::seconds_per_minute * TIME_OF_DAY::minutes_per_hour;

   const UINT32 TIME_OF_DAY::seconds_per_day =
        TIME_OF_DAY::seconds_per_minute * TIME_OF_DAY::minutes_per_day;

   const UINT32 TIME_OF_DAY::milliseconds_per_minute =
        TIME_OF_DAY::milliseconds_per_second *
        TIME_OF_DAY::seconds_per_minute;

   const UINT32 TIME_OF_DAY::milliseconds_per_hour =
        TIME_OF_DAY::milliseconds_per_second * TIME_OF_DAY::seconds_per_hour;

   const UINT32 TIME_OF_DAY::milliseconds_per_day =
        TIME_OF_DAY::milliseconds_per_second * TIME_OF_DAY::seconds_per_day;
