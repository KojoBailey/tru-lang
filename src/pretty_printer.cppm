module;

#include <print>

export module pretty_printer;

import util;

export {
	template<typename A>
	auto prettyFormat(const A&) -> std::Vector<std::String>;

	void prettyPrint(std::Vector<std::String>& lines)
	{
		for (std::StringView line : lines) {
			std::println("{}", line);
		}
	}
}
