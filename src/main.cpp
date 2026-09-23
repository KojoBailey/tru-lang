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

auto main() -> std::CInt
{
	FunctionCall helloWorld = {
		.callee = Identifier{"printLine"},
		.args = {{Identifier{"0"}, Expression{StringLiteral{"Hello, world!"}}}},
	};

	ExpressionSequence mainFunctionBody;
	mainFunctionBody.expressions.emplace_back(Expression{helloWorld});

	Expression mainFunctionBodyExpr = Expression{mainFunctionBody};
	MemberDeclaration mainFunction = MemberDeclaration{
		.isLocal = false,
		.name = Identifier{"run"},
		.type = nullptr,
		.defaultValue = &mainFunctionBodyExpr,
	};
	Interface program;
	program.components.emplace_back(Component{mainFunction});

	std::String printBuffer = "== PROGRAM START ==\n\n";
	if (std::holds_alternative<MemberDeclaration>(program.components[0].component)) {
		auto memberDeclaration = std::get<MemberDeclaration>(program.components[0].component);
		if (memberDeclaration.isLocal) {
			printBuffer += "local ";
		}
		printBuffer += memberDeclaration.name.name + " ";
		printBuffer += "# ...";
		if (memberDeclaration.defaultValue.has_value() and *memberDeclaration.defaultValue != nullptr) {
			printBuffer += " = ";
			if (std::holds_alternative<ExpressionSequence>((**memberDeclaration.defaultValue).node)) {
				printBuffer += "() -> ():\n";
				auto expressionSequence = std::get<ExpressionSequence>((**memberDeclaration.defaultValue).node);
				for (auto& expression : expressionSequence.expressions) {
					printBuffer += "\t";
					if (std::holds_alternative<FunctionCall>(expression.node)) {
						auto functionCall = std::get<FunctionCall>(expression.node);
						printBuffer += functionCall.callee.name + "; ";
						auto& firstArg = functionCall.args.at(Identifier{"0"});
						if (std::holds_alternative<StringLiteral>(firstArg.node)) {
							auto stringLiteral = std::get<StringLiteral>(firstArg.node);
							printBuffer += "\"" + stringLiteral.contents + "\"";
						}
					}
				}
			}
		}
		printBuffer += "\n";
	}
	printBuffer += "\n== PROGRAM END ==\n";
	std::print("{}", printBuffer);

    return 0;
}
