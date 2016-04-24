#include "RRDTP/LocalStore.h"
#include "RRDTP/Entry.h"
#include <cassert>

using namespace rrdtp;

Category::~Category()
{
	List<Category*>::Node* c = m_subcategories.Begin();
	while (c != NULL)
	{
		delete c->GetValue();

		c = c->GetNext();
	}

	m_subcategories.Clear();

	List<Entry*>::Node* e = m_entries.Begin();
	while (e != NULL)
	{
		delete e->GetValue();

		e = e->GetNext();
	}

	m_entries.Clear();
}

Category* Category::CreateSubcategory(const char* name)
{
	//Check for existing category first.
	Category* ret = GetSubcategory(name);

	//If it doesn't exist, then create it.
	if (ret == NULL)
	{
		ret = new Category(name);
		m_subcategories.Insert(ret);
	}

	return ret;
}

Category* Category::GetSubcategory(const char* name)
{
	List<Category*>::Node* n = m_subcategories.Begin();
	while (n != NULL)
	{
		if (strcmp(n->GetValue()->GetName(), name) == 0)
		{
			return n->GetValue();
		}

		n = n->GetNext();
	}

	return NULL;
}

Entry* Category::CreateEntry(HostID owner, const char* name, E_DATA_TYPE type)
{
	//Check for existing entry first.
	Entry* ret = GetEntry(name, type);

	//If it doesn't exist, then create it.
	if (ret == NULL)
	{
		ret = Entry::Create(name, type);
		m_entries.Insert(ret);
	}

	return ret;
}

Entry* Category::GetEntry(const char* name)
{
	List<Entry*>::Node* n = m_entries.Begin();
	while (n != NULL)
	{
		if (strcmp(n->GetValue()->GetName(), name) == 0)
		{
			return n->GetValue();
		}

		n = n->GetNext();
	}

	return NULL;
}

Entry* Category::GetEntry(const char* name, E_DATA_TYPE type)
{
	List<Entry*>::Node* n = m_entries.Begin();
	while (n != NULL)
	{
		if (strcmp(n->GetValue()->GetName(), name) == 0 && n->GetValue()->GetType() == type)
		{
			return n->GetValue();
		}

		n = n->GetNext();
	}

	return NULL;
}

Entry* LocalStore::Create(HostID owner, const char* identifier, E_DATA_TYPE type)
{
	if (identifier == NULL)
	{
		return NULL;
	}

	Category* category = &m_rootCategory;
	Entry* entry = NULL;

	//Create copy of string to tokenize.
	char* identifierCopy = _strdup(identifier);

	//Loop over tokens
	char* nextToken = NULL;
	char* token = strtok(identifierCopy, ".");
	while (token != NULL)
	{
		//The next token is retrieved first so that we know which one is the last one.
		nextToken = strtok(NULL, ".");

		//If we haven't reached the last token, then the current one is a category.
		if (nextToken != NULL)
		{
			category = category->CreateSubcategory(token);//Get or create the new category
		}
		//Otherwise we've reached the value and can create it.
		else
		{
			entry = category->CreateEntry(owner, token, type);
		}

		//Next token becomes current token for next loop iteration.
		token = nextToken;
	}

	return entry;
}

Entry* LocalStore::Get(const char* identifier)
{
	if (identifier == NULL)
	{
		return NULL;
	}

	Category* category = &m_rootCategory;
	Entry* entry = NULL;

	char* identifierCopy = _strdup(identifier);

	//Loop over tokens
	char* nextToken = NULL;
	char* token = strtok(identifierCopy, ".");
	while (token != NULL)
	{
		//The next token is retrieved first so that we know which one is the last one.
		nextToken = strtok(NULL, ".");

		//If we haven't reached the last token, then the current one is a category.
		if (nextToken != NULL)
		{
			category = category->GetSubcategory(token);
		}
		//Otherwise we've reached the value.
		else
		{
			entry = category->GetEntry(token);
		}

		//Next token becomes current token for next loop iteration.
		token = nextToken;
	}

	return entry;
}