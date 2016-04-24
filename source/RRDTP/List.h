#ifndef List_H
#define List_H
#include <string>

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

			Node(const T& val)
				:m_value(val)
			{}

			void SetValue(const T& val)
			{
				m_value = val;
			}

			const T& GetValue()
			{
				return m_value;
			}

			Node* GetNext()
			{
				return m_next;
			}

		private:

			T m_value;

			Node* m_next;
		};

		List()
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

			return node;
		}

		///@brief Removes a node from the list.
		///@return The node that took the place of the last one (for erasing while iterating).
		Node* Erase(Node* node)
		{
			Node* next = node->GetNext();
			if (next != NULL)
			{
				node->m_data = next->m_data;
				delete next;
			}

			return node;
		}

	private:

		Node m_root;
	};
}

#endif