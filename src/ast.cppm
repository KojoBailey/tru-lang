module;

#include <format>
#include <variant>

export module ast;

import util;
import pretty_printer;

export {

	template<>
	auto prettyFormat(const std::String& object) -> std::Vector<PrettyFormatElement>
	{
		return std::Vector<PrettyFormatElement>{{std::format("\"{}\"", object)}};
	}

	class Expression;

	template<>
	auto prettyFormat(const Expression& expression)
		-> std::Vector<PrettyFormatElement>;

	struct Identifier {
		std::String name;

		auto operator==(const Identifier& other) const -> std::Bool
		{
			return name == other.name;
		}
	};

	template<>
	auto prettyFormat(const Identifier& object) -> std::Vector<PrettyFormatElement>
	{
		// return prettyFormat(/*nodeName=*/"Identifier", /*nodeArgs=*/{
		// 	{"name", prettyFormat(object.name)}
		// });
		return std::Vector<PrettyFormatElement>{{object.name}};
	}

	template<>
	struct std::hash<Identifier> {
		auto operator()(const Identifier& self) const noexcept -> USz {
			return std::hash<std::String>{}(self.name);
		}
	};

	// aka Block
	struct ExpressionSequence {
		std::Vector<Expression> expressions;
	};

	struct Declaration {
		std::Maybe<Identifier> newIdentifier; // May be anonymous.
		std::Maybe<Expression*> type; // May be inferred.
	};

	struct Assignment {
		Identifier target;
		Expression* value;
	};

	struct Return {
		Expression* value;
	};

	struct StringLiteral {
		std::String contents;
	};

	template<>
	auto prettyFormat(const StringLiteral& object) -> std::Vector<PrettyFormatElement>
	{
		return std::Vector<PrettyFormatElement>{{std::format("\"{}\"", object.contents)}};
	}

	template<>
	auto prettyFormat(const std::HashMap<Identifier, Expression*>& object)
		-> std::Vector<PrettyFormatElement>
	{
		std::Vector<PrettyFormatElement> result;
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

	struct FunctionCall {
		Identifier callee;
		std::HashMap<Identifier, Expression*> args; // Supports mixed positional & keyword args.
	};

	template<>
	auto prettyFormat(const FunctionCall& object) -> std::Vector<PrettyFormatElement>
	{
		// NOTE: This could be automated with C++26 reflection.
		return prettyFormat(/*nodeName=*/"FunctionCall", /*nodeArgs=*/{
			{"callee", prettyFormat(object.callee)},
			{"args", prettyFormat(object.args)},
		});
	}

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
		std::Variant<
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

	template<>
	auto prettyFormat(const Expression& expression)
		-> std::Vector<PrettyFormatElement>
	{
		if (std::holds_alternative<StringLiteral>(expression.node))
			return prettyFormat(std::get<StringLiteral>(expression.node));
		// TODO: Support other expressions.
		return std::Vector<PrettyFormatElement>{{"<expr>"}};
	}

	struct MemberDeclaration {
		std::Bool isLocal;
		Identifier name;
		Expression* type;
		std::Maybe<Expression*> defaultValue;
	};

	struct Import {
		std::Vector<std::String> path;
		std::Bool isQualified;
		std::Maybe<std::String> qualifierAlias;
	};

	class Component {
	public:
		std::Variant<
			MemberDeclaration,
			Import
		> component;
	};

	class Interface {
	public:
		std::Vector<Component> components;
	};

}
