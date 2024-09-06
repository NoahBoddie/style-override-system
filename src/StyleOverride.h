#pragma once

#include "CombatStyleManager.h"
#include "SettingsManager.h"
#include "CombatValue.h"
#include "ValueMask.h"
#include "Types.h"

#include "Serialization/SerializableMap.h"
#include "Serialization/SerializationTypePlayground.h"

namespace SOS
{

	struct StyleOverride;
	
	template <class T, class S = RGL::DefaultSerialize>
	using FormMap = RGL::SerializableMap<RGL::SerialFormID, T, S, S>;



	struct StyleOverride : public RGL::SerializationHandler
	{

		struct SerializeClass
		{
			void operator()(std::pair<const RGL::SerialFormID, StyleOverride*>& entry, RGL::SerialArgument& serializer, bool& success)
			{
				success = serializer.Serialize(entry.first);//Needs to be a particular type of object, serializable formID


				bool is_deserializing = serializer.IsDeserializing();

				if (success && is_deserializing == true) {
					RE::Actor* actor = nullptr;
					if (success) {
						actor = RE::TESForm::LookupByID<RE::Actor>(entry.first);

						if (!actor) {
							logger::error("Actor FormID {:08X} invalid, dumping.", static_cast<RE::FormID>(entry.first));
							success = false;
						}
						else {
							logger::info("Actor {}(FormID:{:08X}) successful, creating and deserializing.", actor->GetName(), static_cast<RE::FormID>(entry.first));
							entry.second = new StyleOverride(actor);
						}
					}
					else
					{
						logger::error("Failure deserializing form ID.");
					}
				}

				//If the pointer is null or the success is false, it will dump the data, and return unsuccessful.
				success = serializer.DumpIfFailure(entry.second, success);

				if (success)
					logger::info("serialized: {:08X} at {:08X}", static_cast<RE::FormID>(entry.first), (uint64_t)entry.second);
				else {
					logger::warn("failed to de/serialize, data may have been trival.");
					
					if (entry.second) {
						delete entry.second;

						entry.second = nullptr;
					}
				}
			}

			void operator()(std::map<RGL::SerialFormID, StyleOverride*>&)
			{
				logger::debug("Reverting extra value storage");
				RemoveAllOverrides();
			}
		};


	private:
		//May make these unique, and only give out regular pointers.
		//inline static EVStorageMap& _valueTable = Initializer<EVStorageMap, PrimaryRecordType>(HandlePrimarySerializer, PrimaryRecordType::ExtraValueStorage);
		inline static auto& _overrideMap = RGL::SerializationHandler::CreatePrimarySerializer<FormMap<StyleOverride*, SerializeClass>>(RGL::PrimaryRecordType::StyleOverride);
		//inline static std::map<RE::FormID, ExtraValueStorage*>* _valueTable = nullptr;

	public:




		struct ValueModifier
		{
			float value;
			float damage;
		};



	public:

		static void RemoveAllOverrides()
		{
			for (auto& [key, ptr] : *_overrideMap)
			{
				delete ptr;
			}

			_overrideMap->clear();
		}


		static StyleOverride* GetOverride(RE::FormID id)
		{
			//This won't be used often, but it's used in situations where it would be ok to not create storage yet.
			if (!id) {
				return nullptr;
			}

			//if (actor->IsPlayerRef() == true) {
			//	return nullptr;
			//}
			auto result = _overrideMap->find(id);

			return result == _overrideMap->end() ? nullptr : result->second;
		}

		static StyleOverride* GetOverride(RE::Actor* actor)
		{
			//This won't be used often, but it's used in situations where it would be ok to not create storage yet.
			if (!actor) {
				return nullptr;
			}

			//if (actor->IsPlayerRef() == true) {
			//	return nullptr;
			//}
			auto result = _overrideMap->find(actor->formID);

			return result == _overrideMap->end() ? nullptr : result->second;
		}


		static StyleOverride* GetCreateOverride(RE::Actor* actor)
		{
			//Should this be thead locked? I feel like this should be thread locked

			if (!actor) {
				logger::critical("No actor detected, terminating storage search.");
				return nullptr;  //Just crash, this isn't supposed to be found.
			}

			if (actor->IsPlayerRef() == true) {
				return nullptr;
			}

			StyleOverride*& storage_spot = _overrideMap[actor->formID];

			if (!storage_spot) {
				//logger::warn("Stor loc {}", (uintptr_t)storage_spot);
				storage_spot = new StyleOverride(actor->formID);
			}


			return storage_spot;
		}


		static bool RemoveOverride(RE::FormID _id)
		{
			if (!_id || _overrideMap->contains(_id) == false)
				return false;


			if (_id == 0x14) {
				//logger::debug("PlayerCharacter does not have a combat style to process.");
				return false;

			}

			//needs an initializer part
			//It will need to search the left hand, the right hand
			logger::info("[Unregister {:08X} ]", _id);


			StyleOverride* _override = _overrideMap[_id];

			//LOCK while removing

			_overrideMap->erase(_id);

			delete _override;

			return true;
		}


