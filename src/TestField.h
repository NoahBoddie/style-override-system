#pragma once

#include "Utility.h"

#include "SettingsManager.h"

//#define PREFIX(content) CONCAT(STRINGIZE(fCS_), STRINGIZE(content))
//constexpr const char* prefix = PREFIX(Tet);
namespace RE
{
	class FloatSetting
	{
	private:
		uintptr_t _vtable = RE::VTABLE_Setting[0].address(); //0x00

	public:
		float			currValue;	// 0x08
		mutable float	prevValue;	// 0x0C
		const char*		name;		// 0x10

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
		SingleWielding = CombatStyleInput::ValueTotal,
		DualWielding,
		DuelingStyle,
		FlankingStyle,
		//Another divide might happen here for custom values. Make sure to watch it.
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

	std::string_view CombatStyleInputToString(CombatStyleInput input)
	{
		if (input < 0 || input > CombatStyleInput::Total)
			return "Total";

		return extraValueNameArray[input];
	}

	
	//I'll keep them like this, because it would be a pain to do it otherwise.
	std::array<RE::FloatSetting, CombatStyleInput::ValueTotal> styleSettingsArray
	{
		RE::FloatSetting("fCS_offensiveMultMax", 1.0f),
		RE::FloatSetting("fCS_defensiveMultMax", 1.0f),
		RE::FloatSetting("fCS_groupOffensiveMultMax", 1.0f),
		RE::FloatSetting("fCS_avoidThreatChanceMax", 1.0f),
		RE::FloatSetting("fCS_staffScoreMultMax", 10.f),
		RE::FloatSetting("fCS_hoverChanceMax", 1.0f),
		RE::FloatSetting("fCS_diveBombChanceMax", 1.0f),
		RE::FloatSetting("fCS_groundAttackChanceMax", 1.0f),
		RE::FloatSetting("fCS_hoverTimeMultMax", 1.0f),
		RE::FloatSetting("fCS_groundAttackTimeMultMax", 1.0f),
		RE::FloatSetting("fCS_perchAttackChanceMax", 1.0f),
		RE::FloatSetting("fCS_perchAttackTimeMultMax", 1.0f),
		RE::FloatSetting("fCS_flyingAttackChanceMax", 1.0f),
		RE::FloatSetting("fCS_strafeMultMax", 1.0f),
		RE::FloatSetting("fCS_circleMultMax", 1.0f),
		RE::FloatSetting("fCS_fallbackMultMax", 1.0f),
		RE::FloatSetting("fCS_flankDistanceMultMax", 1.0f),
		RE::FloatSetting("fCS_stalkTimeMultMax", 1.0f),
		RE::FloatSetting("fCS_specialAttackMultMax", 1.0f),

		RE::FloatSetting("fCS_meleeScoreMultMax", 10.f),
		RE::FloatSetting("fCS_magicScoreMultMax", 10.f),
		RE::FloatSetting("fCS_rangedScoreMultMax", 10.f),
		RE::FloatSetting("fCS_shoutScoreMultMax", 10.f),
		RE::FloatSetting("fCS_unarmedScoreMultMax", 10.f),
		RE::FloatSetting("fCS_attackIncapacitatedMultMax", 10.f),
		RE::FloatSetting("fCS_powerAttackIncapacitatedMultMax", 10.f),
		RE::FloatSetting("fCS_powerAttackBlockingMultMax", 10.f),
		RE::FloatSetting("fCS_bashMultMax", 10.f),
		RE::FloatSetting("fCS_bashRecoilMultMax", 10.f),
		RE::FloatSetting("fCS_bashAttackMultMax", 10.f),
		RE::FloatSetting("fCS_bashPowerAttackMultMax", 10.f)
	};
	//if the small bits in the combat style aren't the same as this, it will purge it's values
	uint8_t settingUpdateTag = 1;

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
	
	void SettingInit()
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
		struct ReleaseCombatStyle
		{
			constexpr ReleaseCombatStyle() noexcept = default;

			//template <class _Uty, std::enable_if_t<std::is_convertible_v<_Uty(*), RE::TESCombatStyle(*)>, int> = 0>
			//constexpr ReleaseCombatStyle(const default_delete<_Uty[]>&) noexcept {}

			constexpr void operator()(RE::TESCombatStyle* _Ptr) const noexcept {
				//This resubmits this back to the thread_queue instead of deleting it.
				if (_Ptr)
					thread_queue.push(_Ptr);
				//std::default_delete<int> f;
			}
		};

