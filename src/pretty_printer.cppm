module;

#include <string>

export module pretty_printer;

import util;

using namespace core;

export {
	struct PrettyIdentifier {
		String name;

		auto operator==(const PrettyIdentifier& other) const -> Bool
		{
			return name == other.name;
		}
	};

	struct PrettyNewline {};
	struct PrettyIndent {};
	struct PrettyOutdent {};

	struct PrettyFormatElement {
		Variant<
			String,
			Bool,
			PrettyIdentifier,
			// PrettyList,
			// PrettyMap,
			PrettyNewline,
			PrettyIndent,
			PrettyOutdent
		> variant;
	};

	auto prettyFormat(
		StringView nodeName,
		Vector<Pair<String, Vector<PrettyFormatElement>>> nodeArgs
	) -> Vector<PrettyFormatElement>;

	template<typename A>
	auto prettyFormat(const A&) -> Vector<PrettyFormatElement> = delete;

	void prettyPrint(const Vector<PrettyFormatElement>& format);

	template<typename T>
	void prettyPrint(const T& object)
	{
		prettyPrint(prettyFormat(object));
	}
}
