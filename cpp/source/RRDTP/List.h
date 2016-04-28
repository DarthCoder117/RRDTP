#ifndef List_H
#define List_H
#include <string>
#include <cassert>

namespace rrdtp
{
	///@brief Singly linked list to avoid using the standard library.
	template <typename T>
	class List
	{
	public:

		///@brief Store links and data in the list.
		class Node
		{
		public:

			Node()
				:m_value(0),
				m_next(NULL)
			{}

			Node(const T& val)
				:m_value(val),
				m_next(NULL)
			{}

			void SetValue(const T& val)
			{
				m_value = val;
			}

			const T& GetValue()
			{
				return m_value;
			}

			bool HasNext()
			{
				return m_next != NULL;
			}

			Node* GetNext()
			{
				return m_next;
			}

		private:

			friend class List;

			T m_value;

			Node* m_next;
		};

		List()
			:m_size(0)
		{}

		~List()
		{}

		///@return The first node in the linked list, or NULL if the list is empty.
		Node* Begin()
		{
			return m_root.GetNext();
		}

		///@brief Inserts a new node either at the beginning of the list, or before location.
		///@param location The node to insert the new value before. If this is left NULL, then the value will be inserted at the beginning of the list (right after the root).
		///@return The new node so that insertion can be done while iterating.
		Node* Insert(const T& val, Node* location = NULL)
		{
			Node* node = new Node(val);

			if (location == NULL)
			{
				location = &m_root;
			}

			//Insert node by swapping links around.
			Node* next = location->GetNext();
			location->m_next = node;
			node->m_next = next;

			m_size++;

			return node;
		}

		///@brief Removes a node from the list.
		///@return The node that took the place of the last one (for erasing while iterating).
		Node* Erase(Node* node)
		{
			assert(node != NULL);

			Node* next = node->GetNext();
			if (next != NULL)
			{
				node->m_value = next->m_value;
				delete next;
			}

			m_size--;
			return node;
		}

		///@brief Removes all nodes in the list.
		void Clear()
		{
			while (Size() > 0)
			{
				Erase(Begin());
			}
		}

		///@return The number of elements in the list.
		unsigned int Size()
		{
			return m_size;
		}

	private:

		unsigned int m_size;

		Node m_root;
	};
}

#endif