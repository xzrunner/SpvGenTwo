#include "example/Linkage.h"
#include "common/LinkerHelper.h"
#include "spvgentwo/Templates.h"
#include "common/ModulePrinter.h"

#include <stdio.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

using namespace spvgentwo;

spvgentwo::Module examples::linkageLib(spvgentwo::IAllocator* _pAllocator, spvgentwo::ILogger* _pLogger)
{
	// create a new spir-v module
	Module module(_pAllocator, spv::AddressingModel::Logical, spv::MemoryModel::Simple, _pLogger);

	// configure capabilities and extensions
	module.addCapability(spv::Capability::Shader);
	module.addCapability(spv::Capability::Linkage);

	Instruction* uniformVar = module.uniform<float>(u8"u_Time");
	//LinkerHelper::addLinkageDecoration(uniformVar, spv::LinkageType::Export, "@u_Time");

	// float add(float x, float y)
	Function& funcAdd = module.addFunction<float, float, float>(u8"add", spv::FunctionControlMask::Const);
	{
		BasicBlock& bb = *funcAdd; // get entry block to this function

		Instruction* x = funcAdd.getParameter(0);
		Instruction* y = funcAdd.getParameter(1);

		Instruction* z = bb.Add(x, y);
		bb.returnValue(z);
	}

	// float addGlobalTime(float x);
	Function& funcAddGlobalTime = module.addFunction<float, float>(u8"addGlobalTime", spv::FunctionControlMask::Const);
	LinkerHelper::addLinkageDecoration(funcAddGlobalTime.getFunction(), spv::LinkageType::Export, "@addGlobalTime");
	{
		BasicBlock& bb = *funcAddGlobalTime; // get entry block to this function
		Instruction* x = funcAddGlobalTime.getParameter(0);

		Instruction* uTime = bb->opLoad(uniformVar);
		Instruction* z = bb->call(&funcAdd, x, uTime); // call add(s, s)
		bb.returnValue(z);
	}

	// void main();
	{
		EntryPoint& entry = module.addEntryPoint(spv::ExecutionModel::Fragment, u8"main");
		entry.addExecutionMode(spv::ExecutionMode::OriginUpperLeft);

		Instruction* someValue = module.constant(16.f);

		entry->call(&funcAddGlobalTime, someValue); // call add(s, s)
		entry->opReturn();
	}

	return module;
}

spvgentwo::Module examples::linkageConsumer(spvgentwo::IAllocator* _pAllocator, spvgentwo::ILogger* _pLogger)
{
	// create a new spir-v module
	Module module(_pAllocator, spv::AddressingModel::Logical, spv::MemoryModel::Simple, _pLogger);

	// configure capabilities and extensions
	module.addCapability(spv::Capability::Shader);
	module.addCapability(spv::Capability::Linkage);

	//Instruction* uniformVar = module.uniform<float>(u8"u_Time");
	//LinkerHelper::addLinkageDecoration(uniformVar, spv::LinkageType::Import, "@u_Time");

	// float addGlobalTime(float x);
	Function& funcAddGlobalTime = module.addFunction<float, float>(u8"addGlobalTime", spv::FunctionControlMask::Const, false);
	LinkerHelper::addLinkageDecoration(funcAddGlobalTime.getFunction(), spv::LinkageType::Import, "@addGlobalTime");

	// void main();
	{
		EntryPoint& entry = module.addEntryPoint(spv::ExecutionModel::Fragment, u8"main");
		entry.addExecutionMode(spv::ExecutionMode::OriginUpperLeft);

		Instruction* someValue = module.constant(16.f);

		entry->call(&funcAddGlobalTime, someValue); // call add(s, s)
		entry->opReturn();
	}

	return module;
}

bool examples::linkageLinked(const spvgentwo::Module& _lib, spvgentwo::Module& _consumer, spvgentwo::IAllocator* _pAllocator, const spvgentwo::Grammar* _pGrammar)
{
	auto printer = ModulePrinter::ModuleSimpleFuncPrinter([](const char* str) {
		printf("%s", str);

#ifdef _WIN32
		OutputDebugStringA(str);
#endif
	});

	LinkerHelper::LinkerOptions options{};
	options.flags = LinkerHelper::LinkerOptionBits::All;
	options.grammar = _pGrammar;
	options.printer = &printer;
	options.allocator = _pAllocator;

	return LinkerHelper::import(_lib, _consumer, options);
}
