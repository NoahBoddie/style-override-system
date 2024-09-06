#pragma once

#include "SettingsManager.h"
#include "CombatValue.h"

namespace SOS
{
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
				temp_style->pad1B = SettingsManager::GetUpdateTag();
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



		//Cut off.

		static float* GetCombatValueSafe(RE::TESCombatStyle* style, CombatValue input)
		{

			if (style)
			{

				switch (input)
				{
				case CombatValue::offensiveMult:
					return &style->generalData.offensiveMult;
				case CombatValue::defensiveMult:
					return &style->generalData.defensiveMult;
				case CombatValue::groupOffensiveMult:
					return &style->generalData.groupOffensiveMult;
				case CombatValue::avoidThreatChance:
					return &style->generalData.avoidThreatChance;
				case CombatValue::meleeScoreMult:
					return &style->generalData.meleeScoreMult;
				case CombatValue::magicScoreMult:
					return &style->generalData.magicScoreMult;
				case CombatValue::rangedScoreMult:
					return &style->generalData.rangedScoreMult;
				case CombatValue::shoutScoreMult:
					return &style->generalData.shoutScoreMult;
				case CombatValue::unarmedScoreMult:
					return &style->generalData.unarmedScoreMult;
				case CombatValue::staffScoreMult:
					return &style->generalData.staffScoreMult;


				case CombatValue::hoverChance:
					return &style->flightData.hoverChance;
				case CombatValue::diveBombChance:
					return &style->flightData.diveBombChance;
				case CombatValue::groundAttackChance:
					return &style->flightData.groundAttackChance;
				case CombatValue::hoverTimeMult:
					return &style->flightData.hoverTimeMult;
				case CombatValue::groundAttackTimeMult:
					return &style->flightData.groundAttackTimeMult;
				case CombatValue::perchAttackChance:
					return &style->flightData.perchAttackChance;
				case CombatValue::perchAttackTimeMult:
					return &style->flightData.perchAttackTimeMult;
				case CombatValue::flyingAttackChance:
					return &style->flightData.flyingAttackChance;

				case CombatValue::strafeMult:
					return &style->longRangeData.strafeMult;

				case CombatValue::circleMult:
					return &style->closeRangeData.circleMult;
				case CombatValue::fallbackMult:
					return &style->closeRangeData.fallbackMult;
				case CombatValue::flankDistanceMult:
					return &style->closeRangeData.flankDistanceMult;
				case CombatValue::stalkTimeMult:
					return &style->closeRangeData.stalkTimeMult;


				case CombatValue::specialAttackMult:
					return &style->meleeData.specialAttackMult;
				case CombatValue::attackIncapacitatedMult:
					return &style->meleeData.attackIncapacitatedMult;
				case CombatValue::powerAttackIncapacitatedMult:
					return &style->meleeData.powerAttackIncapacitatedMult;
				case CombatValue::powerAttackBlockingMult:
					return &style->meleeData.powerAttackBlockingMult;
				case CombatValue::bashMult:
					return &style->meleeData.bashMult;
				case CombatValue::bashRecoilMult:
					return &style->meleeData.bashRecoilMult;
				case CombatValue::bashAttackMult:
					return &style->meleeData.bashAttackMult;
				case CombatValue::bashPowerAttackMult:
					return &style->meleeData.bashPowerAttackMult;
				}
			}

			return nullptr;
		}


		static float& GetCombatValueUnsafe(RE::TESCombatStyle* style, CombatValue input) { return *GetCombatValueSafe(style, input); }


		static float GetCombatValue(RE::TESCombatStyle* style, CombatValue input)
		{
			auto result = GetCombatValueSafe(style, input);

			return result ? *result : 0;
		}

		
		static RE::TESCombatStyle* GetCombatStyle(RE::Actor* a_this)
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

	private:
		//Alt version of this function that sticks close to the psuedo code.
		static RE::TESCombatStyle* GetCombatStyleRaw(RE::Character* a_this)
		{
			//This doesn't respect the read write lock tho do it?
			auto extra_style = a_this->extraList.GetByType<RE::ExtraCombatStyle>();

			auto result = extra_style ? extra_style->combatStyle : nullptr;
			if (!result)
			{
				auto Process_F0 = a_this->GetActorRuntimeData().currentProcess;
				if (!Process_F0)
				{
					goto base;
				}
				auto Package_8 = Process_F0->currentPackage.package;
				if (!Package_8)
				{
					goto base;
				}
				result = Package_8->combatStyle;
				if (!result)
				{
					goto base;
				}
			}
			return result;

		base:
			RE::TESNPC* npc_base = skyrim_cast<RE::TESNPC*>(a_this->GetBaseObject());

			if (npc_base) {
				return npc_base->combatStyle;
			}
			return nullptr;
		}


		//This should remain largely unused due to the manual nature of some of these.
		static void ModifyCombatStyle(RE::TESCombatStyle* style, CombatValue input, float value)
		{
			//should I thread lock this function?

			if (!style || value == 0 || isnan(value) == true)
				return;

			float& style_value = GetCombatValueUnsafe(style, input);

			style_value += value;
		}

	};
}