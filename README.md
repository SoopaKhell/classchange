# classchange

This program will output how long (hours, minutes, seconds) until the next class / classchange, given a schedule. I made this for better time managment in highschool and college.

## Installation

Change directory to the cloned repo (`cd classchange`) and run `make install`.

## Usage

Edit `$HOME/.config/classchange/schedule.cfg` and add your own schedule. Here is an example `schedule.cfg`:

```txt
mon: 10:20:00-MATH,11:10:00-MATH,12:40:00-POLS,13:30:00-POLS,15:00:00-LING,15:50:00-LING
tue: 9:35:00-MATH,10:50:00-MATH,11:10:00-CSCI,12:25:00-CSCI,14:20:00-POLS,15:10:00-POLS,15:55:00-CSCI,17:10:00-CSCI
wed: 15:00:00-LING,15:50:00-LING
thu: 9:35:00-MATH,10:50:00-MATH,11:10:00-CSCI,12:25:00-CSCI,14:20:00-POLS,15:10:00-POLS,15:55:00-CSCI,17:10:00-CSCI
fri: 15:00:00-LING,15:50:00-LING
```

Run `classchange`. Example output:
```txt
LING (01:35:08)
```

Meaning: there is 1 hour, 35 minutes, and 8 seconds until the Linguistics class starts.
