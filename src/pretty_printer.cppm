module;

#include <string>
#include <variant>

export module pretty_printer;

import util;

export {
	struct PrettyIdentifier {
		std::String name;

		auto operator==(const PrettyIdentifier& other) const -> std::Bool
		{
			return name == other.name;
		}
	};

	struct PrettyNewline {};
	struct PrettyIndent {};
	struct PrettyOutdent {};

	struct PrettyFormatElement {
		std::Variant<
			std::String,
			std::Bool,
			PrettyIdentifier,
			// PrettyList,
			// PrettyMap,
			PrettyNewline,
			PrettyIndent,
			PrettyOutdent
		> variant;
	};

	auto prettyFormat(
		std::StringView nodeName,
		std::Vector<std::Pair<std::String, std::Vector<PrettyFormatElement>>> nodeArgs
	) -> std::Vector<PrettyFormatElement>;

	template<typename A>
	auto prettyFormat(const A&) -> std::Vector<PrettyFormatElement>;

	void prettyPrint(const std::Vector<PrettyFormatElement>& format);
}