		~StyleOverride()
		{
			//This needs to remove the style
			ReleaseTempStyle();
		}


		void HandleSerialize(RGL::SerialArgument& buffer, bool& success) override
		{
			buffer.Serialize(modifierMap);
		}


		bool IsRelevant()
		{
			if (modifierMap->empty() == true) {
				logger::debug("ModifierMap for {:08X} is empty.", _id);
				return false;
			}
			else
			{
				logger::debug("ModifierMap for {:08X} is relevant.", _id);
			}

			return true;
		}

		//This will likely delete relatively often, so delete is likely for the best.
		RGL::SerializableMap<CombatValue, ValueModifier> modifierMap;

		//When an update to a combat style goes through, this will check the left value for equivalency. If any value isn't the same,
		// all old modifiers stored in the right side will be removed from the combat map.
		//std::unordered_map<CombatValue, StyleCache> modifierCache;

		RE::FormID _id{};
		RE::TESCombatStyle* tempStyle{ nullptr };
		RE::TESCombatStyle* currentStyle{ nullptr };

		bool modifierFlag = false;//If the modifiers have changed, and this needs to be used, this is used.

		StyleOverride(RE::FormID i) : _id{ i } { logger::debug("Container Address for {:08X}: {:X}", i, (uintptr_t)this); }
		StyleOverride(RE::Actor* a) : _id{ a ? a->formID : 0 } {}

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

			style->pad1B = SettingsManager::GetUpdateTag();
			style->pad1C = _id;

			ValueModifier* flank_mod = nullptr;
			ValueModifier* duel_mod = nullptr;

			ValueModifier* single_mod = nullptr;
			ValueModifier* dual_mod = nullptr;

			for (auto& [input, modifier] : *modifierMap)
			{

				switch (input)
				{
				case CombatValue::FlankingStyle:
					flank_mod = &modifier;
					continue;

				case CombatValue::DuelingStyle:
					duel_mod = &modifier;
					continue;

				case CombatValue::SingleWielding:
					single_mod = &modifier;
					continue;

				case CombatValue::DualWielding:
					dual_mod = &modifier;
					continue;

				}

				float* v = CombatStyleManager::GetCombatValueSafe(style, input);

				if (!v) {
					continue;
				}

				float& value = *v;

				float max = SettingsManager::styleSettingsArray[input].GetValue();

				//This needs to be a clamp rather than a min.
				float mod = modifier.value + modifier.damage;

				//float base = fmin(modifier.value - modifier.damage, max - value);

				//Raw value goes first, then we place damage.
				//base -= modifier.damage;

				logger::debug("Mod '{}' from {} to {}(unclamped) on {:08X}",
					CombatValueToString(input), value, value + mod, _id);
				value += mod;

				value = std::clamp(value, 0.f, max);
				//modifierCache[input] = StyleCache(value, base);
			}

			float flank_style = flank_mod ? (flank_mod->value + flank_mod->damage) : 0;
			float duel_style = duel_mod ? (duel_mod->value + duel_mod->damage) : 0;


			float single_wield = single_mod ? (single_mod->value + single_mod->damage) : 0;
			float dual_wield = dual_mod ? (dual_mod->value + dual_mod->damage) : 0;

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
					//style->formFlags |= (uint32_t)RE::TESCombatStyle::RecordFlags::kAllowDualWielding;
					style->flags |= RE::TESCombatStyle::FLAG::kAllowDualWielding;
				}
				else if (dual_wield < single_wield) {
					//style->formFlags &= ~(uint32_t)RE::TESCombatStyle::RecordFlags::kAllowDualWielding;
					style->flags &= ~RE::TESCombatStyle::FLAG::kAllowDualWielding;
				}

