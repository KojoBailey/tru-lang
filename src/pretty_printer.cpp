module;

#include <print>
#include <variant>

module pretty_printer;

auto prettyFormat(
	StringView nodeName,
	Vector<Pair<String, Vector<PrettyFormatElement>>> nodeArgs
) -> Vector<PrettyFormatElement>
{
	Vector<PrettyFormatElement> result;
	result.emplace_back(std::format("[{}", nodeName));
	result.emplace_back(PrettyNewline{});
	result.emplace_back(PrettyIndent{});
	for (const auto& [key, value] : nodeArgs) {
		result.emplace_back(std::format("${}: ", key));
		result.append_range(value);
		result.emplace_back(PrettyNewline{});
	}
	result.emplace_back(PrettyOutdent{});
	result.emplace_back("]");
	return result;
}

void prettyPrint(const Vector<PrettyFormatElement>& format)
{
	String buffer;

	USz indentDepth = 0;
	for (const auto& item : format) {
		if (std::holds_alternative<String>(item.variant)) {
			buffer += std::get<String>(item.variant);
		}
		if (std::holds_alternative<PrettyIndent>(item.variant)) {
			++indentDepth;
		}
		if (std::holds_alternative<PrettyOutdent>(item.variant)) {
			--indentDepth;
		}
		if (std::holds_alternative<PrettyNewline>(item.variant)) {
			for (USz i = 0; i < indentDepth; ++i) {
				std::print("\t");
			}
			std::println("{}", buffer);
			buffer.clear();
		}
	}

	if (not buffer.empty()) {
		std::println("{}", buffer);
	}
}
