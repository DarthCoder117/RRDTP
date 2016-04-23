#include <RRDTP/LocalStore.h>
#include <RRDTP/Entry.h>
#include <cassert>

int main()
{
	rrdtp::LocalStore localStore;

	//Test integer storage
	/*rrdtp::Entry* entry = localStore.Create(-1, "test-int", rrdtp::EDT_INT);
	assert(entry != NULL);

	entry->Set<int>(123456789);
	assert(entry->Get<int>() == 123456789);

	//Test setting values directly
	int testInt = 8726348;
	entry->Set((unsigned char*)&testInt, sizeof(int));
	assert(entry->Get<int>() == 8726348);

	//Test setting strings
	entry->SetString("A B C D E F G H I J K L M N O P Q R S T U V W X Y Z");
	assert(strcmp(entry->GetString(), "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z") == 0);*/

	return 0;
}