#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdint.h>

template<typename T>
class IndirectionMap {
public:
	uint64_t Add(const T& newData, const std::string &name) {
		uint64_t newID = data.size();
		data.push_back(newData);
		names.insert(std::make_pair(name, newID));
		return newID;
	}

	uint64_t GetID(const std::string &name) {
		auto numElements = names.count(name);
		if (numElements > 0) {
			auto result = names.at(name);
			return result;
		} else {
			return ~(0UL);
		}
	}

	const T *GetPtr(uint64_t id) const {
		if (id < data.size())
			return &data.at(id);
		return nullptr;
	}

	const T& Get(uint64_t id) const {
		if (id < data.size())
			return data.at(id);
		return nullptr;
	}

	void Delete() {
		while (!data.empty()) {
			auto ptr = data.back();
			delete ptr;
			data.pop_back();
		}
	}
private:
protected:
	std::vector<T> data;
	std::map<std::string, uint64_t> names;
};