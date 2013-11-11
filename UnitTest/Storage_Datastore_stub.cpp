#include "stdafx.h"
#include "Storage_Datastore_stub.h"

using namespace std;

void Storage_Datastore_stub::emptyStore() {
	Task::flushAllIndices();
	_db.clear();
}