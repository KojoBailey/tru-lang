import util;
import ast;
import pretty_printer;

using namespace core;
using namespace ast;

auto main() -> CInt
{
	Expression stringLiteral = Expression{StringLiteral{"Hello, world!"}};
	FunctionCall helloWorld = {
		.callee = Identifier{"printLine"},
		.args = {
			{Identifier{"0"}, &stringLiteral},
		},
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

	prettyPrint(helloWorld);

    return 0;
}
