#pragma once

namespace Kernel {
template <typename T>
class Singleton {
protected:
	Singleton() = default;
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static T& getInstance() {
		static T instance;
		return instance;
	}
};
}

#define SINGLETON_CLASS(Name) class Name : public Singleton<Name> { friend class Singleton<Name>;