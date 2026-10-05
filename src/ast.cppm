module;

#include <format>
#include <variant>

export module ast;

import util;
import pretty_printer;

using namespace core;

export namespace ast {
	struct Identifier;

	struct ExpressionSequence; // aka Block

	struct Declaration;

	struct Assignment;

	struct Return;

	struct StringLiteral;

	struct FunctionCall;

	struct BinaryOperator;

	struct BinaryOperation;

	struct UnaryOperator;

	struct UnaryOperation;

	class Expression;

	struct MemberDeclaration;

	struct Import;

	class Component;

	class Interface;
}

struct ast::Identifier {
	String name;

	auto operator==(const ast::Identifier& other) const -> Bool
	{
		return name == other.name;
	}
};

// aka Block
struct ast::ExpressionSequence {
	Vector<ast::Expression> expressions;
};

struct ast::Declaration {
	Maybe<ast::Identifier> newIdentifier; // May be anonymous.
	Maybe<ast::Expression*> type; // May be inferred.
};

struct ast::Assignment {
	ast::Identifier target;
	ast::Expression* value;
};

struct ast::Return {
	ast::Expression* value;
};

struct ast::StringLiteral {
	String contents;
};

struct ast::FunctionCall {
	ast::Identifier callee;
	Vector<Pair<ast::Identifier, ast::Expression*>> args; // Supports mixed positional & keyword args.
};

struct ast::BinaryOperator {};

struct ast::BinaryOperation {
	ast::BinaryOperator op; // NOTE: `operator` is a reserved C++ keyword.
	ast::Expression* leftExpression;
	ast::Expression* rightExpression;
};

struct ast::UnaryOperator {};

struct ast::UnaryOperation {
	ast::UnaryOperator op; // NOTE: `operator` is a reserved C++ keyword.
	ast::Expression* expression;
};

class ast::Expression {
public:
	Variant<
		ast::ExpressionSequence,
		ast::Declaration,
		ast::Assignment,
		ast::Return,
		ast::Identifier,
		ast::StringLiteral,
		ast::FunctionCall,
		ast::BinaryOperation,
		ast::UnaryOperation
	> node;
};

struct ast::MemberDeclaration {
	Bool isLocal;
	ast::Identifier name;
	ast::Expression* type;
	Maybe<ast::Expression*> defaultValue;
};

struct ast::Import {
	Vector<String> path;
	Bool isQualified;
	Maybe<String> qualifierAlias;
};

class ast::Component {
public:
	Variant<
		ast::MemberDeclaration,
		ast::Import
	> component;
};

class ast::Interface {
public:
	Vector<ast::Component> components;
};

export {
	template<>
	struct std::hash<ast::Identifier> {
		auto operator()(const ast::Identifier& self) const noexcept -> USz {
			return std::hash<String>{}(self.name);
		}
	};

	template<>
	auto prettyFormat(const ast::Expression& expression)
		-> Vector<PrettyFormatElement>;

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
		// TODO: Support other expressions.
		return Vector<PrettyFormatElement>{{"<expr>"}};
	}
}
