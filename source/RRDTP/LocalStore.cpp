#include "RRDTP/LocalStore.h"

using namespace rrdtp;

DataEntry* LocalStore::FindOrCreate(const char* identifier, E_DATA_TYPE type)
{
	CategoryEntry* e = &m_rootEntry;
	DataEntry* result = NULL;

	//Tokenize string and search through the tree until the identifier is found.
	char* identifierCpy = _strdup(identifier);
	char* pch = strtok(identifierCpy, ".");
	while (pch != NULL)//For each token
	{
		char* currentPch = pch;
		pch = strtok(NULL, ".");

		//Try and find the child with that name.
		Entry* child = e->FindChild(currentPch);
		if (child != 0)
		{
			//Traverse into subcategory if needed.
			if (child->IsCategory())
			{
				e = (CategoryEntry*)child;
			}
			else
			{
				result = (DataEntry*)child;
			}
		}
		//If the child doesn't exist, then create it
		else
		{
			if (pch == NULL)//If pch is NULL then that means we've reached the end of the categories and can add the data entry.
			{
				result = new DataEntry(currentPch, type);
				if (!e->Add(result))
				{
					return NULL;
				}
			}
			else//Otherwise another category will be needed.
			{
				CategoryEntry* newCategory = new CategoryEntry(currentPch);
				e->Add(newCategory);
				e = newCategory;
			}
		}
	}

	return result;
}