		//I see a max of 7 threads, so I'm gonna make it 8 instead. 10 to be safe
		constexpr static size_t thread_size = 10;
		constexpr static size_t trans_size = 8;

		//If this idea works out, this shall not be stored, nor shall it be saved. It will only be copied to and then removed.
		// Hopefully, it shant make it's way into papyrus, but we'll see.
	
		thread_local inline static std::unique_ptr<RE::TESCombatStyle, ReleaseCombatStyle> threadLocalStyle;
		
		inline static std::queue<RE::TESCombatStyle*> thread_queue;
		inline static std::queue<RE::TESCombatStyle*> temp_queue;//Maybe just lump it all in with thread queue?

		inline static std::mutex _mutex;
		//might move this
		static constexpr size_t combatStyleDataLength = 0x80;//0x7C


		static void TransferData(RE::TESCombatStyle* real_style, RE::TESCombatStyle* temp_style)
		{
			//Needs to be some rules to make sure each is each.
			if (!real_style || !temp_style)
				return;

			
			std::memcpy(&temp_style->generalData, &real_style->generalData, combatStyleDataLength);
			
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
			
			if (!style_1 || !style_2 || std::memcmp(&style_1->generalData, &style_2->generalData, combatStyleDataLength) != 0)
				return false;
			else
				return true;
		}

		static RE::TESCombatStyle* RequestThreadStyle()
		{
			if (!threadLocalStyle)
			{
				//We lock here.

				//do the front check here btw
				
				//Also, if we're out of styles, just make another one.
				threadLocalStyle = decltype(threadLocalStyle){ thread_queue.front() };

				

				static bool once = false;
				if (!threadLocalStyle)
				{
					if (!once) {
						logger::critical("Style request failed, no more temp styles.");
						once = true;
					}
				}
				else
				{
					logger::debug("Style {:08X} associated to thread {}", threadLocalStyle->formID, GetThreadID());
				}
				

				if (thread_queue.empty() == false)
					thread_queue.pop();
			}

			return threadLocalStyle.get();
		}


		static RE::TESCombatStyle* RequestTemporaryStyle()
		{
			auto result = temp_queue.front();
			temp_queue.pop();
			logger::debug("TempStyle {:08X} requested on thread {}. {} remaining.", result->formID, GetThreadID(), temp_queue.size());
			return result;
		}


		static void ReleaseTemporaryStyle(RE::TESCombatStyle* style)
		{
			if (!style) {
				logger::debug("Couldn't release");
				return;
			}

			temp_queue.push(style);
		
			logger::debug("TempStyle {:08X} released on thread {}. {} remaining.", style->formID, GetThreadID(), temp_queue.size());
		}


