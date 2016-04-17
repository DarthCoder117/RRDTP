#ifndef LocalStore_H
#define LocalStore_H
#include "RRDTP/Types.h"
#include "RRDTP/Sockets/Socket.h"
#include <list>
#include <cassert>

namespace rrdtp
{
	///@brief Class for managing local storage of data recieved over the network.
	class LocalStore
	{
	public:

		LocalStore()
			:m_rootCategory("root")
		{}

		///@brief Tracks a value entry in the local store.
		class Entry
		{
		public:

			HostID m_owner;

			const char* m_identifier;

			E_DATA_TYPE m_type;

			size_t m_size;
			size_t m_data;
		};

		///@brief Category of entries.
		class Category
		{
		public:

			///@brief Creates a new subcategory if one doesn't already exist.
			///@return The newly created category (or an already existing one).
			Category* CreateSubcategory(const char* name);
			///@brief Gets a subcategory by name.
			Category* GetSubcategory(const char* name);

			///@brief Creates a new entry if one doesn't already exist.
			///@return The newly created entry, or an existing one if compatible.
			///If an entry exists already, and the data types match, then the existing entry is returned.
			///If the data types don't match however, then NULL will be returned to indicate that the data type is wrong.
			Entry* CreateEntry(const char* name, E_DATA_TYPE type);
			///@brief Finds an entry inside this category.
			///@return If an entry is found and the data types match, then this will return the entry. 
			///Otherwise, if the data types don't match or no entry exists, then this will return NULL.
			Entry* GetEntry(const char* name, E_DATA_TYPE type);

		private:

			friend class LocalStore;

			Category(const char* name)
				:m_name(name)
			{}

			Category(const Category& oth)
			{}

			std::list<Category> m_subcategories;
			std::list<Entry> m_entries;

			const char* m_name;
		};

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