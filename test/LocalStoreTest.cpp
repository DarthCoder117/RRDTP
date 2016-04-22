#include <RRDTP/LocalStore.h>
#include <cassert>

int main()
{
	rrdtp::LocalStore localStore;

	//Test integer storage
	rrdtp::Entry* entry = localStore.Create(-1, "test-int", rrdtp::EDT_INT);
	assert(entry != NULL);

	entry->Set<int>(123456789);
	assert(entry->Get<int>() == 123456789);

	return 0;
}