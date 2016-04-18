#include "RRDTP/LocalStore.h"
#include <cassert>

using namespace rrdtp;

Entry::Entry(HostID owner, const char* name, E_DATA_TYPE type)
	:m_owner(owner),
	m_name(name),
	m_type(type),
	m_size(0),
	m_data(0)
{}

Entry::~Entry()
{
	if (m_size > sizeof(size_t) && m_data != 0)
	{
		delete[](unsigned char*)m_data;
		m_data = 0;
	}
}

void Entry::SetString(const char* str)
{
	assert(str);

	size_t sz = strlen(str)*sizeof(char);
	Reallocate(sz);

	memcpy((void*)m_data, str, sz);
	m_size = sz;
}

void Entry::Reallocate(size_t sz)
{
	if (m_size < sz)
	{
		//Free memory if it has been allocated already
		if (m_size != 0 && m_data != 0)
		{
			delete[](unsigned char*)m_data;
			m_data = 0;
		}

		m_data = (size_t) new unsigned char[sz];
	}
}

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
		ret = new Entry(owner, name, type);
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

	//Create copy of string to tokenize.
	char* identifierCopy = strdup(identifier);

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
			//TODO: Create value...
		}

		//Next token becomes current token for next loop iteration.
		token = nextToken;
	}

	//TODO: parse identifier and create categories and entry.
}

Entry* LocalStore::Get(const char* identifier)
{
	//TODO: parse identifier and find entry.
}