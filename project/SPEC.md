# Project Specs

# States

DAY:                light on, heater as needed
NIGHT:              light off, heater as needed
FEED_PAUSE:         pump off so food is not sucked in
WATER_CHANGE:       heater and pump off while water is exchanged
ALARM:              water temp out of range

# Events

FEED:               switch pressed
WATER_CHANGE_SW:    turned on/off
DAYLIGHT:           switch turned on/off
ACK:                switch pressed
TEMP CHANGES:       if temp drops below 23 C, or rises above 28 C sound alarm
60 TICKS:           when 60 ticks pass end FEED_PAUSE

