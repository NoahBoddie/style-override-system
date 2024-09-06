#pragma once

//#include "TestField.h"
#include "Utility.h"
#include "StyleOverride.h"

#include "xbyak/xbyak.h"

namespace SOS
{

	int IsCallOrJump(uintptr_t addr)
	{
		//0x15 0xE8//These are calls, represented by negative numbers
		//0x25 0xE9//These are jumps, represented by positive numbers.
		//And zero represent it being neither.

		if (addr)
		{
			auto first_byte = reinterpret_cast<uint8_t*>(addr);

			switch (*first_byte)
			{
			case 0x15:
			case 0xE8:
				return -1;

			case 0x25:
			case 0xE9:
				return 1;

			}
		}

		return 0;
	}

	RE::TESCombatStyle* GetCombatStyleRaw(RE::Character* a_this)
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
	
	struct Character__GetCombatStyleHook
	{
		static void Patch()
		{
			//Need this to just override at a certain point, no need for this tomfoolery
			auto hook_addr = REL::RelocationID(37586, 38536).address();//SE: 0x623E60, AE: 0x65B780, VR: ???
			auto return_addr = hook_addr + 0x6;
			
			struct Code : Xbyak::CodeGenerator
			{
				Code(uintptr_t ret_addr)
				{
					//Unsure of AE/SE version dif
					push(rbx);
					sub(rsp, 0x20);
					

					mov(rax, ret_addr);
					jmp(rax);
				}
			} static code{ return_addr };

			auto& trampoline = SKSE::GetTrampoline();

			auto placed_call = IsCallOrJump(hook_addr) > 0;

			auto place_query = trampoline.write_branch<5>(hook_addr, (uintptr_t)thunk);

			if (!placed_call)
				func = (uintptr_t)code.getCode();
			else
				func = place_query;

			logger::info("Character::GetCombatStyle Hook complete...");
		}
		//4FE5B0
		inline static REL::Relocation<uintptr_t> exception { REL::RelocationID { 32492, 00 }, 0x8F };

		static RE::TESCombatStyle* thunk(RE::Character* a_this)
		{
			//RE::CombatController

			RE::TESCombatStyle* result = func(a_this);

			//auto excep_addr = exception.address();
			//auto ret_addr = (uintptr_t)_ReturnAddress();
			
			//logger::info("except:{:8X} vs ret:{:8X}", excep_addr, ret_addr);

			if (result)// && ret_addr != excep_addr)
			{
				StyleOverride* style = StyleOverride::GetOverride(a_this);
				
				if (style){
					//I want to make this an out function instead.
					result = style->SetCombatStyle(result);
					if (auto controller = a_this->GetActorRuntimeData().combatController; controller && result) {
						controller->combatStyle = result;
						//NEW
						NULL_OP(controller->inventory)->dirty = true;
					}
				}
				else {
					//FightingStyle::RevertStyle(result);
				}
			}

			return result;
		}

		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct CombatStyleCtorHook
	{
		static void Patch()
		{
			//return;
			// 
			//SE: 0x215150, AE: 0x224A70, VR:???
			REL::Relocation<uintptr_t> InitializeData{ REL::RelocationID { 16912, 418341 }, 0x16 };

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(InitializeData.address(), thunk);

			logger::info("CombatStyleCtorHook Hook complete...");
		}


		static RE::TESForm* thunk(RE::TESForm* a_this)
		{
			//if (!a_this)
			//	goto end;

			a_this->pad1B = 0x00;
			a_this->pad1C = 0x00000000;

		//end:
			return func(a_this);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};


	struct RecalculateLeveledActorHook
	{
		static void Patch()
		{
			//SE: 0x5D57B0, AE: 0x60C1D0, VR:???
			REL::Relocation<uintptr_t> RecalculateLeveledActor{ REL::RelocationID { 36333, 37323 }, 0x51 };

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(RecalculateLeveledActor.address(), thunk);

			logger::info("RecalculateLeveledActor Hook complete...");
		}


		static DWORD thunk(RE::TESObjectREFR* a_this, void* a2, void* a3)
		{
			if (StyleOverride::RemoveOverride(a_this->formID) == true)
				logger::debug("Removing {}'s Override, FormID {:08X}", a_this->GetName(), a_this->formID);

			return func(a_this, a2, a3);
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};


	struct Hooks
	{
		static void Install()
		{
			//return;

#ifdef _DEBUG
			constexpr size_t reserve = 256;
#else
			constexpr size_t reserve = 42;
#endif

			SKSE::AllocTrampoline(reserve);

			Character__GetCombatStyleHook::Patch();
			RecalculateLeveledActorHook::Patch();
			CombatStyleCtorHook::Patch();

			logger::info("Hooks installed.");
		}
	};
}
