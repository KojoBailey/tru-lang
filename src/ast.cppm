module;

#include <print>
#include <variant>

export module ast;

import util;
import pretty_printer;

export {

	class Expression;

	struct Identifier : PrettyFormatable {
		std::String name;

		Identifier(std::String name)
			: name(std::move(name)) {}

		auto operator==(const Identifier& other) const -> std::Bool
		{
			return name == other.name;
		}

		auto prettyFormat() const -> std::Vector<std::String>
		{
			return {std::format("Identifier(\"{}\")", name)};
		}
	};

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

	struct StringLiteral : PrettyFormatable {
		std::String contents;

		StringLiteral(std::String contents)
			: contents(std::move(contents)) {}

		auto prettyFormat() const -> std::Vector<std::String>
		{
			return {std::format("StringLiteral(\"{}\")", contents)};
		}
	};

	struct FunctionCall {
		Identifier callee;
		std::Map<Identifier, Expression*> args; // Supports mixed positional & keyword args.
											   //
		auto prettyFormat() const -> std::Vector<std::String>
		{
			std::Vector<std::String> result = {
				"FunctionCall(",
				std::format("\tcallee = {},", callee.prettyFormat()[0]),
				"\targs = ("
			};
			for (auto [key, value] : args) {
				result.push_back(
					std::format("\t\t{} -> TBD,", key.prettyFormat()[0])
				);
			}
			result.push_back("\t),");
			result.push_back(")");
			return result;
		}
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
