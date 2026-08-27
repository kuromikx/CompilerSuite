#include <iostream>

#include <IR/Builder.hpp>
#include <IR/CFG.hpp>
#include <IR/InterferenceGraph.hpp>

#include <X64/Allocator.hpp>
#include <X64/Selector.hpp>
#include <X64/Optimizer.hpp>
#include <X64/Peephole.hpp>

int main() {
	IR::Types types;
	IR::Function function("main");
	IR::Builder builder(function, types);

	auto a = function.CreateVariable(types.Int64(), { "a" });
	auto b = function.CreateVariable(types.Int64(), { "b" });
	auto c = function.CreateVariable(types.Int64(), { "c" });
	auto d = function.CreateVariable(types.Int64(), { "d" });

	auto initBlock = function.CreateBlock();
	auto trueBlock = function.CreateBlock();
	auto falseBlock = function.CreateBlock();
	auto returnBlock = function.CreateBlock();

	builder.SetInsertPoint(initBlock);
	builder.Store(a, function.CreateConstant(types.Int64(), 10));
	builder.Store(b, function.CreateConstant(types.Int64(), 20));
	builder.Store(c, builder.Add(a, b));
	builder.Store(d, builder.Add(c, c));

	builder.Branch(builder.Greater(d, function.CreateConstant(types.Int64(), 15)),
		trueBlock,
		falseBlock
	);

	builder.SetInsertPoint(trueBlock);
	builder.Store(d, function.CreateConstant(types.Int64(), 65));
	builder.Jump(returnBlock);

	builder.SetInsertPoint(falseBlock);
	builder.Store(d, function.CreateConstant(types.Int64(), 66));

	builder.SetInsertPoint(returnBlock);
	builder.Return(d);

	auto cfg = IR::CFG::Build(function);
	auto ifg = IR::InterferenceGraph::Build(cfg);

	X64::Allocator allocator;
	allocator.Allocate(ifg);

	X64::Code code;

	X64::Selector selector(allocator, code);
	selector.Select(function);

	X64::Optimizer optimizer;
	optimizer.Add<X64::Peephole>();
	optimizer.Optimize(code);

	for (const auto& [label, block] : code) {
		std::cout << label.ToString() << ":\n";
		for (const auto& ins : *block) {
			std::cout << '\t' << ins.ToString() << '\n';
		}
	}

	return 0;
}