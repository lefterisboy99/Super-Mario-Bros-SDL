#pragma once
#include <iostream>
#include <string>
/**
 * This has got to be the _worst_ language ever made...
 * Really, how hard is it to develop a metaprogramming feature that goes:
 * - Hey, I want to *map* this specific functionality to different scenarios
 * especially when you take into account that they _never_ did anything about
 * introspection???
 *
 * So we have to resort to writing this garbage, in order to avoid the strain on
 * our eyes when using std::optional, but printing structs? Are you crazy?!
 */

template <typename T>
struct Ok {
	Ok() = default;
	~Ok() = default;
	explicit Ok(const T &value) :value(value) {}

	T value;
};

template <typename T>
struct Err {
	Err() = default;
	~Err() = default;
	explicit Err(const T &value) :errorValue(value) {}

	T errorValue;
};

template <typename T, typename E = std::string>
class Result {
public:
	Result() = default;
	~Result() = default;

	Result(const Ok<T> &okay) :okayValue(okay), errValue{}, isGood(true) {}
	Result(const Err<E> &notOkay) :okayValue{}, errValue(notOkay), isGood(false) {}
	explicit Result(T value) :okayValue(value), errValue{}, isGood(false) {}

	bool good() const {return isGood; }

    T &unwrap() {
    	if (!isGood) {
    		throw std::runtime_error("Unwrap on a bad value!");
    	} else {
    		return okayValue.value;
    	}
    }

    T const &unwrap() const {
	    if (!isGood) {
	    	throw std::runtime_error("Unwrap on a bad value!");
	    } else {
	    	return okayValue.value;
	    }
    }

private:
	Ok<T> okayValue;
	Err<E> errValue;
	bool isGood;
};

template <typename T, typename E = std::string>
static inline Result<T, E> ok(const T &value) {
	Ok<T> okay(value);
	return okay;
}

static inline Err<std::string> err(const std::string &why) {
	Err<std::string> notOkay(why);
	return notOkay;
}