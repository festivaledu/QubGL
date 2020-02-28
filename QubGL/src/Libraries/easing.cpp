//
//  easing.cpp
//  QubGL
//
//  Copyright © 2020 Team FESTIVAL. All rights reserved.
//

#include <cmath>
#include <map>

#include "easing.h"

#ifndef PI
#define PI 3.1415926545
#endif

double easeInSine( double t, double b, double c, double d ) {
	return -c * cos(t / d * (PI / 2)) + c + b;
}

double easeOutSine( double t, double b, double c, double d ) {
	return c * sin(t / d * (PI / 2)) + b;
}

double easeInOutSine( double t, double b, double c, double d ) {
	return -c / 2 * (cos(PI*t / d) - 1) + b;
}


double easeInQuad( double t, double b, double c, double d ) {
	return c * (t /= d)*t + b;
}

double easeOutQuad( double t, double b, double c, double d ) { 
	return -c * (t /= d)*(t - 2) + b;
}

double easeInOutQuad( double t, double b, double c, double d ) {
	if ((t /= d / 2) < 1) return c / 2 * t*t + b;
	return -c / 2 * ((--t)*(t - 2) - 1) + b;
}


double easeInCubic( double t, double b, double c, double d ) {
	return c * (t /= d)*t*t + b;
}

double easeOutCubic( double t, double b, double c, double d ) {
	return c * ((t = t / d - 1)*t*t + 1) + b;
}

double easeInOutCubic( double t, double b, double c, double d ) {
	if ((t /= d / 2) < 1) return c / 2 * t*t*t + b;
	return c / 2 * ((t -= 2)*t*t + 2) + b;
}


double easeInQuart( double t, double b, double c, double d ) {
	return c * (t /= d)*t*t*t + b;
}

double easeOutQuart( double t, double b, double c, double d) {
	return -c * ((t = t / d - 1)*t*t*t - 1) + b;
}

double easeInOutQuart( double t, double b, double c, double d ) {
	if ((t /= d / 2) < 1) return c / 2 * t*t*t*t + b;
	return -c / 2 * ((t -= 2)*t*t*t - 2) + b;
}


double easeInQuint( double t, double b, double c, double d ) {
	return c * (t /= d)*t*t*t*t + b;
}

double easeOutQuint( double t, double b, double c, double d ) {
	return c * ((t = t / d - 1)*t*t*t*t + 1) + b;
}

double easeInOutQuint( double t, double b, double c, double d ) {
	if ((t /= d / 2) < 1) return c / 2 * t*t*t*t*t + b;
	return c / 2 * ((t -= 2)*t*t*t*t + 2) + b;
}


double easeInExpo( double t, double b, double c, double d ) {
	return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}

double easeOutExpo( double t, double b, double c, double d ) {
	return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}

double easeInOutExpo( double t, double b, double c, double d ) {
	if (t == 0) return b;
	if (t == d) return b + c;
	if ((t /= d / 2) < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;
	return c / 2 * (-pow(2, -10 * --t) + 2) + b;
}


double easeInCirc( double t, double b, double c, double d ) {
	return -c * (sqrt(1 - (t /= d)*t) - 1) + b;
}

double easeOutCirc( double t, double b, double c, double d ) {
	return c * sqrt(1 - (t = t / d - 1)*t) + b;
}

double easeInOutCirc( double t, double b, double c, double d ) {
	if ((t /= d / 2) < 1) return -c / 2 * (sqrt(1 - t * t) - 1) + b;
	return c / 2 * (sqrt(1 - (t -= 2)*t) + 1) + b;
}


double easeInBack( double t, double b, double c, double d, double s ) {
	return c * (t /= d)*t*((s + 1)*t - s) + b;
}

double easeOutBack( double t, double b, double c, double d, double s ) {
	return c * ((t = t / d - 1)*t*((s + 1)*t + s) + 1) + b;
}

double easeInOutBack( double t, double b, double c, double d, double s ) {
	if ((t /= d / 2) < 1) return c / 2 * (t*t*(((s *= (1.525)) + 1)*t - s)) + b;
	return c / 2 * ((t -= 2)*t*(((s *= (1.525)) + 1)*t + s) + 2) + b;
}


double easeInElastic( double t, double b, double c, double d ) {
	auto s = 1.70158; auto p = 0; auto a = c;
	if (t == 0) return b;  if ((t /= d) == 1) return b + c;  if (!p) p = d * .3;
	if (a < abs(c)) { a = c; auto s = p / 4; }
	else auto s = p / (2 * PI) * asin(c / a);
	return -(a*pow(2, 10 * (t -= 1)) * sin((t*d - s)*(2 * PI) / p)) + b;
}

double easeOutElastic( double t, double b, double c, double d ) {
	auto s = 1.70158; auto p = 0; auto a = c;
	if (t == 0) return b;  if ((t /= d) == 1) return b + c;  if (!p) p = d * .3;
	if (a < abs(c)) { a = c; auto s = p / 4; }
	else auto s = p / (2 * PI) * asin(c / a);
	return a * pow(2, -10 * t) * sin((t*d - s)*(2 * PI) / p) + c + b;
}

double easeInOutElastic( double t, double b, double c, double d ) {
	auto s = 1.70158; auto p = 0; auto a = c;
	if (t == 0) return b;  if ((t /= d / 2) == 2) return b + c;  if (!p) p = d * (.3*1.5);
	if (a < abs(c)) { a = c; auto s = p / 4; }
	else auto s = p / (2 * PI) * asin(c / a);
	if (t < 1) return -.5*(a*pow(2, 10 * (t -= 1)) * sin((t*d - s)*(2 * PI) / p)) + b;
	return a * pow(2, -10 * (t -= 1)) * sin((t*d - s)*(2 * PI) / p)*.5 + c + b;
}


double easeInBounce( double t, double b, double c, double d ) {
	return c - easeOutBounce( d - t, 0, c, d) + b;
}

double easeOutBounce( double t, double b, double c, double d ) {
	if ((t /= d) < (1 / 2.75)) {
		return c * (7.5625*t*t) + b;
	}
	else if (t < (2 / 2.75)) {
		return c * (7.5625*(t -= (1.5 / 2.75))*t + .75) + b;
	}
	else if (t < (2.5 / 2.75)) {
		return c * (7.5625*(t -= (2.25 / 2.75))*t + .9375) + b;
	}
	else {
		return c * (7.5625*(t -= (2.625 / 2.75))*t + .984375) + b;
	}
}

double easeInOutBounce( double t, double b, double c, double d ) {
	if (t < d / 2) return easeInBounce(t * 2, 0, c, d) * .5 + b;
	return easeOutBounce(t * 2 - d, 0, c, d) * .5 + c * .5 + b;
}
