# PandReaker 
## Author: [Raphael Dray](https://www.linkedin.com/in/raphaeldray/)

OpenMP Optimized Multi-Threaded Password Breaker.
It can be run on Linux/Windows but not for OSX since _crypt_r(3)_
is unavailable and _crypt(3)_ is not thread-safe.

Moreover, _cancellation point_ is unavailable for OSX.

## Usage:
_Tested on **Intel(R) Core(TM) i7-10510U CPU @ 1.80GHz   2.30 GHz**_
```shell
[OMP_NUM_THREADS=8] OMP_CANCELLATION=true ./PandReaker [<password> <first_char> <last_char>]
*running parameters*
 -password length:	4 digits
 -digits:		from - - to -~-
 -crypted to break:	saPriAzzLlrOs
max_iter = 78074896 
password found: A$4c
time: 0min 4.554s 
```