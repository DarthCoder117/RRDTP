/*==============================================================================
The MIT License (MIT)

Copyright (c) 2016 Tanner Mickelson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
==============================================================================*/
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

Entry* Category::CreateEntry(HostID owner, const char* identifier, E_DATA_TYPE type)
{
	//Check for existing entry first.
	Entry* ret = GetEntryByIdentifier(identifier);
	if (ret != NULL && ret->GetType() != type)
	{
		return NULL;
	}

	//If it doesn't exist, then create it.
	if (ret == NULL)
	{
		ret = Entry::Create(identifier, type);
		m_entries.Insert(ret);
	}

	return ret;
}

Entry* Category::GetEntryByName(const char* name)
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

Entry* Category::GetEntryByIdentifier(const char* identifier)
{
	List<Entry*>::Node* n = m_entries.Begin();
	while (n != NULL)
	{
		if (strcmp(n->GetValue()->GetIdentifier(), identifier) == 0)
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
	int identifierLen = strlen(identifier);
	char* identifierCopy = new char[identifierLen];
	memcpy(identifierCopy, identifier, identifierLen);

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
			entry = category->CreateEntry(owner, identifier, type);
		}

		//Next token becomes current token for next loop iteration.
		token = nextToken;
	}

	delete[] identifierCopy;

	return entry;
}

bool LocalStore::Delete(const char* identifier)
{
	if (identifier == NULL)
	{
		return false;
	}

	Category* category = &m_rootCategory;
	Entry* entry = NULL;

	int identifierLen = strlen(identifier);
	char* identifierCopy = new char[identifierLen];
	memcpy(identifierCopy, identifier, identifierLen);

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
			//Iterate through and delete
			List<Entry*>& entries = category->GetEntries();
			List<Entry*>::Node* n = entries.Begin();
			while (n != NULL)
			{
				if (n->GetValue()->GetName() == token)
				{
					delete n->GetValue();
					entries.Erase(n);

					delete[] identifierCopy;

					return true;
				}

				n = n->GetNext();
			}
		}

		//Next token becomes current token for next loop iteration.
		token = nextToken;
	}

	delete[] identifierCopy;

	return false;
}

Entry* LocalStore::Get(const char* identifier)
{
	if (identifier == NULL)
	{
		return NULL;
	}

	Category* category = &m_rootCategory;
	Entry* entry = NULL;

	int identifierLen = strlen(identifier);
	char* identifierCopy = new char[identifierLen];
	memcpy(identifierCopy, identifier, identifierLen);

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
			entry = category->GetEntryByName(token);
		}

		//Next token becomes current token for next loop iteration.
		token = nextToken;
	}

	delete[] identifierCopy;

	return entry;
}