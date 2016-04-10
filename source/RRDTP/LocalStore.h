#ifndef LocalStore_H
#define LocalStore_H
#include "RRDTP/Types.h"
#include <vector>
#include <cassert>

namespace rrdtp
{
	///@brief Entry is used by the local store to implement categories.
	class Entry
	{
	public:

		Entry(const char* name)
			:m_name(name)
		{}

		///@return This entry's name.
		const char* GetName() { return m_name; }

		///@return Whether this entry is a category, or data.
		virtual bool IsCategory() = 0;

	protected:

		const char* m_name;
	};

	///@brief A category entry stores subcategories and data. Uncategorized data is stored in the root category entry.
	class CategoryEntry : public Entry
	{
	public:

		CategoryEntry(const char* name)
			:Entry(name)
		{}

		///@brief Find a child entry to this one by name.
		Entry* FindChild(const char* name)
		{
			for (unsigned int i = 0; i < m_children.size(); ++i)
			{
				if (strcmp(name, m_children[i]->GetName()) == 0)
				{
					return m_children[i];
				}
			}

			return NULL;
		}

		///@brief Adds a child to this category.
		///@return True if successfull, false if a child with the same name already exists.
		bool Add(Entry* child)
		{
			for (unsigned int i = 0; i < m_children.size(); ++i)
			{
				if (strcmp(child->GetName(), m_children[i]->GetName()) == 0)
				{
					return false;
				}
			}

			m_children.push_back(child);
			return true;
		}

		///@return The number of child entries belonging to this one.
		unsigned int GetChildCount() { return m_children.size(); }

		bool IsCategory() { return true; }

	private:

		std::vector<Entry*> m_children;
	};

	///@brief DataEntry is used by the LocalStore to identify and store data.
	class DataEntry : public Entry
	{
	public:

		DataEntry(const char* name, E_DATA_TYPE type)
			:Entry(name),
			m_type(type),
			m_data(0),
			m_dataSz(0)
		{}

		~DataEntry()
		{
			//Free up dynamically allocated data.
			if (m_dataSz > sizeof(size_t))
			{
				delete[](char*)m_data;
			}
		}

		///@brief Set the value stored in this object.
		///@warning This is not for use with strings. To set a string use SetString instead.
		template <typename T>
		void Set(const T& val)
		{
			//Data larger than the native processor architecture's length has to be dynamically allocated.
			if (m_dataSz > sizeof(size_t))
			{
				if (m_data == 0)//No memory has been allocated yet if m_data == 0.
				{
					m_dataSz = sizeof(T);
					m_data = new char[sizeof(T)];
				}

				//Check that sizes match and then copy.
				assert(m_dataSz == sizeof(T));
				*((T*)m_data) = val;
			}
			//Small data just goes right into the data field.
			else
			{
				T* ptr = (T*)&m_data;
				*ptr = val;
			}
		}

		///@brief Get the value stored in this object.
		///@warning This is not for use with strings. To get a string use GetString instead.
		template <typename T>
		T Get()
		{
			if (m_dataSz > sizeof(size_t))
			{
				return *((T*)m_data);
			}
			else
			{
				T* ptr = (T*)&m_data;
				return *ptr;
			}
		}

		///@return The data type of this value.
		E_DATA_TYPE GetType() { return m_type; }

		///@return The size of the stored data in bytes.
		size_t GetDataSize() { return m_dataSz; }

		bool IsCategory() { return false; }

	private:

		E_DATA_TYPE m_type;

		size_t m_data;
		size_t m_dataSz;
	};

	///@brief Class for managing local storage of data recieved over the network.
	class LocalStore
	{
	public:

		LocalStore()
			:m_rootEntry("root")
		{}

		CategoryEntry& GetRootEntry() { return m_rootEntry; }

		///@brief Finds a data entry based on its identifier, or creates one if it doesn't exist.
		///@param identifier A valid value identifier.
		///@return The entry with the specified identifier, or NULL if an error occurred (invalid path, trying to add children to values instead of categories, etc.)
		DataEntry* FindOrCreate(const char* identifier, E_DATA_TYPE type);

	private:

		CategoryEntry m_rootEntry;
	};
}

#endif