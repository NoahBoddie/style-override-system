#pragma once

#include "Utility.h"


namespace RE
{
	class FloatSetting
	{
	public:
		
		virtual ~FloatSetting() {}  // 00

		// members
		float			currValue;	// 0x00
		mutable float	prevValue;	// 0x08
		const char*		name;	// 0x10

		float GetValue() const { return currValue; }

		void Update() const { prevValue = currValue; }

		FloatSetting(const char* a_name, float a_value)
		{
			name = a_name;
			currValue = a_value;
			prevValue = a_value;
		}

		operator float() { return currValue; }
		operator RE::Setting* () { return reinterpret_cast<RE::Setting*>(this); }
	};
	static_assert(sizeof(FloatSetting) == 0x18);
}



namespace SOS
{

	using ActorValueModifier = RE::ACTOR_VALUE_MODIFIER;


	ENUM(CombatStyleInput)
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
		specialAttackMult,
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
		SingleWielding = CombatStyleInput::ValueTotal,
		DualWielding,
		DuelingStyle,
		FlankingStyle,
		Total,
	};


	ENUM(StyleValueMask)
	{
		None		= 0,
		Base		= 1 << 0,
		Modifier	= 1 << 1,
		Damage		= 1 << 2
	};


	constexpr std::array<std::string_view, CombatStyleInput::Total> extraValueNameArray
	{
		"offensiveMult",
		"defensiveMult",
		"groupOffensiveMult",
		"avoidThreatChance",
		"meleeScoreMult",
		"magicScoreMult",
		"rangedScoreMult",
		"shoutScoreMult",
		"unarmedScoreMult",
		"staffScoreMult",
		"hoverChance",
		"diveBombChance",
		"groundAttackChance",
		"hoverTimeMult",
		"groundAttackTimeMult",
		"perchAttackChance",
		"perchAttackTimeMult",
		"flyingAttackChance",
		"strafeMult",
		"circleMult",
		"fallbackMult",
		"flankDistanceMult",
		"stalkTimeMult",
		"specialAttackMult",
		"attackIncapacitatedMult",
		"powerAttackIncapacitatedMult",
		"powerAttackBlockingMult",
		"bashMult",
		"bashRecoilMult",
		"bashAttackMult",
		"bashPowerAttackMult",
		"singleWielding",
		"dualWielding",
		"duelingStyle",
		"flankingStyle",
	};

	CombatStyleInput StringToCombatStyleInput(const std::string& str)
	{
		for (int i = 0; i < extraValueNameArray.size(); i++)
		{
			if (strnicmp(extraValueNameArray[i].data(), str.c_str(), extraValueNameArray[i].size()) == 0) {
				return static_cast<CombatStyleInput>(i);
			}
		}

		return CombatStyleInput::Total;
	}

	
	//I'll keep them like this, because it would be a pain to do it otherwise.
	std::array<RE::FloatSetting, CombatStyleInput::ValueTotal> styleSettingsArray
	{
		RE::FloatSetting("fCS_offensiveMult", 1.0f),
		RE::FloatSetting("fCS_defensiveMult", 1.0f),
		RE::FloatSetting("fCS_groupOffensiveMult", 1.0f),
		RE::FloatSetting("fCS_avoidThreatChance", 1.0f),
		RE::FloatSetting("fCS_meleeScoreMult", 10.f),
		RE::FloatSetting("fCS_magicScoreMult", 10.f),
		RE::FloatSetting("fCS_rangedScoreMult", 10.f),
		RE::FloatSetting("fCS_shoutScoreMult", 10.f),
		RE::FloatSetting("fCS_unarmedScoreMult", 10.f),
		RE::FloatSetting("fCS_staffScoreMult", 10.f),
		RE::FloatSetting("fCS_hoverChance", 1.0f),
		RE::FloatSetting("fCS_diveBombChance", 1.0f),
		RE::FloatSetting("fCS_groundAttackChance", 1.0f),
		RE::FloatSetting("fCS_hoverTimeMult", 1.0f),
		RE::FloatSetting("fCS_groundAttackTimeMult", 1.0f),
		RE::FloatSetting("fCS_perchAttackChance", 1.0f),
		RE::FloatSetting("fCS_perchAttackTimeMult", 1.0f),
		RE::FloatSetting("fCS_flyingAttackChance", 1.0f),
		RE::FloatSetting("fCS_strafeMult", 1.0f),
		RE::FloatSetting("fCS_circleMult", 1.0f),
		RE::FloatSetting("fCS_fallbackMult", 1.0f),
		RE::FloatSetting("fCS_flankDistanceMult", 1.0f),
		RE::FloatSetting("fCS_stalkTimeMult", 1.0f),
		RE::FloatSetting("fCS_specialAttackMult", 1.0f),
		RE::FloatSetting("fCS_attackIncapacitatedMult", 10.f),
		RE::FloatSetting("fCS_powerAttackIncapacitatedMult", 10.f),
		RE::FloatSetting("fCS_powerAttackBlockingMult", 10.f),
		RE::FloatSetting("fCS_bashMult", 10.f),
		RE::FloatSetting("fCS_bashRecoilMult", 10.f),
		RE::FloatSetting("fCS_bashAttackMult", 10.f),
		RE::FloatSetting("fCS_bashPowerAttackMult", 10.f)
	};
	//if the small bits in the combat style aren't the same as this, it will purge it's values
	uint8_t settingUpdateTag = 0;

	//A function used to check if settings have changed values, and if so, updates their old values and returns true.
	void SettingUpdate()
	{
		bool result = false;

		for (int i = 0; i < styleSettingsArray.size(); i++)
		{
			RE::FloatSetting& setting = styleSettingsArray[i];

			//if the result
			if (result || setting.currValue != setting.prevValue) {
				if (!result) {
					settingUpdateTag++;
					result = true;
				}
				setting.Update();
			}

		}
	}
	//using StyleModifier = std::pair<float, float>;


	float* GetCombatStyleValueSafe(RE::TESCombatStyle* style, CombatStyleInput input)
	{

		if (style)
		{

			switch (input)
			{
				case CombatStyleInput::offensiveMult:
					return &style->generalData.offensiveMult;
				case CombatStyleInput::defensiveMult:
					return &style->generalData.defensiveMult;
				case CombatStyleInput::groupOffensiveMult:
					return &style->generalData.groupOffensiveMult;
				case CombatStyleInput::avoidThreatChance:
					return &style->generalData.avoidThreatChance;
				case CombatStyleInput::meleeScoreMult:
					return &style->generalData.meleeScoreMult;
				case CombatStyleInput::magicScoreMult:
					return &style->generalData.magicScoreMult;
				case CombatStyleInput::rangedScoreMult:
					return &style->generalData.rangedScoreMult;
				case CombatStyleInput::shoutScoreMult:
					return &style->generalData.shoutScoreMult;
				case CombatStyleInput::unarmedScoreMult:
					return &style->generalData.unarmedScoreMult;
				case CombatStyleInput::staffScoreMult:
					return &style->generalData.staffScoreMult;


				case CombatStyleInput::hoverChance:
					return &style->flightData.hoverChance;
				case CombatStyleInput::diveBombChance:
					return &style->flightData.diveBombChance;
				case CombatStyleInput::groundAttackChance:
					return &style->flightData.groundAttackChance;
				case CombatStyleInput::hoverTimeMult:
					return &style->flightData.hoverTimeMult;
				case CombatStyleInput::groundAttackTimeMult:
					return &style->flightData.groundAttackTimeMult;
				case CombatStyleInput::perchAttackChance:
					return &style->flightData.perchAttackChance;
				case CombatStyleInput::perchAttackTimeMult:
					return &style->flightData.perchAttackTimeMult;
				case CombatStyleInput::flyingAttackChance:
					return &style->flightData.flyingAttackChance;

				case CombatStyleInput::strafeMult:
					return &style->longRangeData.strafeMult;

				case CombatStyleInput::circleMult:
					return &style->closeRangeData.circleMult;
				case CombatStyleInput::fallbackMult:
					return &style->closeRangeData.fallbackMult;
				case CombatStyleInput::flankDistanceMult:
					return &style->closeRangeData.flankDistanceMult;
				case CombatStyleInput::stalkTimeMult:
					return &style->closeRangeData.stalkTimeMult;


				case CombatStyleInput::specialAttackMult:
					return &style->meleeData.specialAttackMult;
				case CombatStyleInput::attackIncapacitatedMult:
					return &style->meleeData.attackIncapacitatedMult;
				case CombatStyleInput::powerAttackIncapacitatedMult:
					return &style->meleeData.powerAttackIncapacitatedMult;
				case CombatStyleInput::powerAttackBlockingMult:
					return &style->meleeData.powerAttackBlockingMult;
				case CombatStyleInput::bashMult:
					return &style->meleeData.bashMult;
				case CombatStyleInput::bashRecoilMult:
					return &style->meleeData.bashRecoilMult;
				case CombatStyleInput::bashAttackMult:
					return &style->meleeData.bashAttackMult;
				case CombatStyleInput::bashPowerAttackMult:
					return &style->meleeData.bashPowerAttackMult;
			}
		}

		return nullptr;
	}


	inline float& GetCombatStyleValueUnsafe(RE::TESCombatStyle* style, CombatStyleInput input) { return *GetCombatStyleValueSafe(style, input); }


	inline float GetCombatStyleValue(RE::TESCombatStyle* style, CombatStyleInput input) 
	{
		auto result = GetCombatStyleValueSafe(style, input); 
		
		return result ? *result : 0;
	}


	struct CombatStyleManager
	{
		//I see a max of 7 threads, so I'm gonna make it 8 instead. 10 to be safe
		constexpr static size_t size = 10;

		//If this idea works out, this shall not be stored, nor shall it be saved. It will only be copied to and then removed.
		// Hopefully, it shant make it's way into papyrus, but we'll see.
	
		static std::map<std::thread::id, RE::TESCombatStyle*> threadMap;

		static std::queue<RE::TESCombatStyle*> queue;

		static std::mutex _mutex;


		static void TransferData(RE::TESCombatStyle* real_style, RE::TESCombatStyle* temp_style)
		{
			//Needs to be some rules to make sure each is each.
			if (!real_style || !temp_style)
				return;

			temp_style->generalData.offensiveMult = real_style->generalData.offensiveMult;
			temp_style->generalData.defensiveMult = real_style->generalData.defensiveMult;
			temp_style->generalData.groupOffensiveMult = real_style->generalData.groupOffensiveMult;
			temp_style->generalData.avoidThreatChance = real_style->generalData.avoidThreatChance;
			temp_style->generalData.meleeScoreMult = real_style->generalData.meleeScoreMult;
			temp_style->generalData.magicScoreMult = real_style->generalData.magicScoreMult;
			temp_style->generalData.rangedScoreMult = real_style->generalData.rangedScoreMult;
			temp_style->generalData.shoutScoreMult = real_style->generalData.shoutScoreMult;
			temp_style->generalData.unarmedScoreMult = real_style->generalData.unarmedScoreMult;
			temp_style->generalData.staffScoreMult = real_style->generalData.staffScoreMult;


			temp_style->flightData.hoverChance = real_style->flightData.hoverChance;
			temp_style->flightData.diveBombChance = real_style->flightData.diveBombChance;
			temp_style->flightData.groundAttackChance = real_style->flightData.groundAttackChance;
			temp_style->flightData.hoverTimeMult = real_style->flightData.hoverTimeMult;
			temp_style->flightData.groundAttackTimeMult = real_style->flightData.groundAttackTimeMult;
			temp_style->flightData.perchAttackChance = real_style->flightData.perchAttackChance;
			temp_style->flightData.perchAttackTimeMult = real_style->flightData.perchAttackTimeMult;
			temp_style->flightData.flyingAttackChance = real_style->flightData.flyingAttackChance;

			temp_style->longRangeData.strafeMult = real_style->longRangeData.strafeMult;

			temp_style->closeRangeData.circleMult = real_style->closeRangeData.circleMult;
			temp_style->closeRangeData.fallbackMult = real_style->closeRangeData.fallbackMult;
			temp_style->closeRangeData.flankDistanceMult = real_style->closeRangeData.flankDistanceMult;
			temp_style->closeRangeData.stalkTimeMult = real_style->closeRangeData.stalkTimeMult;


			temp_style->meleeData.specialAttackMult = real_style->meleeData.specialAttackMult;
			temp_style->meleeData.attackIncapacitatedMult = real_style->meleeData.attackIncapacitatedMult;
			temp_style->meleeData.powerAttackIncapacitatedMult = real_style->meleeData.powerAttackIncapacitatedMult;
			temp_style->meleeData.powerAttackBlockingMult = real_style->meleeData.powerAttackBlockingMult;
			temp_style->meleeData.bashMult = real_style->meleeData.bashMult;
			temp_style->meleeData.bashRecoilMult = real_style->meleeData.bashRecoilMult;
			temp_style->meleeData.bashAttackMult = real_style->meleeData.bashAttackMult;
			temp_style->meleeData.bashPowerAttackMult = real_style->meleeData.bashPowerAttackMult;
		}


		static bool CompareStyle(RE::TESCombatStyle* style_1, RE::TESCombatStyle* style_2)
		{
			//might move this
			constexpr size_t combatStyleDataLength = 0x7C;

			if (!style_1 || !style_2 || std::memcmp(&style_1->generalData, &style_2->generalData, combatStyleDataLength) != 0)
				return false;
			else
				return true;
		}

		static bool RequestStyle()
		{
			RE::TESCombatStyle*& entry = threadMap[std::this_thread::get_id()];

			if (!entry)
			{
				//We lock here.
				entry = queue.front();

				static bool once = false;

				if (!entry && !once) {
					logger::critical("Style request failed, no more temp styles.");
					once = true;
				}

				queue.pop();
			}

			return entry;
		}

		static void Init()
		{			
			auto* factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::TESCombatStyle>();

			for (size_t i = 0; i < size; i++)
			{
				RE::TESCombatStyle* temp_style = factory->Create();
				logger::info("Temp CombatStyle {:08X} created.", temp_style->formID);
				queue.push(temp_style);
			}
		}
	};


	void ModifyCombatStyle(RE::TESCombatStyle* style, CombatStyleInput input, float value)
	{
		//should I thread lock this function?

		if (!style || value == 0 || isnan(value) == true)
			return;

		float& style_value = GetCombatStyleValueUnsafe(style, input);

		style_value += value;
	}

	RE::TESCombatStyle* GetCombatStyle(RE::Actor* a_this)
	{
		if (!a_this)
			return nullptr;

		auto extra_style = a_this->extraList.GetByType<RE::ExtraCombatStyle>();


		RE::TESCombatStyle* style = nullptr;


		if (!extra_style || !extra_style->combatStyle)
		{

			auto& actor_runtime = a_this->GetActorRuntimeData();

			if (actor_runtime.currentProcess && actor_runtime.currentProcess->currentPackage.package)
			{
				style = actor_runtime.currentProcess->currentPackage.package->combatStyle;
			}

			if (!style)
			{
				RE::TESNPC* npc_base = skyrim_cast<RE::TESNPC*>(a_this->GetBaseObject());

				if (npc_base) {
					style = npc_base->combatStyle;
				}

			}
		}
		else
		{
			style = extra_style->combatStyle;
		}
		return style;
		

	}

	struct StyleModifier
	{
		float value;
		float damage;
	};


	struct StyleCache
	{
		float value;
		float modifier;

		StyleCache() = default;
		StyleCache(float v, float m) :value{ v }, modifier{m} {}
	};

	struct FightingStyle
	{
	private:
		inline static std::map<RE::FormID, FightingStyle*> _sheetTable{};

	public:

		static void RemoveAllStorages()
		{
			for (auto& [key, ptr] : _sheetTable)
			{
				delete ptr;
			}

			_sheetTable.clear();
		}


		static FightingStyle* GetStorage(RE::FormID id)
		{
			//This won't be used often, but it's used in situations where it would be ok to not create storage yet.
			if (!id) {
				return nullptr;
			}

			//if (actor->IsPlayerRef() == true) {
			//	return nullptr;
			//}
			auto result = _sheetTable.find(id);

			return result == _sheetTable.end() ? nullptr : result->second;
		}

		static FightingStyle* GetStorage(RE::Actor* actor)
		{
			//This won't be used often, but it's used in situations where it would be ok to not create storage yet.
			if (!actor) {
				return nullptr;
			}

			//if (actor->IsPlayerRef() == true) {
			//	return nullptr;
			//}
			auto result = _sheetTable.find(actor->formID);

			return result == _sheetTable.end() ? nullptr : result->second;
		}


		static FightingStyle* GetCreateStorage(RE::Actor* actor)
		{
			//Should this be thead locked? I feel like this should be thread locked

			if (!actor) {
				logger::critical("No actor detected, terminating storage search.");
				return nullptr;  //Just crash, this isn't supposed to be found.
			}

			//if (actor->IsPlayerRef() == true) {
			//	return nullptr;
			//}

			FightingStyle*& storage_spot = _sheetTable[actor->formID];

			if (!storage_spot) {
				//logger::warn("Stor loc {}", (uintptr_t)storage_spot);
				storage_spot = new FightingStyle(actor->formID);
			}


			return storage_spot;
		}


		static bool RemoveStorage(RE::FormID _id)
		{
			if (!_id || _sheetTable.contains(_id) == false)
				return false;


			if (_id == 0x14) {
				//logger::debug("PlayerCharacter does not have a combat style to process.");
				//return false;

			}

			//needs an initializer part
			//It will need to search the left hand, the right hand
			logger::info("[Unregister {:08X} ]", _id);


			FightingStyle* storage = _sheetTable[_id];

			//LOCK while removing

			_sheetTable.erase(_id);

			delete storage;

			return true;
		}




		//This will likely delete relatively often, so delete is likely for the best.
		std::map<CombatStyleInput, StyleModifier> modifierMap;

		//When an update to a combat style goes through, this will check the left value for equivalency. If any value isn't the same,
		// all old modifiers stored in the right side will be removed from the combat map.
		std::unordered_map<CombatStyleInput, StyleCache> modifierCache;

		RE::FormID _id{};

		RE::TESCombatStyle* currentStyle{ nullptr };

		FightingStyle(RE::FormID i) : _id { i } {};

		//This checks only if it needs to remove itself from the given function.
		bool CacheIsValid(RE::TESCombatStyle* style)
		{
			if (style)
			{
				if (style == currentStyle)
				{
					for (auto& [input, cache] : modifierCache)
					{
						float* value = GetCombatStyleValueSafe(style, input);

						if (value && *value == cache.value) {
							return false;
						}
					}

					return true;
				}
			}
			
			return false;
		}

		//This was supposed to work.
		bool CacheIsValid_Changed(RE::TESCombatStyle* style)
		{
			if (style && style != currentStyle)
			{
				for (auto& [input, cache] : modifierCache)
				{
					float* value = GetCombatStyleValueSafe(style, input);

					if (value && *value == cache.value) {
						return false;
					}
				}
			}
			return true;
		}


		//These should have something for the flag values too.
		void RemoveCache(RE::TESCombatStyle* style)
		{
			if (style)
			{
				for (auto& [input, cache] : modifierCache) {
					ModifyCombatStyle(style, input, -cache.modifier);
				}

				style->pad1C = NULL;
			}

			//Clears either way
			modifierCache.clear();
		}


		void ApplyModifiers(RE::TESCombatStyle* style)
		{
			if (!style)
				return;

			style->pad1C = _id;

			for (auto& [input, modifier] : modifierMap)
			{
				float* v = GetCombatStyleValueSafe(style, input);

				if (!v) {
					continue;
				}

				float& value = *v;

				float max = styleSettingsArray[input].GetValue();

				//This needs to be a clamp rather than a min.
				//float base = fmin(modifier.value, max - value);
				float base = fmin(modifier.value - modifier.damage, max - value);

				//Raw value goes first, then we place damage.
				//base -= modifier.damage;

				value += base;

				modifierCache[input] = StyleCache(value, base);

			}
		}

		void TryDestroyModifier(CombatStyleInput input, StyleModifier& modifier)
		{
			//Note, this must own this to destroy it.
			if (!modifier.value && !modifier.damage)
				modifierMap.erase(input);
		}
		//Later, I'd like this to do get. I would also like to use the mask instead.
		float GetStyleModifer(CombatStyleInput input, bool is_damage, RE::TESCombatStyle* style = nullptr)
		{
			auto it = modifierMap.find(input);

			if (modifierMap.end() != it) {
				return is_damage ? it->second.damage :  it->second.value;
			}

			return 0;
		}

		//This is't functioning right I don't think, doesn't account for if it doesn't own it.
		// Would like to redo this.
		float GetOriginalValue(CombatStyleInput input, RE::TESCombatStyle* style = nullptr)
		{
			if (!style)
				style = currentStyle;

			float current_value = GetCombatStyleValue(style, input);

			float mod_value = 0;

			auto it = modifierCache.find(input);

			if (modifierCache.end() != it) {
				mod_value = it->second.modifier;
			}

			return current_value - mod_value;
		}

		void ModifyStyle(CombatStyleInput input, float value)
		{
			//should I thread lock this function?
			if (input == CombatStyleInput::Total || value == 0 || isnan(value) == true)
				return;
			
			StyleModifier& modifier = modifierMap[input];

			modifier.value += value;
			
			logger::debug("{} {}", (int)input, modifier.value);

			TryDestroyModifier(input, modifier);
		}

		void DamageStyle(CombatStyleInput input, float value)
		{
			//should I thread lock this function?

			if (value == 0 || isnan(value) == true)
				return;
			StyleModifier& modifier = modifierMap[input];

			modifier.damage += value;

			TryDestroyModifier(input, modifier);
		}

		inline static std::mutex style_lock;

		static void RevertStyle(RE::TESCombatStyle* style)
		{
			std::lock_guard<std::mutex> lock{ style_lock };

			if (!style || !style->pad1C)
				return;
			

			FightingStyle* owner = GetStorage(style->pad1C);
			
			logger::info("before something stupid (again) {:08X}", style->pad1C);


			if (owner->CacheIsValid(style) == false)
			{
				logger::debug("Reverting Cache for {:08X}", owner->_id);
				owner->RemoveCache(owner->currentStyle);
			}

		}

		RE::TESCombatStyle* SetCombatStyle(RE::TESCombatStyle* style)
		{
			if (!style)
				return style;

			std::lock_guard<std::mutex> lock{ style_lock };
			//Lock this function

			//checks for 2 things, is this ours, and are these values what our cache says it is.
			
			//Encorporate revert.
			FightingStyle* owner = !style->pad1C || style->pad1C == _id ? this : GetStorage(style->pad1C);

			logger::info("before something stupid {:08X}", style->pad1C);

			if (owner->CacheIsValid(style) == false)
			{
				logger::debug("Removing Cache for {:08X} (from {:08X})", owner->_id, _id);
				owner->RemoveCache(owner->currentStyle);
				//We don't remove it from current because it is still owning.
			}
			
			//removing it's own
			if (owner != this && CacheIsValid(style) == false)
			{
				logger::debug("Removing Cache for {:08X}", _id);
				RemoveCache(currentStyle);
			}

			//Current style gets less important as time goes on.
			currentStyle = style;

			logger::debug("Applying mods for {:08X}", _id);
			ApplyModifiers(currentStyle);

			return style;
		}

		static void callback__SetStyleModifier(const ActorValueGeneratorAPI::ExportSetData& data)
		{
			//Can't do this without a target, cannot use base, just a safe guard, from cannot be nan. Forget what that means.
			if (!data.target || data.av_modifier == ActorValueModifier::kTotal || isnan(data.from) == true) {
				return;
			}
			logger::debug("a1 {}", data.av_name);
			CombatStyleInput input = StringToCombatStyleInput(data.av_name);
			
			if (input == CombatStyleInput::Total) {
				return;
			}
	
			logger::debug("b");

			FightingStyle* npc_style = GetCreateStorage(data.target);

			logger::debug("c");

			float mod_value = data.to - data.from;

			switch (data.av_modifier)
			{
			case ActorValueModifier::kDamage:
				logger::debug("d1");
				npc_style->DamageStyle(input, mod_value);
				break;

			case ActorValueModifier::kPermanent:
			case ActorValueModifier::kTemporary:
				logger::debug("d2 {}({}/{})", mod_value, data.to, data.from);
				npc_style->ModifyStyle(input, mod_value);
				break;
			}
			logger::debug("e");
		}

		static float callback__GetCombatValue(Arthmetic::Target self, const Arthmetic::ArgumentList args)
		{
			//Params: String name, Number input type.

			RE::Actor* actor = self ? self->As<RE::Actor>() : nullptr;

			if (!actor) {
				//This should thrown an exception when accessed like this, but I never set it up because I'm a bad programmer.
				// use exceptions kids.
				return 0;
			}


			FightingStyle* npc_style = GetStorage(actor);

			std::string ev_name = args[0]->AsString();
			
			StyleValueMask value_mask = (StyleValueMask)args[1]->AsNumber();

			CombatStyleInput input = StringToCombatStyleInput(ev_name);


			if (input == CombatStyleInput::Total) {
				return 0;
			}



			float result = 0;
			
			//This only needs to be gotten if we're getting base, make sure to check the value mask for that.
			RE::TESCombatStyle* style = GetCombatStyle(actor);

			if (style && npc_style && npc_style->currentStyle && npc_style->currentStyle != style)
			{
				npc_style->SetCombatStyle(style);
			}
			//npc_style->currentStyle
			//If the current combat style and the stored one are different, this needs to update it before using it.

			logger::debug("mask {}", value_mask);
			cycle_switch(value_mask)
			{
			case StyleValueMask::Base:
			if (style)
			{
				if (style->pad1C == NULL)
				{
					//This should actually update get the currentStyle if it's not there, it doesn't have to update, but you know.
					result += GetCombatStyleValue(style, input);
				}
				else
				{
					//If this is owned, we do this
					FightingStyle* owner = (npc_style && style->pad1C == npc_style->_id) ? npc_style : GetStorage(style->pad1C);

					result += owner->GetOriginalValue(input, style);
				}
				logger::info("Base");
				
			}
			break;

			case StyleValueMask::Modifier:
			if (npc_style)
			{
				logger::debug("Mod {}", npc_style->modifierMap.size());
				auto it = npc_style->modifierMap.find(input);
				
				if (npc_style->modifierMap.end() != it) {
					logger::debug("Mod-");
					result += it->second.value;
				}
				
			}
			break;

			case StyleValueMask::Damage:
			if (npc_style)
			{
				//Damage isn't being done somehow, despite technically supposed to be loaded?
				logger::info("Dam");
				auto it = npc_style->modifierMap.find(input);

				if (npc_style->modifierMap.end() != it)
					result += it->second.damage;

				
			}
			break;

			default:
				break;
			}

			return result;
		}

		static void Init1()
		{
			//Do in post loaded
			ActorValueGeneratorAPI::Interface->RegisterExportFunction("SetStyleModifier", callback__SetStyleModifier);
		}

		static void Init2()
		{
			//do in data loaded. Currently, this is cursed.
			ArithmeticAPI::Interface->LinkNativeFunction("GetCombatValue", callback__GetCombatValue);
		}
	};
}