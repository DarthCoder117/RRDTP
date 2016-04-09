#include <RRDTP/LocalStore.h>
#include <cassert>

int main()
{
	rrdtp::LocalStore localStore;

	//Test get/set 32-bit integer value with no category specified.
	localStore.SetInt("test-val", 12345);
	assert(localStore.GetInt("test-val") == 12345);

	//Test get/set 32-bit integer value with category specified.
	localStore.SetInt("category1.test-val", 54321);
	assert(localStore.GetInt("category1.test-val") == 54321);

	return 0;
}