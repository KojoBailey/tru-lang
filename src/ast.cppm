module;

#include <format>
#include <variant>

export module ast;

import util;
import pretty_printer;

using namespace core;

export namespace ast {
	class Expression;

	struct Identifier {
		String name;

		auto operator==(const Identifier& other) const -> Bool
		{
			return name == other.name;
		}
	};

	// aka Block
	struct ExpressionSequence {
		Vector<Expression> expressions;
	};

	struct Declaration {
		Maybe<Identifier> newIdentifier; // May be anonymous.
		Maybe<Expression*> type; // May be inferred.
	};

	struct Assignment {
		Identifier target;
		Expression* value;
	};

	struct Return {
		Expression* value;
	};

	struct StringLiteral {
		String contents;
	};

	struct FunctionCall {
		Identifier callee;
		Vector<Pair<Identifier, Expression*>> args; // Supports mixed positional & keyword args.
	};

	struct BinaryOperator {};

	struct BinaryOperation {
		BinaryOperator op; // NOTE: `operator` is a reserved C++ keyword.
		Expression* leftExpression;
		Expression* rightExpression;
	};

	struct UnaryOperator {};

	struct UnaryOperation {
		UnaryOperator op; // NOTE: `operator` is a reserved C++ keyword.
		Expression* expression;
	};

	class Expression {
	public:
		Variant<
			ExpressionSequence,
			Declaration,
			Assignment,
			Return,
			Identifier,
			StringLiteral,
			FunctionCall,
			BinaryOperation,
			UnaryOperation
		> node;
	};

	struct MemberDeclaration {
		Bool isLocal;
		Identifier name;
		Expression* type;
		Maybe<Expression*> defaultValue;
	};

	struct Import {
		Vector<String> path;
		Bool isQualified;
		Maybe<String> qualifierAlias;
	};

	class Component {
	public:
		Variant<
			MemberDeclaration,
			Import
		> component;
	};

	class Interface {
	public:
		Vector<Component> components;
	};
}

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
