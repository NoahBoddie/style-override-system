#pragma once

#include "Utility.h"

namespace SOS
{
	ENUM(StyleValueMask)
	{
		None		= 0,
		Base		= 1 << 0,
		Modifier	= 1 << 1,
		Damage		= 1 << 2
	};
}