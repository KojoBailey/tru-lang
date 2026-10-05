module;

#include <print>
#include <variant>

module pretty_printer;

auto prettyFormat(
	std::StringView nodeName,
	std::Vector<std::Pair<std::String, std::Vector<PrettyFormatElement>>> nodeArgs
) -> std::Vector<PrettyFormatElement>
{
	std::Vector<PrettyFormatElement> result;
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

void prettyPrint(const std::Vector<PrettyFormatElement>& format)
{
	std::String buffer;

	std::USz indentDepth = 0;
	for (const auto& item : format) {
		if (std::holds_alternative<std::String>(item.variant)) {
			buffer += std::get<std::String>(item.variant);
		}
		if (std::holds_alternative<PrettyIndent>(item.variant)) {
			++indentDepth;
		}
		if (std::holds_alternative<PrettyOutdent>(item.variant)) {
			--indentDepth;
		}
		if (std::holds_alternative<PrettyNewline>(item.variant)) {
			for (std::USz i = 0; i < indentDepth; ++i) {
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
