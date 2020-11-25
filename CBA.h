#pragma once
#include <vector>
#include <type_traits>
#include <span>

namespace CBA
{
	struct EntityID
	{
		static EntityID get_new() { return EntityID{ next++ }; };
		static EntityID invalid;

		bool operator==(EntityID rhs) const { return value == rhs.value; }
		bool operator>(EntityID rhs) const { return value > rhs.value; }
		bool operator<(EntityID rhs) const { return value < rhs.value; }
		explicit operator uint64_t() const { return value; }

		EntityID() : value(invalid.value) {}
	private:
		EntityID(uint64_t v) : value(v) {}
		uint64_t value;
		static uint64_t next;
	};

	EntityID EntityID::invalid { ~0U };
	uint64_t EntityID::next { 0U };


	template<typename T>
	concept Component = std::is_same_v<decltype(T::id), EntityID> && std::is_same_v<EntityID, decltype(T::id)>;

	template<Component Component_t>
	class Components
	{
	public:
		using component_iterator = typename std::vector<Component_t>::iterator;
		using const_component_iterator = typename std::vector<Component_t>::const_iterator;

		void insert(EntityID id, Component_t &&component)
		{
			const_component_iterator emplace_position = find_position_for_id_mut(id);
			component.id = id;
			components.insert(emplace_position, component);
		}

		void remove_all_with_id(EntityID id)
		{
			component_iterator erasure_begin = find_position_for_id_mut(id);

			if (erasure_begin != components.end() && (*erasure_begin).id == id)
			{
				component_iterator erasure_end = std::find_if_not(erasure_begin, components.end(), [id](const Component_t &c) { return c.id == id; });

				components.erase(erasure_begin, erasure_end);
			}
		}

		[[nodiscard]]
		std::span<const Component_t> with_id(EntityID id) const
		{
			const_component_iterator span_begin = find_position_for_id(id);
			const_component_iterator span_end = std::find_if_not(span_begin, components.end(), [id](const Component_t &c) { return c.id == id; });
			return std::span<const Component_t>(span_begin, span_end);
		}

		[[nodiscard]]
		std::span<Component_t> with_id_mut(EntityID id)
		{
			component_iterator span_begin = find_position_for_id_mut(id);
			component_iterator span_end = std::find_if_not(span_begin, components.end(), [id](const Component_t &c) { return c.id == id; });
			return std::span<Component_t>(span_begin, span_end);
		}

		component_iterator begin() { return components.begin(); }
		component_iterator end() { return components.end(); }
		const_component_iterator cbegin() const { return components.cbegin(); }
		const_component_iterator cend() const { return components.cend(); }


	private:

		[[nodiscard]]
		const_component_iterator find_position_for_id(EntityID id) const
		{
			return std::lower_bound(components.begin(), components.end(), id, [](const Component_t &component, EntityID id) { return component.id < id; });
		}

		[[nodiscard]]
		component_iterator find_position_for_id_mut(EntityID id)
		{
			return std::lower_bound(components.begin(), components.end(), id, [](Component_t &component, EntityID id) { return component.id < id; });
		}

		std::vector<Component_t> components{};
	};
}