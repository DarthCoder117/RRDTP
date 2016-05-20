/*==============================================================================
The MIT License (MIT)

Copyright (c) 2016 Tanner Mickelson & The RRDTP Team

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
#ifndef LocalStore_H
#define LocalStore_H
#include "RRDTP/Types.h"
#include "RRDTP/Sockets/Socket.h"
#include "RRDTP/List.h"
#include <list>
#include <cassert>

namespace rrdtp
{
	class Entry;

	///@brief Category of entries.
	class Category
	{
	public:

		~Category();

		///@brief Creates a new subcategory if one doesn't already exist.
		///@return The newly created category (or an already existing one).
		Category* CreateSubcategory(const char* name);
		///@brief Gets a subcategory by name.
		Category* GetSubcategory(const char* name);

		///@return A reference to the list of subcategories.
		List<Category*>& GetSubcategories()
		{
			return m_subcategories;
		}

		///@return A reference to the list of entries.
		List<Entry*>& GetEntries()
		{
			return m_entries;
		}

		///@brief Creates a new entry if one doesn't already exist.
		///@param owner When creating an entry, the host ID of the creator needs to be specified so that the server can track who owns what entry.
		///@param name The name of the entry.
		///@param type The data type stored in the entry. Future updates to the entry MUST use a consistent data type.
		///@return The newly created entry, or an existing one if compatible.
		///If an entry exists already, and the data types match, then the existing entry is returned.
		///If the data types don't match however, then NULL will be returned to indicate that the data type is wrong.
		Entry* CreateEntry(HostID owner, const char* name, E_DATA_TYPE type);

		///@brief Finds an entry inside this category by name regardless of type.
		///@return The entry if it was found. NULL otherwise.
		Entry* GetEntryByName(const char* name);
		///@brief Finds an entry inside this category by identifier regardless of type.
		///@return The entry if it was found. NULL otherwise.
		Entry* GetEntryByIdentifier(const char* identifier);

		const char* GetName()
		{
			return m_name;
		}

	private:

		friend class LocalStore;

		Category(const char* name);

		Category(const Category& oth)
		{}

		List<Category*> m_subcategories;
		List<Entry*> m_entries;

		char* m_name;
	};

	///@brief Class for managing local storage of data recieved over the network.
	class LocalStore
	{
	public:

		LocalStore()
			:m_rootCategory("root")
		{}

		///@brief Creates an entry and all required categories in the identifier.
		///@param owner The host that owns this entry.
		///@param identifier The full value identifier of the entry to create.
		///@param type The data type of the entry.
		///@return Returns a pointer to the newly created entry, or returns NULL if an error ocurred
		///(see the documentation for Category for more detail on possible errors).
		Entry* Create(HostID owner, const char* identifier, E_DATA_TYPE type);

		///@brief Gets an entry.
		///@param identifier The full value identifier of the entry to search for.
		Entry* Get(const char* identifier);

		///@brief Gets an entry.
		///@param identifier The full value identifier of the entry to search for.
		///@return True if the value entry was deleted, false if it didn't exist.
		bool Delete(const char* identifier);

		///@return The root category that all the other categories are stored in.
		Category* GetRootCategory()
		{
			return &m_rootCategory;
		}

	private:

		Category m_rootCategory;
	};
}

#endif