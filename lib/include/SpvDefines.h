#pragma once

#define SPV_ENABLE_UTILITY_CODE
#include <vulkan/spirv.hpp11>

namespace spvgentwo
{
	constexpr bool isType(const spv::Op _type)
	{		
		if ((_type >= spv::Op::OpTypeVoid && _type <= spv::Op::OpTypeForwardPointer) || // basic types
			(_type >= spv::Op::OpTypeVmeImageINTEL && _type <= spv::Op::OpTypeAvcSicResultINTEL) // intel ext
			)
		{
			return true;
		}

		// special cases and small extensions
		switch (_type)
		{
		case spv::Op::OpTypePipeStorage:
		case spv::Op::OpTypeNamedBarrier:
		case spv::Op::OpTypeAccelerationStructureNV:
		case spv::Op::OpTypeCooperativeMatrixNV:
			return true;
		default:
			return false;
		}
	}

	constexpr size_t wordCount(const size_t _byteCount)
	{
		return (_byteCount / sizeof(spv::Id)) + (_byteCount % sizeof(spv::Id) != 0u ? 1u : 0u);
	}
} //!spvgentwo