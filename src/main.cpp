import util;

#include <iostream>
#include <print>
#include <variant>

class Expression;

struct Identifier {
	std::String name;

	auto operator==(const Identifier& other) const -> std::Bool {
		return name == other.name;
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

struct StringLiteral {
	std::String contents;
};

struct FunctionCall {
	Identifier callee;
	std::Map<Identifier, Expression> args; // Supports mixed positional & keyword args.
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
	std::Variant<
		MemberDeclaration,
		Import
	> component;
};

class Interface {
public:
	std::Vector<Component> components;
};

auto main() -> std::CInt
{
	auto expr = Expression{Identifier{"foobar"}};
	std::println("{}", std::get<Identifier>(expr.node).name);

    return 0;
}
