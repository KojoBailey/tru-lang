import util;
import ast;

#include <print>
#include <variant>

auto main() -> std::CInt
{
	Expression stringLiteral = Expression{StringLiteral{"Hello, world!"}};
	FunctionCall helloWorld = {
		.callee = Identifier{"printLine"},
		.args = {{Identifier{"0"}, &stringLiteral}},
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
					if (std::holds_alternative<FunctionCall>(expression.node)) {
						auto functionCall = std::get<FunctionCall>(expression.node);
						for (auto& line : functionCall.prettyFormat()) {
							printBuffer += "\t" + line + "\n";
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
