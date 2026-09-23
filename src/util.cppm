module;

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

export module util;

export namespace std {
	using USz = std::size_t;
	using CInt = int;
	using Bool = bool;

	template<typename A>
	using Vector = std::vector<A>;

	template<typename... As>
	using Variant = std::variant<As...>;

	template<typename A>
	using Maybe = std::optional<A>;

	using String = std::string;

	template<typename Key, typename Value>
	using Map = std::unordered_map<Key, Value>;
}
