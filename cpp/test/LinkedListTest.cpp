#include <RRDTP/List.h>
#include <cassert>

int main()
{
	rrdtp::List<int> list;

	list.Insert(1);
	list.Insert(2);
	list.Insert(3);

	rrdtp::List<int>::Node* n = list.Begin();
	assert(n->GetValue() == 3);
	n = n->GetNext();
	assert(n->GetValue() == 2);
	n = n->GetNext();
	assert(n->GetValue() == 1);

	assert(list.Size() == 3);

	n = n->GetNext();
	assert(n == NULL);

	n = list.Begin();
	n = list.Erase(n->GetNext());
	assert(n->GetValue() == 1);

	assert(list.Size() == 2);

	return 0;
}