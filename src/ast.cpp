module;

#include <format>
#include <variant>

module ast;

auto ast::Identifier::operator==(const ast::Identifier& other) const
	-> Bool { return name == other.name; }

template<>
struct std::hash<ast::Identifier> {
	auto operator()(const ast::Identifier& self) const noexcept
		-> USz { return std::hash<String>{}(self.name); }
};

// Strings are printed as quoted strings rather than nodes.
template<>
auto prettyFormat(const String& object) -> Vector<PrettyFormatElement>
{
	return Vector<PrettyFormatElement>{{std::format("\"{}\"", object)}};
}

// Identifiers are printed as simple unquoted strings.
template<>
auto prettyFormat(const ast::Identifier& object) -> Vector<PrettyFormatElement>
{
	return Vector<PrettyFormatElement>{{object.name}};
}

template<>
auto prettyFormat(const ast::StringLiteral& object) -> Vector<PrettyFormatElement>
{
	return Vector<PrettyFormatElement>{{std::format("\"{}\"", object.contents)}};
}

template<>
auto prettyFormat(const Vector<Pair<ast::Identifier, ast::Expression*>>& object)
	-> Vector<PrettyFormatElement>
{
	Vector<PrettyFormatElement> result;
	result.emplace_back("[");
	if (object.size() > 1) {
		result.emplace_back(PrettyNewline{});
		result.emplace_back(PrettyIndent{});
		for (auto& [identifier, expression] : object) {
			result.emplace_back(std::format("{} -> ", identifier.name));
			result.append_range(prettyFormat(*expression));
			result.emplace_back(PrettyNewline{});
		}
		result.emplace_back(PrettyOutdent{});
	} else {
		for (auto& [identifier, expression] : object) {
			result.emplace_back(std::format("{} -> ", identifier.name));
			result.append_range(prettyFormat(*expression));
		}
	}
	result.emplace_back("]");
	return result;
}

template<>
auto prettyFormat(const Vector<ast::Expression>& object) -> Vector<PrettyFormatElement>
{
	Vector<PrettyFormatElement> result;
	result.emplace_back("[");
	result.emplace_back(PrettyNewline{});
	result.emplace_back(PrettyIndent{});
	for (auto& expression : object) {
		result.append_range(prettyFormat(expression));
		result.emplace_back(PrettyNewline{});
	}
	result.emplace_back(PrettyOutdent{});
	result.emplace_back("]");
	return result;
}

template<>
auto prettyFormat(const ast::ExpressionSequence& object) -> Vector<PrettyFormatElement>
{
	// NOTE: This could be automated with C++26 reflection.
	return prettyFormat(/*nodeName=*/"ExpressionSequence", /*nodeArgs=*/{
		{"type", {}},
		{"expressions", prettyFormat(object.expressions)},
	});
}

template<>
auto prettyFormat(const ast::FunctionCall& object) -> Vector<PrettyFormatElement>
{
	// NOTE: This could be automated with C++26 reflection.
	return prettyFormat(/*nodeName=*/"FunctionCall", /*nodeArgs=*/{
		{"callee", prettyFormat(object.callee)},
		{"args", prettyFormat(object.args)},
	});
}

template<>
auto prettyFormat(const ast::Expression& expression)
	-> Vector<PrettyFormatElement>
{
	if (std::holds_alternative<ast::StringLiteral>(expression.node))
		return prettyFormat(std::get<ast::StringLiteral>(expression.node));
	if (std::holds_alternative<ast::FunctionCall>(expression.node))
		return prettyFormat(std::get<ast::FunctionCall>(expression.node));
	// TODO: Support other expressions.
	return Vector<PrettyFormatElement>{{"<expr>"}};
}