		static void Init()
		{			
			auto* factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::TESCombatStyle>();

			for (size_t i = 0; i < thread_size; i++)
			{
				RE::TESCombatStyle* temp_style = factory->Create();
				temp_style->pad1B = settingUpdateTag;
				logger::info("Threaded CombatStyle {:08X} created.", temp_style->formID);
				thread_queue.push(temp_style);
			}

			for (size_t i = 0; i < trans_size; i++)
			{
				RE::TESCombatStyle* temp_style = factory->Create();
				temp_style->pad1B = 255;
				logger::info("Temp CombatStyle {:08X} created.", temp_style->formID);
				temp_queue.push(temp_style);
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

		if (auto test_null_op = NULL_OP(NULL_OP(a_this)->GetActorRuntimeData().currentProcess)->currentPackage.package; test_null_op)
			logger::info("Null op success, form {}", test_null_op->formID);

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


		~FightingStyle()
		{
			//This needs to remove the style
			ReleaseTempStyle();
		}

		//This will likely delete relatively often, so delete is likely for the best.
		std::map<CombatStyleInput, StyleModifier> modifierMap;

		//When an update to a combat style goes through, this will check the left value for equivalency. If any value isn't the same,
		// all old modifiers stored in the right side will be removed from the combat map.
		//std::unordered_map<CombatStyleInput, StyleCache> modifierCache;

		RE::FormID _id{};
		RE::TESCombatStyle* tempStyle{ nullptr };
		RE::TESCombatStyle* currentStyle{ nullptr };

		bool modifierFlag = false;//If the modifiers have changed, and this needs to be used, this is used.

		FightingStyle(RE::FormID i) : _id{ i } { logger::debug("Container Address for {:08X}: {:X}",i , (uintptr_t)this); }
		FightingStyle(RE::Actor* a) : _id{ a ? a->formID : 0 } {}

		//This checks only if it needs to remove itself from the given function.
		bool CacheIsValid(RE::TESCombatStyle* style)
		{
			
			return false;

			//Needs a name change, and also a function change. For now, all of this works.

			if (style)
			{
				if (style == currentStyle)
				{
					//for (auto& [input, cache] : modifierCache)
					//{
					//	float* value = GetCombatStyleValueSafe(style, input);
					//	if (value && *value == cache.value) {
					//		return false;
					//	}
					//}

					//return true;
				}
			}
			
			return false;
		}



		//These should have something for the flag values too.
		void ReleaseTempStyle()
		{
			if (tempStyle && tempStyle->pad1C == _id)
			{
				
				//for (auto& [input, cache] : modifierCache) {
				//	ModifyCombatStyle(style, input, -cache.modifier);
				//}

				tempStyle->pad1C = NULL;

				if (tempStyle->pad1B == 255) 
				{
					CombatStyleManager::ReleaseTemporaryStyle(tempStyle);
				}
				
				tempStyle = nullptr;
			}

			//Clears either way
			//modifierCache.clear();
		}
		

		void ApplyModifiers(RE::TESCombatStyle* style)
		{
			if (!style)
				return;
			
			style->pad1B = settingUpdateTag;
			style->pad1C = _id;

			StyleModifier* flank_mod = nullptr;
			StyleModifier* duel_mod = nullptr;
			
			StyleModifier* single_mod = nullptr;
			StyleModifier* dual_mod = nullptr;

			for (auto& [input, modifier] : modifierMap)
			{
				
				switch (input)
				{
				case CombatStyleInput::FlankingStyle:
					flank_mod = &modifier;
					continue;

				case CombatStyleInput::DuelingStyle:
					duel_mod = &modifier;
					continue;

				case CombatStyleInput::SingleWielding:
					single_mod = &modifier;
					continue;

				case CombatStyleInput::DualWielding:
					dual_mod = &modifier;
					continue;

				}

				float* v = GetCombatStyleValueSafe(style, input);

				if (!v) {
					continue;
				}

				float& value = *v;

				float max = styleSettingsArray[input].GetValue();

				//This needs to be a clamp rather than a min.
				float mod = modifier.value - modifier.damage;

				//float base = fmin(modifier.value - modifier.damage, max - value);

				//Raw value goes first, then we place damage.
				//base -= modifier.damage;

				logger::debug("Mod '{}' from {} to {}(unclamped) on {:08X}",
					CombatStyleInputToString(input), value, value + mod, _id);
				value += mod;

				value = std::clamp(value, 0.f, max);
				//modifierCache[input] = StyleCache(value, base);
			}

			float flank_style = flank_mod ? (flank_mod->value - flank_mod->damage) : 0;
			float duel_style = duel_mod ? (duel_mod->value - duel_mod->damage) : 0;


			float single_wield = single_mod ? (single_mod->value - single_mod->damage) : 0;
			float dual_wield = dual_mod ? (dual_mod->value - dual_mod->damage) : 0;

			//If both are below, it won't do anything.
			if (flank_style > 0 || duel_style > 0)
			{
				if (flank_style > duel_style) {
					style->flags &= ~RE::TESCombatStyle::FLAG::kDuelingStyle;
					style->flags |= RE::TESCombatStyle::FLAG::kFlankingStyle;
				}
				else if (flank_style < duel_style) {
					style->flags &= ~RE::TESCombatStyle::FLAG::kFlankingStyle;
					style->flags |= RE::TESCombatStyle::FLAG::kDuelingStyle;
				}
					
				//If both are equal, it jsut does the original.
			}

			if (dual_wield > 0 || single_wield > 0)
			{
				if (dual_wield > single_wield) {
					style->flags |= RE::TESCombatStyle::FLAG::kAllowDualWielding;
				}
				else if (dual_wield < single_wield) {
					style->flags &= ~RE::TESCombatStyle::FLAG::kAllowDualWielding;
				}

				//If both are equal, it jsut does the original.
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
		//Also, is deprecated
		float GetOriginalValue(CombatStyleInput input, RE::TESCombatStyle* style = nullptr)
		{
			//This might need to get the original combatstyle of the person

			if (!style)
				style = currentStyle;

			float current_value = GetCombatStyleValue(style, input);

			return current_value;

			float mod_value = 0;

			//auto it = modifierCache.find(input);

			//if (modifierCache.end() != it) {
			//	mod_value = it->second.modifier;
			//}

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

			modifierFlag = true;

			TryDestroyModifier(input, modifier);
		}

		void DamageStyle(CombatStyleInput input, float value)
		{
			//should I thread lock this function?

			if (value == 0 || isnan(value) == true)
				return;
			StyleModifier& modifier = modifierMap[input];

			modifier.damage += value;

			modifierFlag = true;

			TryDestroyModifier(input, modifier);
		}

		inline static std::mutex style_lock;

		//Make one of these for fighting styles too btw.
		static void RevertStyle(RE::TESCombatStyle* temp_style)
		{
			std::lock_guard<std::mutex> lock{ style_lock };

			if (!temp_style || !temp_style->pad1C)
				return;
			

			FightingStyle* owner = GetStorage(temp_style->pad1C);
			
			logger::info("before something stupid (again) {:08X}", temp_style->pad1C);


			if (owner->CacheIsValid(temp_style) == false)
			{
				logger::debug("Reverting TempStyle for {:08X}", owner->_id);
				owner->ReleaseTempStyle();
			}

		}

		RE::TESCombatStyle* SetCombatStyle(RE::TESCombatStyle* style)
		{
			if (!style)
				return style;

			std::lock_guard<std::mutex> lock{ style_lock };
			//Lock this function

			//checks for 2 things, is this ours, and are these values what our cache says it is.
			
			RE::TESCombatStyle* thr_style = CombatStyleManager::RequestThreadStyle();
			
			if (!thr_style)
				return style;

			//If we have no modifiers, no need to keep these up.
			if (modifierMap.empty())
				return style;

			logger::debug("{:08X} using thread style {:08X}", _id, thr_style->formID);

			if (!tempStyle || tempStyle->pad1C != _id || tempStyle->pad1B == 255){
				//Encorporate revert.
				FightingStyle* owner = !thr_style->pad1C || thr_style->pad1C == _id ? this : GetStorage(thr_style->pad1C);
				bool is_owner = owner == this;

				logger::info("before something stupid {:08X}, {}", style->pad1C, is_owner);

				//removing it's own
				if (!is_owner && CacheIsValid(style) == false)
				{
					logger::debug("Releasing temp for {:08X}, style {:08X}, id {}", _id, NULL_OP(tempStyle)->formID, NULL_OP(tempStyle)->pad1B);
					ReleaseTempStyle();

				}

				if (owner->CacheIsValid(style) == false)
				{
					logger::debug("Releasing temp for {:08X} (from {:08X})", owner->_id, _id);

					bool skip_release = false;

					if (!is_owner)
					{
						RE::Actor* actor = RE::TESForm::LookupByID<RE::Actor>(owner->_id);

						auto controller = !actor ? nullptr : actor->GetActorRuntimeData().combatController;


						if (controller)
						{
							auto temp_style = CombatStyleManager::RequestTemporaryStyle();

							CombatStyleManager::TransferData(thr_style, temp_style);

							logger::debug("Releasing temp FROM OTHER {:08X}, style {:08X}, id {}", owner->_id, owner->tempStyle ? owner->tempStyle->formID : 0, owner->tempStyle ? owner->tempStyle->pad1B : 0);
							owner->ReleaseTempStyle();

							temp_style->pad1C = owner->_id;
							owner->tempStyle = temp_style;
							controller->combatStyle = temp_style;

							skip_release = true;
						}
					}
					if (!skip_release)
						owner->ReleaseTempStyle();

					//We don't remove it from current because it is still owning.
				}
			} else{thr_style = tempStyle;}
			
			tempStyle = thr_style;
			currentStyle = style;

			logger::debug("Applying mods for {:08X}", _id);
			
			CombatStyleManager::TransferData(style, thr_style);

			ApplyModifiers(thr_style);

			return thr_style;
		}

		static void callback__SetStyleModifier(const ActorValueGeneratorAPI::ExportSetData& data)
		{
			//Can't do this without a target, cannot use base, just a safe guard, from cannot be nan. Forget what that means.
			if (!data.target || data.av_modifier == ActorValueModifier::kTotal || isnan(data.from) == true) {
				return;
			}

			if (data.target->IsPlayerRef() == true)
				return;

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

			if (actor->IsPlayerRef() == true)
				return 0;

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

			//if (style && npc_style && npc_style->currentStyle && npc_style->currentStyle != style)
			//{
			//	npc_style->SetCombatStyle(style);
			//}
			// 
			//npc_style->currentStyle
			//If the current combat style and the stored one are different, this needs to update it before using it.

			logger::debug("mask {}", value_mask);
			cycle_switch(value_mask)
			{
			case StyleValueMask::Base:
			if (style)
			{
				//This should actually update get the currentStyle if it's not there, it doesn't have to update, but you know.
				result += GetCombatStyleValue(style, input);

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