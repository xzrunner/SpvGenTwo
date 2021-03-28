#pragma once
#include "spvgentwo/Spv.h"

namespace spvgentwo
{
	// forward decls
	class Module;
	class Function;
	class IAllocator;

	namespace LinkerHelper
	{
		// turn function definition into a declaration by removing its basic blocks and instruction decorates/names from the module
		bool removeFunctionBody(Function& _func);

		// adds OpDecorateLinkage to global variables referenced in _func, use variable names (from OpName) as export name, if no name is present, it can't be exportet
		bool addLinkageDecorateForUsedGlobalVariables(const Function& _func, spv::LinkageType _linkage, IAllocator* _pAllocator = nullptr);

		struct LinkerOptions {};

		// import exported symbols of _lib to _consumber
		bool import(const Module& _lib, Module& _consumer, const LinkerOptions& _options, IAllocator* _pAllocator = nullptr);
	} // !LinkerHelper
} // spvgentwo