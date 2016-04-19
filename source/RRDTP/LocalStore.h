#ifndef LocalStore_H
#define LocalStore_H
#include "RRDTP/Types.h"
#include "RRDTP/Sockets/Socket.h"
#include <list>
#include <cassert>

namespace rrdtp
{
	///@brief Tracks a value entry in the local store.
	class Entry
	{
	public:

		Entry(HostID owner, const char* name, E_DATA_TYPE type);

		~Entry();

		HostID GetOwner(){return m_owner;}

		const char* GetName(){return m_name;}

		E_DATA_TYPE GetType(){return m_type;}

		///@brief Set the entry to a value (fixed size types)
		template <typename T>
		void Set(const T& val)
		{
			if (sizeof(T) <= sizeof(size_t))//Small data
			{
				*((T*)&m_data) = val;
			}
			else//Large data
			{
				Reallocate(sizeof(T));

				((T*)m_data) = val;
			}

			m_size = sizeof(T);
		}

		///@brief Set the entry to a string value.
		void SetString(const char* str);

		///@brief Set the entry to a value (fixed size types)
		template <typename T>
		T Get()
		{
			//TODO: Implement this...
		}

	private:

		Entry(const Entry& oth) {}

		///@brief Reallocates memory if needed.
		void Reallocate(size_t sz);

		HostID m_owner;

		const char* m_name;

		E_DATA_TYPE m_type;

		size_t m_size;
		size_t m_data;///< Stores either the data itself if it will fit, or a pointer to the data.
	};

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
		Entry* GetEntry(const char* name);
		///@brief Finds an entry inside this category with a matching type.
		///@return If an entry is found and the data types match, then this will return the entry. 
		///Otherwise, if the data types don't match or no entry exists, then this will return NULL.
		Entry* GetEntry(const char* name, E_DATA_TYPE type);

		const char* GetName()
		{
			return m_name;
		}

	private:

		friend class LocalStore;

		Category(const char* name)
			:m_name(name)
		{}

		Category(const Category& oth)
		{}

		std::list<Category*> m_subcategories;
		std::list<Entry*> m_entries;

		const char* m_name;
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