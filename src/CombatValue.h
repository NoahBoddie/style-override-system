#pragma once

#include "Utility.h"

namespace SOS
{

	ENUM(CombatValue)
	{
		offensiveMult,
		defensiveMult,
		groupOffensiveMult,
		avoidThreatChance,
		//Equip Scores
		meleeScoreMult,
		magicScoreMult,
		rangedScoreMult,
		shoutScoreMult,
		unarmedScoreMult,
		staffScoreMult,
		//Aerial
		hoverChance,
		diveBombChance,
		groundAttackChance,
		hoverTimeMult,
		groundAttackTimeMult,
		perchAttackChance,
		perchAttackTimeMult,
		flyingAttackChance,
		//Ranged
		strafeMult,
		//Dueling
		circleMult,
		fallbackMult,
		//Flank
		flankDistanceMult,
		stalkTimeMult,
		//Stuff?
		specialAttackMult,//Unused, but will keep. Should remain unimplemented.
		attackIncapacitatedMult,
		powerAttackIncapacitatedMult,
		powerAttackBlockingMult,
		//Bashing
		bashMult,
		bashRecoilMult,
		bashAttackMult,
		bashPowerAttackMult,
		ValueTotal,
		//Flagged Values
		// These have to be held somewhat seperately, as they aren't values. Each one counters the other, also meaning, there's no setting
		// for any of these.
		SingleWielding = CombatValue::ValueTotal,
		DualWielding,
		DuelingStyle,
		FlankingStyle,
		//Another divide might happen here for custom values. Make sure to watch it.
		Total,
	};


	constexpr std::array<std::string_view, CombatValue::Total> combatValueNameArray
	{
		"OffensiveMult",
		"DefensiveMult",
		"GroupOffensiveMult",
		"AvoidThreatChance",
		"MeleeScoreMult",
		"MagicScoreMult",
		"RangedScoreMult",
		"ShoutScoreMult",
		"UnarmedScoreMult",
		"StaffScoreMult",
		"HoverChance",
		"DiveBombChance",
		"GroundAttackChance",
		"HoverTimeMult",
		"GroundAttackTimeMult",
		"PerchAttackChance",
		"PerchAttackTimeMult",
		"FlyingAttackChance",
		"StrafeMult",
		"CircleMult",
		"FallbackMult",
		"FlankDistanceMult",
		"StalkTimeMult",
		"SpecialAttackMult",
		"AttackIncapacitatedMult",
		"PowerAttackIncapacitatedMult",
		"PowerAttackBlockingMult",
		"BashMult",
		"BashRecoilMult",
		"BashAttackMult",
		"BashPowerAttackMult",

		"SingleWielding",
		"DualWielding",
		"DuelingStyle",
		"FlankingStyle",
	};

	//Try to make constexpr if you can.
	
	constexpr CombatValue StringToCombatValue(const std::string_view str)
	{
		for (int i = 0; i < combatValueNameArray.size(); i++)
		{
			if (strnicmp(combatValueNameArray[i].data(), str.data(), combatValueNameArray[i].size()) == 0) {
				return static_cast<CombatValue>(i);
			}
		}

		return CombatValue::Total;
	}
	/*
	CombatValue StringToCombatValue(const std::string& str)
	{
		for (int i = 0; i < combatValueNameArray.size(); i++)
		{
			if (strnicmp(combatValueNameArray[i].data(), str.c_str(), combatValueNameArray[i].size()) == 0) {
				return static_cast<CombatValue>(i);
			}
		}

		return CombatValue::Total;
	}
	//*/

	constexpr std::string_view CombatValueToString(CombatValue val)
	{
		if (val < 0 || val > CombatValue::Total)
			return "Total";

		return combatValueNameArray[val];
	}

	constexpr bool IsCombatValueStyleBased(CombatValue cv)
	{
		//Returns false for custom combat values, and for dual wielding, single wielding, flank, and duel
		return CombatValue::ValueTotal > cv;
	}
}