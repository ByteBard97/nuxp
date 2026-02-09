/*************************************************************************
*
* ADOBE CONFIDENTIAL
* 
* Copyright 2017 Adobe
* 
* All Rights Reserved.
*
* NOTICE: Adobe permits you to use, modify, and distribute this file in
* accordance with the terms of the Adobe license agreement accompanying
* it. If you have received this file from a source other than Adobe,
* then your use, modification, or distribution of it requires the prior
* written permission of Adobe.
*
**************************************************************************/

#pragma once

#include "AITypes.h"

#include <cstddef>
#include <stdexcept>
#include <cstring>
#include <string>
#include <string_view>

namespace ai
{
	/**
	* String Literal Class
	* --------------------
	*  Replacement for:
	*       const char* const kWidgetId = "SomeMagicString";
	*
	*  How to create:
	*       constexpr ai::LiteralString kWidgetId {"WidgetName"};
	*       constexpr ai::LiteralString kWidgetId = "WidgetName";
	*
	*  When to use:
	*       To specify placeholders for magic strings for which we generally use const char* const ...
	*
	*  Why to use:
	*       - When a constexpr object is created with a string literal which is a constant expression (e.g. magic strings), there is:
	*           - No runtime overhead for creation
	*           - Size is known at compile time
	*           - Implicit conversion to const char* when you need to call a function that expects const char*
	*
	*  NOTE:
	*       - You can compare two constexpr ai::LiteralString at compile time.
	*       - However, you can compare LiteralString with const char* with std::strcmp only at runtime.
	***/

	class LiteralString
	{
	public:
		using const_iterator = const char*;

		template<std::size_t N>
		constexpr LiteralString(const char(&arr)[N]) AINOEXCEPT : str{arr}, len{N - 1} 
		{
			AI_STATIC_CHECK(N >= 1, "not a string literal");
		}
    
		constexpr char operator[](std::size_t i) const 
		{
    		return i < len ? str[i] : throw std::out_of_range{""}; 
		}
    
		constexpr std::size_t size() const AINOEXCEPT { return len; }
		
		/** Implicit conversion to const char*
		*/
		constexpr operator const char*() const AINOEXCEPT { return str; } 

		/** Use c_str() when implicit conversion is not applicable. For example, you can use this string in cases such as
			template argument deduction
		*/
		constexpr const char* c_str() const AINOEXCEPT { return str; }

		/** Support for range based loops
		*/
		constexpr const_iterator begin() const AINOEXCEPT { return str; }
		
		/** Support for range based loops
		*/
		constexpr const_iterator end() const AINOEXCEPT { return str + len; }

	private:
		const char* const str;
		const std::size_t len;
	};

	inline constexpr std::string_view to_string_view(const ai::LiteralString& str)
	{
		return std::string_view(str.c_str(), str.size());
	}

	/** Lexicographical comparison
	*/
	inline constexpr bool operator<(const ai::LiteralString& lhs, const ai::LiteralString& rhs) AINOEXCEPT
	{
		return to_string_view(lhs).compare(rhs) < 0;
	}

	/** Lexicographical comparison
	*/
	inline constexpr bool operator>(const ai::LiteralString& lhs, const ai::LiteralString& rhs) AINOEXCEPT
	{
		return (rhs < lhs);
	}

	inline constexpr bool operator==(const ai::LiteralString& lhs, const ai::LiteralString& rhs) AINOEXCEPT
	{
		return to_string_view(lhs).compare(rhs) == 0;
	}

	inline constexpr bool operator!=(const ai::LiteralString& lhs, const ai::LiteralString& rhs) AINOEXCEPT
	{
		return !(lhs == rhs);
	}

	inline constexpr bool operator==(const ai::LiteralString& lhs, const char* rhs)
	{
		return to_string_view(lhs).compare(rhs) == 0;
	}

	inline constexpr bool operator!=(const ai::LiteralString& lhs, const char* rhs)
	{
		return !(lhs == rhs);
	}

	inline constexpr bool operator==(const char* lhs, const ai::LiteralString& rhs)
	{
		return (rhs == lhs);
	}

	inline constexpr bool operator!=(const char* lhs, const ai::LiteralString& rhs)
	{
		return (rhs != lhs);
	}

	inline std::string to_std_string(const ai::LiteralString& str)
	{
		return std::string(str.c_str(), str.size());
	}

	constexpr ai::LiteralString kEmptyLiteralString { "" };

} // namespace ai
