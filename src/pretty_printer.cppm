module;

#include <print>

export module pretty_printer;

import util;

export {
	class PrettyFormatable {
	public:
		virtual auto prettyFormat() const -> std::Vector<std::String> = 0;
	};

	void prettyPrint(std::Vector<std::String>& lines)
	{
		for (std::StringView line : lines) {
			std::println("{}", line);
		}
	}
}
