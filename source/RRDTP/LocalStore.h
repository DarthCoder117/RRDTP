#ifndef LocalStore_H
#define LocalStore_H

namespace rrdtp
{
	class LocalStore
	{
	public:

		///@brief Set a 32-bit integer value).
		///@param identifier A valid value identifier.
		///@param val The value to set.
		void SetInt(const char* identifier, int val);
		///@brief Get the 32-bit integer value with the specified identifer.
		///@param identifier A valid value identifier.
		///@param defaultVal The default value that will be returned if the identified value was not found.
		///@return The retrieved value.
		int GetInt(const char* identifier, int defaultVal = 0);
	};
}

#endif