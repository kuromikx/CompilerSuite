#include <iostream>

#include <IR/Builder.hpp>
#include <IR/CFG.hpp>
#include <IR/InterferenceGraph.hpp>

#include <X64/Allocator.hpp>
#include <X64/MCSelector.hpp>

int main() {
	IR::Types types;
	IR::Function function;
	IR::Builder builder (function, types);

	auto a = function.CreateVariable(types.Int64(), { "a" });
	auto b = function.CreateVariable(types.Int64(), { "b" });
	auto c = function.CreateVariable(types.Int64(), { "c" });
	auto d = function.CreateVariable(types.Int64(), { "d" });

	auto initBlock = function.CreateBlock();
	
	builder.SetInsertPoint(initBlock);
	builder.Store(a, function.CreateConstant(types.Int64(), 10));
	builder.Store(b, function.CreateConstant(types.Int64(), 20));
	builder.Store(c, builder.Add(a, b));
	builder.Store(d, builder.Add(c, c));
	builder.Return(d);

	auto cfg = IR::CFG::Build(function);
	auto ifg = IR::InterferenceGraph::Build(cfg);
	
	X64::Allocator allocator;
	allocator.Allocate(ifg);

	X64::MCBlock output;

	for (auto& blockInfo : cfg.Blocks()) {
		auto selector = X64::MCSelector(allocator);
		auto result = selector.Select(*blockInfo.block);

		for (const auto& ins : result) {
			std::cout << ins.ToString() << '\n';
		}
	}

	return 0;
}