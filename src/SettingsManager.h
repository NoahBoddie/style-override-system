#pragma once

#include "Utility.h"
#include "FloatSetting.h"
#include "CombatValue.h"


#define PREFIX() fCS_
#define SUFFIX() Max
#define SETTTING_NAME_WITH(content, suffix) STRINGIZE(PREFIX()) STRINGIZE(content) STRINGIZE(suffix)
#define SETTTING_NAME(content) SETTTING_NAME_WITH(content, SUFFIX())
namespace SOS
{

	constexpr const char* prefix = SETTTING_NAME(Test);
	struct SettingsManager
	{
		//I'll keep them like this, because it would be a pain to do it otherwise.
		inline static std::array<RE::FloatSetting, CombatValue::ValueTotal> styleSettingsArray
		{
			RE::FloatSetting(SETTTING_NAME(offensiveMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(defensiveMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(groupOffensiveMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(avoidThreatChance), 1.0f),
			RE::FloatSetting(SETTTING_NAME(staffScoreMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(hoverChance), 1.0f),
			RE::FloatSetting(SETTTING_NAME(diveBombChance), 1.0f),
			RE::FloatSetting(SETTTING_NAME(groundAttackChance), 1.0f),
			RE::FloatSetting(SETTTING_NAME(hoverTimeMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(groundAttackTimeMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(perchAttackChance), 1.0f),
			RE::FloatSetting(SETTTING_NAME(perchAttackTimeMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(flyingAttackChance), 1.0f),
			RE::FloatSetting(SETTTING_NAME(strafeMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(circleMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(fallbackMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(flankDistanceMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(stalkTimeMult), 1.0f),
			RE::FloatSetting(SETTTING_NAME(specialAttackMult), 1.0f),

			RE::FloatSetting(SETTTING_NAME(meleeScoreMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(magicScoreMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(rangedScoreMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(shoutScoreMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(unarmedScoreMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(attackIncapacitatedMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(powerAttackIncapacitatedMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(powerAttackBlockingMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(bashMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(bashRecoilMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(bashAttackMult), 10.f),
			RE::FloatSetting(SETTTING_NAME(bashPowerAttackMult), 10.f)
		};
		
		static uint8_t GetUpdateTag() { return settingUpdateTag; }
		//if the small bits in the combat style aren't the same as this, it will purge it's values
		inline static uint8_t settingUpdateTag = 1;

		//A function used to check if settings have changed values, and if so, updates their old values and returns true.
		static void Update()
		{
			bool result = false;

			for (int i = 0; i < styleSettingsArray.size(); i++)
			{
				RE::FloatSetting& setting = styleSettingsArray[i];

				//if the result
				if (result || setting.currValue != setting.prevValue) {
					if (!result) {
						if (settingUpdateTag == 254)
							settingUpdateTag = 1;
						else
							settingUpdateTag++;

						result = true;
					}
					setting.Update();
				}

			}
		}

		static void Init()
		{
			RE::GameSettingCollection* collection = RE::GameSettingCollection::GetSingleton();

			if (collection)
			{
				for (int i = 0; i < styleSettingsArray.size(); i++)
				{
					collection->InsertSetting(styleSettingsArray[i]);
				}
			}
		}


	};
}



#undef PREFIX
#undef SUFFIX
#undef SETTTING_NAME
#undef SETTTING_NAME_WITH