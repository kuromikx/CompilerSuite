#include <iostream>

#include <IR/Builder.hpp>
#include <IR/CFG.hpp>
#include <IR/InterferenceGraph.hpp>

int main() {
	IR::Types types;
	IR::Function function;
	IR::Builder builder (function, types);

	auto left = function.CreateVariable(types.Int32(), { "left" });
	auto right = function.CreateVariable(types.Int32(), { "right" });
	auto result = function.CreateVariable(types.Int32(), { "result" });

	auto initBlock = function.CreateBlock();
	
	builder.SetInsertPoint(initBlock);
	builder.Store(left, function.CreateConstant(types.Int32(), 10));
	builder.Store(right, function.CreateConstant(types.Int32(), 20));
	builder.Store(result, builder.Add(left, right));
	builder.Return(result);

	auto cfg = IR::CFG::Build(function);
	auto ifg = IR::InterferenceGraph::Build(cfg);


	return 0;
}