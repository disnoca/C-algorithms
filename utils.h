#pragma once


#define MIN(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define MAX(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

#define DIV_CEIL(x,y) 	({__typeof__ (x) _x = (x); __typeof__ (y) _y = (y); ((_x / _y) + !!(_x % _y)); })
#define ALIGN_UP(n,m)	({__typeof__ (n) _n = (n); __typeof__ (m) _m = (m); _n % _m == 0 ? _n : (_n + (_m - (_n % _m))); })	
#define ALIGN_DOWN(n,m)	({__typeof__ (n) _n = (n); (_n - (_n % (m))); })
