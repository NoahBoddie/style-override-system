#pragma once

#include "StyleOverride.h"

namespace SOS
{
	struct EventSingleton : 
		public RE::BSTEventSink<RE::TESCombatEvent>,
		public RE::BSTEventSink<RE::TESResetEvent>,
		//public RE::BSTEventSink<RE::BGSActorDeathEvent>,
		public RE::BSTEventSink<RE::TESFormDeleteEvent>
	{
		static EventSingleton* GetSingleton()
		{
			static EventSingleton singleton;

			return &singleton;
		}


		static void Install()
		{
			//At a later point I would like to break this thing up into it's own set of respective classes.
			// While still having a patch events here to do the work in this regard.
			//
			auto sourceHolder = RE::ScriptEventSourceHolder::GetSingleton();

			if (!sourceHolder) {
				//report no source
				return;
			}

			auto event_s = GetSingleton();
			//auto effectManager = EffectManager::GetSingleton();//should do the events here.
			if (!event_s) {  // || !effectManager) {
				//report this thing no exist?
				return;
			}

			//sourceHolder->AddEventSink<RE::TESHitEvent>(event_s);
			sourceHolder->AddEventSink<RE::TESCombatEvent>(event_s);
			sourceHolder->AddEventSink<RE::TESResetEvent>(event_s);
			sourceHolder->AddEventSink<RE::TESFormDeleteEvent>(event_s);

			logger::trace("Event Singleton successfully initialized.");
		}

		
		EventResult ProcessEvent(const RE::TESCombatEvent* a_event, RE::BSTEventSource<RE::TESCombatEvent>* a_eventSource) override
		{
			if (a_event->actor && a_event->newState == RE::ACTOR_COMBAT_STATE::kNone) {
				logger::debug("Combat stopped for {}", a_event->actor->GetName());
				NULL_VOP(StyleOverride::GetOverride(a_event->actor->formID))->ReleaseTempStyle();
			}
			return EventResult::kContinue;
		}

		EventResult ProcessEvent(const RE::TESResetEvent* a_event, RE::BSTEventSource<RE::TESResetEvent>* a_eventSource) override
		{
			if (a_event->object)
				StyleOverride::RemoveOverride(a_event->object->formID);

			return EventResult::kContinue;
		}

		//Unsure if I need such a thing right now.
		//EventResult ProcessEvent(const RE::BGSActorDeathEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override
		//{
		//	return EventResult::kContinue;
		//}

		EventResult ProcessEvent(const RE::TESFormDeleteEvent* a_event, RE::BSTEventSource<RE::TESFormDeleteEvent>* a_eventSource) override
		{
			StyleOverride::RemoveOverride(a_event->formID);
			return EventResult::kContinue;
		}

	protected:
		EventSingleton() = default;
		EventSingleton(const EventSingleton&) = delete;
		EventSingleton(EventSingleton&&) = delete;
		virtual ~EventSingleton() = default;

		auto operator=(const EventSingleton&)->EventSingleton & = delete;
		auto operator=(EventSingleton&&)->EventSingleton & = delete;
	};
}
