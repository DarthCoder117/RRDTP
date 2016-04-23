#include "RRDTP/LocalStore.h"
#include "RRDTP/Entry.h"
#include <cassert>

using namespace rrdtp;

Category::~Category()
{
	for (std::list<Category*>::iterator iter = m_subcategories.begin(); iter != m_subcategories.end(); ++iter)
	{
		delete *iter;
	}

	m_subcategories.clear();

	for (std::list<Entry*>::iterator iter = m_entries.begin(); iter != m_entries.end(); ++iter)
	{
		delete *iter;
	}

	m_subcategories.clear();
}

Category* Category::CreateSubcategory(const char* name)
{
	//Check for existing category first.
	Category* ret = GetSubcategory(name);

	//If it doesn't exist, then create it.
	if (ret == NULL)
	{
		ret = new Category(name);
		m_subcategories.push_back(ret);
	}

	return ret;
}

Category* Category::GetSubcategory(const char* name)
{
	std::list<Category*>::iterator iter;
	for (iter = m_subcategories.begin(); iter != m_subcategories.end(); ++iter)
	{
		if (strcmp((*iter)->m_name, name) == 0)
		{
			return *iter;
		}
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
		m_entries.push_back(ret);
	}

	return ret;
}

Entry* Category::GetEntry(const char* name)
{
	std::list<Entry*>::iterator iter;
	for (iter = m_entries.begin(); iter != m_entries.end(); ++iter)
	{
		if (strcmp((*iter)->GetName(), name) == 0)
		{
			return *iter;
		}
	}

	return NULL;
}

Entry* Category::GetEntry(const char* name, E_DATA_TYPE type)
{
	std::list<Entry*>::iterator iter;
	for (iter = m_entries.begin(); iter != m_entries.end(); ++iter)
	{
		if (strcmp((*iter)->GetName(), name) == 0 && (*iter)->GetType() == type)
		{
			return *iter;
		}
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