
#include <string>
#include <memory>

using namespace std;

class ICache {
public:
	virtual void* lookup(string key);
	virtual void store(string key, void* obj);
};

class _TzCacheDummy : public ICache {
public:
	void* lookup(string key) {
		return nullptr;
	}
	
	void store(string key, void* obj) {
	}

	void* tz_db() {
		return nullptr;
	}
};

class _TzCache : public ICache {
private:
	void* db = nullptr;
	bool dummy = false;

public:
	void* lookup(string key) {
		return nullptr;
	}
	void store(string key, void* obj) {
	}
};


//private:
//	int initialised = -1;
//	void* db = nullptr;
//	bool dummy = false;
//
//public:
//	get_db() {}
//
//	initialise() {}
//
//	lookup() {}
//
//	store() {}
//}

class _TzCacheManager {
private:
	static shared_ptr<ICache> tz_cache;

public:
	static ICache get_tz_cache() {

	}
	static void _initialise() {
		if (tz_cache == nullptr) {
			tz_cache = make_shared<_TzCache>();
		}
	}
};