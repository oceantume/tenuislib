/*
MIT License

Copyright (c) 2016 Gabriel Bourgeois

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef HEADER_TENUIS_OPTIONAL_HPP_
#define HEADER_TENUIS_OPTIONAL_HPP_

#include <cassert>
#include <type_traits>


namespace tenuis {

struct nullopt_t {
	constexpr nullopt_t(int) {}
};

constexpr nullopt_t nullopt{ 0 };


template<class T, class TError = nullopt_t>
class optional
{
	static_assert(!std::is_same<T, TError>::value, "Using the same type for both Value and Error is not supported.");
	static_assert(sizeof(T) > 0 || sizeof(TError), "Storage size must be greated than zero (required by std::aligned_union).");

public:
	//
	// from optional

	optional(const optional& rhs) {
		assign(rhs);
	}

	optional(optional&& rhs) {
		assign(std::move(rhs));
	}

	optional& operator=(const optional& rhs) {
		destroy();
		assign(rhs);
		return *this;
	}

	optional& operator=(optional&& rhs) {
		destroy();
		assign(std::move(rhs));
		return *this;
	}


	//
	// from T

	optional() : optional(T()) {}

	optional(const T& rhs) {
		assign(rhs);
	}

	optional(T&& rhs) {
		assign(std::move(rhs));
	}

	optional& operator=(const T& rhs) {
		destroy();
		assign(rhs);
		return *this;
	}

	optional& operator=(T&& rhs) {
		destroy();
		assign(std::move(rhs));
		return *this;
	}

	//
	// from TError

	optional(const TError& rhs) {
		assign(rhs);
	}

	optional(TError&& rhs) {
		assign(std::move(rhs));
	}

	optional& operator=(const TError& rhs) {
		destroy();
		assign(rhs);
		return *this;
	}

	optional& operator=(TError&& rhs) {
		destroy();
		assign(std::move(rhs));
		return *this;
	}

	~optional() {
		destroy();
	}


	//
	// value getters

	bool has_value() const {
		return has_value_;
	}

	bool has_error() const {
		return !has_value_;
	}

	T& value() {
		assert(has_value());
		return reinterpret_cast<T&>(storage_);
	}

	const T& value() const {
		assert(has_value());
		return reinterpret_cast<const T&>(storage_);
	}

	TError& error() {
		assert(has_error());
		return reinterpret_cast<TError&>(storage_);
	}

	const TError& error() const {
		assert(has_error());
		return reinterpret_cast<const TError&>(storage_);
	}


	//
	// utility

	operator bool() const {
		return has_value();
	}

	T* operator->() {
		return &value();
	}

	const T* operator->() const {
		return &value();
	}

	T& operator*() {
		return value();
	}

	const T& operator*() const {
		return value();
	}

private:
	void assign(const optional& rhs) {
		has_value_ = rhs.has_value_;
		if (has_value_) ::new (std::addressof(storage_)) T(rhs.value());
		else            ::new (std::addressof(storage_)) TError(rhs.error());
	}

	void assign(optional&& rhs) {
		has_value_ = rhs.has_value_;
		if (has_value_) ::new (std::addressof(storage_)) T(std::move(rhs.value()));
		else            ::new (std::addressof(storage_)) TError(std::move(rhs.error()));
	}

	void assign(const T& rhs) {
		has_value_ = true;
		::new (std::addressof(storage_)) T(rhs);
	}

	void assign(T&& rhs) {
		has_value_ = true;
		::new (std::addressof(storage_)) T(std::move(rhs));
	}

	void assign(const TError& rhs) {
		has_value_ = false;
		::new (std::addressof(storage_)) TError(rhs);
	}

	void assign(TError&& rhs) {
		has_value_ = false;
		::new (std::addressof(storage_)) TError(std::move(rhs));
	}

	void destroy() {
		if (has_value_) value().~T();
		else            error().~TError();
	}

	using storage_type = typename std::aligned_union<0, T, TError>::type;
	

	bool has_value_;
	storage_type storage_;
};

}

#endif