				//If both are equal, it jsut does the original.
			}
		}

		void TryDestroyModifier(CombatValue input, ValueModifier& modifier)
		{
			//Note, this must own this to destroy it.
			if (!modifier.value && !modifier.damage)
				modifierMap->erase(input);
		}
		//Later, I'd like this to do get. I would also like to use the mask instead.
		float GetStyleModifer(CombatValue input, bool is_damage, RE::TESCombatStyle* style = nullptr)
		{
			auto it = modifierMap->find(input);

			if (modifierMap->end() != it) {
				return is_damage ? it->second.damage : it->second.value;
			}

			return 0;
		}

		//This is't functioning right I don't think, doesn't account for if it doesn't own it.
		// Would like to redo this.
		//Also, is deprecated
		float GetOriginalValue(CombatValue input, RE::TESCombatStyle* style = nullptr)
		{
			//This might need to get the original combatstyle of the person

			if (!style)
				style = currentStyle;

			float current_value = CombatStyleManager::GetCombatValue(style, input);

			return current_value;

			float mod_value = 0;

			//auto it = modifierCache.find(input);

			//if (modifierCache.end() != it) {
			//	mod_value = it->second.modifier;
			//}

			return current_value - mod_value;
		}

		void ModifyStyle(CombatValue input, float value)
		{
			//should I thread lock this function?
			if (input == CombatValue::Total || value == 0 || isnan(value) == true)
				return;

			ValueModifier& modifier = modifierMap[input];

			modifier.value += value;

			logger::debug("{} {}", (int)input, modifier.value);

			modifierFlag = true;

			TryDestroyModifier(input, modifier);
		}

		void DamageStyle(CombatValue input, float value)
		{
			//should I thread lock this function?

			if (value == 0 || isnan(value) == true)
				return;
			ValueModifier& modifier = modifierMap[input];

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


			StyleOverride* owner = GetOverride(temp_style->pad1C);

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
			if (modifierMap->empty())
				return style;

			logger::debug("{:08X} using thread style {:08X}", _id, thr_style->formID);

			if (!tempStyle || tempStyle->pad1C != _id || tempStyle->pad1B == 255) {
				//Encorporate revert.
				StyleOverride* owner = !thr_style->pad1C || thr_style->pad1C == _id ? this : GetOverride(thr_style->pad1C);
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

							//NEW
							NULL_OP(controller->inventory)->dirty = true;

							skip_release = true;
						}
					}
					if (!skip_release)
						owner->ReleaseTempStyle();

					//We don't remove it from current because it is still owning.
				}
			}
			else { thr_style = tempStyle; }

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
			if (!data.target || data.av_modifier == uActorValueModifier::kTotal || isnan(data.from) == true) {
				return;
			}

			if (data.target->IsPlayerRef() == true)
				return;

			logger::debug("a1 {}", data.av_name);
			CombatValue input = StringToCombatValue(data.av_name);

			if (input == CombatValue::Total) {
				return;
			}

			logger::debug("b");

			StyleOverride* npc_style = GetCreateOverride(data.target);

			logger::debug("c");

			float mod_value = data.to - data.from;

			switch (data.av_modifier)
			{
			case uActorValueModifier::kDamage:
				logger::debug("d1");
				npc_style->DamageStyle(input, mod_value);
				break;

			case uActorValueModifier::kPermanent:
			case uActorValueModifier::kTemporary:
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

			StyleOverride* npc_style = GetOverride(actor);

			std::string ev_name = args[0]->AsString();

			StyleValueMask value_mask = (StyleValueMask)args[1]->AsNumber();

			CombatValue input = StringToCombatValue(ev_name);


			if (input == CombatValue::Total) {
				return 0;
			}



			float result = 0;

			//This only needs to be gotten if we're getting base, make sure to check the value mask for that.
			RE::TESCombatStyle* style = CombatStyleManager::GetCombatStyle(actor);

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
					switch (input)
					{
					default:
						result += CombatStyleManager::GetCombatValue(style, input);
						break;


					case CombatValue::DuelingStyle:
						result += !!(style->flags & RE::TESCombatStyle::FLAG::kDuelingStyle);
						break;

					case CombatValue::FlankingStyle:
						result += !!(style->flags & RE::TESCombatStyle::FLAG::kFlankingStyle);
						break;

					case CombatValue::SingleWielding:
						result += !(style->flags & RE::TESCombatStyle::FLAG::kAllowDualWielding) == true;
						//result += !(style->GetFormFlags() & RE::TESCombatStyle::RecordFlags::kAllowDualWielding) == true;
						break;

					case CombatValue::DualWielding:
						result += !(style->flags & RE::TESCombatStyle::FLAG::kAllowDualWielding) == false;
						//result += !(style->GetFormFlags() & RE::TESCombatStyle::RecordFlags::kAllowDualWielding) == false;
						break;

					/*
					case CombatValue::DuelingStyle:
						result += style->flags.any(RE::TESCombatStyle::FLAG::kDuelingStyle);
						break;

					case CombatValue::FlankingStyle:
						result += style->flags.any(RE::TESCombatStyle::FLAG::kFlankingStyle);
						break;

					case CombatValue::SingleWielding:
						result += style->flags.any(RE::TESCombatStyle::FLAG::kAllowDualWielding) == true;
						break;

					case CombatValue::DualWielding:
						result += style->flags.any(RE::TESCombatStyle::FLAG::kAllowDualWielding) == false;
						break;
					//*/
					}

					logger::info("Base");
				}
				break;

			case StyleValueMask::Modifier:
				if (npc_style)
				{
					logger::debug("Mod {}", npc_style->modifierMap->size());
					auto it = npc_style->modifierMap->find(input);

					if (npc_style->modifierMap->end() != it) {
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
					auto it = npc_style->modifierMap->find(input);

					if (npc_style->modifierMap->end() != it)
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