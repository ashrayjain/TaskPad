#include "stdafx.h"
#include "Storage_Datastore_stub.h"

using namespace std;

void Storage_Datastore_stub::emptyStore() {
	list<Task>::iterator it = _db.begin();

	while(it != _db.end()) {
		it->freeIndex();
		it++;
	}
	_db.clear();
}