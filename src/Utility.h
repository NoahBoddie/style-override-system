#pragma once

//I plan to move this at some point, or put it in a project called rogues gallery.
// For now, it's just in here I guess.

namespace SOS
{
	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T operator~(T a)
	{
		return (T)~(int)a;
	}
	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T operator|(T a, T b)
	{
		return (T)((int)a | (int)b);
	}
	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T operator&(T a, T b)
	{
		return (T)((int)a & (int)b);
	}
	template<class T1, std::integral T2>requires(std::is_enum_v<T1>)
		inline constexpr T1 operator&(T1 a, T2 b) 
	{
		return (T1)((int)a & (int)b); 
	}
	template<std::integral T1, class T2>requires(std::is_enum_v<T2>)
		inline constexpr T2 operator&(T1 a, T2 b) 
	{ 
		return (T2)((int)a & (int)b); 
	}



	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T operator^(T a, T b)
	{
		return (T)((int)a ^ (int)b);
	}
	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T& operator|=(T& a, T b)
	{
		return (T&)((int&)a |= (int)b);
	}
	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T& operator&=(T& a, T b)
	{
		return (T&)((int&)a &= (int)b);
	}
	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T& operator^=(T& a, T b)
	{
		return (T&)((int&)a ^= (int)b);
	}

	template <class T>requires(std::is_enum_v<T>)
		inline constexpr bool operator!(T t) {
		return t == static_cast<T>(0);
	}

	template <class T>requires(std::is_enum_v<T>)
		inline constexpr bool operator==(T t, bool b) {
		return (static_cast<int>(t) != 0) == b;
	}


	template <class T>requires(std::is_enum_v<T>)
		inline constexpr bool operator!=(T t, bool b) {
		return (static_cast<int>(t) != 0) != b;
	}

	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T operator<<(T t, int i) {
		return static_cast<T>(static_cast<int>(t) << i);
	}

	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T operator>>(T t, int i) {
		return static_cast<T>(static_cast<int>(t) >> i);
	}


	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T& operator<<=(T t, int i) {
		return t = t << i;
	}

	template <class T>requires(std::is_enum_v<T>)
		inline constexpr T& operator>>=(T t, int i) {
		return t = t >> i;
	}
	template<class I = int>requires(std::is_integral_v<I>)
		I GetThreadID()
	{
		auto id = std::this_thread::get_id();

		return *reinterpret_cast<I*>(&id);
	}

#define get_switch(mc_behaviour) auto switch_value = mc_behaviour; switch(switch_value)

#define cycle_switch(mc_flag) \
	if (false){\
		break_##mc_flag:\
		true;\
	}\
	else\
	for (decltype(mc_flag) __flag = (decltype(mc_flag))0x1; \
	__flag < sizeof(decltype(mc_flag)) * 8; __flag = __flag << 1) \
	switch (__flag & mc_flag)



	//namespace DefaultType
	struct DefaultType
	{
		void* _address{ nullptr };
		struct Default
		{
			template <class T> operator T() const { static T value{}; return value; }
		} static inline value;


		template <class T> operator T* () const { return reinterpret_cast<T*>(_address); }

		DefaultType(void* ptr) : _address(ptr) {}
	};

#define __MACRO__NULL_COND_CHOICE(PREFIX,_0,_1,_2,_3,_4,_5,SUFFIX,...) PREFIX##SUFFIX 

#define __MACRO__CHOICE__5(PREFIX,_0,_1,_2,_3,_4,_5,SUFFIX,...) PREFIX##SUFFIX 





#define ENUM_2(mc_enum_name, mc_integer_name) \
namespace detail { struct MacroStruct__##mc_enum_name { enum mc_enum_name##Type : mc_integer_name; }; }\
using mc_enum_name = detail::MacroStruct__##mc_enum_name::mc_enum_name##Type; \
enum detail::MacroStruct__##mc_enum_name::mc_enum_name##Type : mc_integer_name 

#define ENUM_1(mc_enum_name) ENUM_2(mc_enum_name, int)

#define ENUM(...) __MACRO__CHOICE__5(ENUM_, _0,__VA_ARGS__,5,4,3,2,1,0)(__VA_ARGS__)


#define STRINGIZE_RAW(mc_content) #mc_content
#define STRINGIZE(mc_content) STRINGIZE_RAW(mc_content)
#define CONCAT_RAW(mc_a, mc_b) mc_a##mc_b
#define CONCAT(mc_a, mc_b) CONCAT_RAW(mc_a, mc_b)

	template <typename T>
	concept pointer_type = std::is_pointer_v<T> ||
		(requires(T pointer) {
			{ pointer.operator->() } -> std::same_as<typename std::remove_reference_t<T>::pointer >;
	}
	&& (requires(T pointer) {
		{ pointer.operator*() } -> std::same_as<typename std::add_lvalue_reference<typename std::remove_reference_t<T>::element_type>::type>;
	})
		);


	namespace detail
	{
		struct CompareType
		{
			//Somewhere within this, this should only be used on objects that have -> and can be compared to null. IE, nullable types.

			static inline thread_local void* _resumeStorage;

			struct Struct {
				template <class T>
				bool operator==(const T& a_rhs)
				{
					_resumeStorage = std::to_address(a_rhs);
					return !a_rhs;
				}

			} static inline value;

			//template <class T>
			//static inline bool operator==(const T& a, const Struct& b) { _resumeStorage = std::to_address(a); return a == false; }
			//template <class T>
			//bool Condition()

			template <class T>

			//static std::remove_pointer<T>::type* Resume()
			static std::pointer_traits<T>::element_type* Resume()
			{
				return reinterpret_cast<std::remove_pointer_t<T>*>(_resumeStorage);
			}

		};
	}

#define NULL_OR(mc_test, mc_or_val) mc_test == detail::CompareType::value ? mc_or_val : detail::CompareType::Resume<decltype(mc_test)>()

#define NULL_OP(mc_test) mc_test == detail::CompareType::value ? DefaultType::value : detail::CompareType::Resume<decltype(mc_test)>()
#define NULL_VOP(mc_test) mc_test == detail::CompareType::value ? void() : detail::CompareType::Resume<decltype(mc_test)>()

//For the these I think it's important to send an issue that the subject must be nullable.

	void ref_foo(int& i)
	{

	}

	void foo()
	{

		RE::Actor* n = nullptr;
		//I'm keeping the resume bit (for now) because it works with functions

		//bool t = n == CompareType::value;

		//auto test_else = NULL_OP(1);
		//ref_foo(1);
		//currentProcess == false ? DefaultType::value : currentProcess
		//This doesn't work because there's no way to automate the type given for resume to use later. Weirdly, it's not the beginning that's
		// the problem, but the end
		auto something = NULL_OP(NULL_OP(NULL_OP(n)->GetActorRuntimeData().currentProcess)->currentPackage.package)->combatStyle;

	}

}

namespace RGL
{
	float Clamp(float value, float _min, float _max)
	{
		if (_min < _max)
		{
			if (value > _max)
				return _max;
			else if (value < _min)
				return _min;
		}
		else
		{
			if (value < _max)
				return _max;
			else if (value > _min)
				return _min;
		}
		return value;
	}


	using string_hash = uint32_t;



	template <class CharType>
	constexpr CharType ConstToLower(const CharType c)
	{
		switch (c)
		{
		case 'A': return 'a';
		case 'B': return 'b';
		case 'C': return 'c';
		case 'D': return 'd';
		case 'E': return 'e';
		case 'F': return 'f';
		case 'G': return 'g';
		case 'H': return 'h';
		case 'I': return 'i';
		case 'J': return 'j';
		case 'K': return 'k';
		case 'L': return 'l';
		case 'M': return 'm';
		case 'N': return 'n';
		case 'O': return 'o';
		case 'P': return 'p';
		case 'Q': return 'q';
		case 'R': return 'r';
		case 'S': return 's';
		case 'T': return 't';
		case 'U': return 'u';
		case 'V': return 'v';
		case 'W': return 'w';
		case 'X': return 'x';
		case 'Y': return 'y';
		case 'Z': return 'z';
		default: return c;
		}
	}


	template<bool Insensitive = false>
	constexpr string_hash hash(const char* data, size_t const size) noexcept
	{
		string_hash hash = 5381;

		for (const char* c = data; c < data + size; ++c) {
			unsigned char _c;

			if constexpr (Insensitive) _c = (unsigned char)ConstToLower(*c);
			else _c = (unsigned char)*c;

			hash = ((hash << 5) + hash) + _c;
		}

		return hash;
	}

	template<bool Insensitive = false>
	constexpr string_hash hash(const char* data) noexcept
	{
		string_hash hash = 5381;

		for (const char* c = data; *c == '\0'; ++c) {
			//hash = ((hash << 5) + hash) + (unsigned char)std::tolower(*c);
			unsigned char _c;

			if constexpr (Insensitive) _c = (unsigned char)ConstToLower(*c);
			else _c = (unsigned char)*c;

			hash = ((hash << 5) + hash) + _c;
		}

		return hash;
	}

	template<bool Insensitive = false, size_t N = 1>
	constexpr string_hash hash(const char(&data)[N]) noexcept
	{
		string_hash hash = 5381;

		for (const char* c = data; *c == '\0'; ++c) {
			unsigned char _c;

			if constexpr (Insensitive) _c = (unsigned char)ConstToLower(*c);
			else _c = (unsigned char)*c;

			hash = ((hash << 5) + hash) + _c;
		}

		return hash;
	}

	template<bool Insensitive = false>
	constexpr string_hash hash(std::string& string) noexcept
	{
		return hash<Insensitive>(string.data(), string.size());
	}

	template<bool Insensitive = false>
	constexpr string_hash hash(std::string_view& view) noexcept
	{
		return hash<Insensitive>(view.data(), view.size());
	}

	template<bool Insensitive = false>
	constexpr string_hash hash(const std::string& string) noexcept
	{
		return hash<Insensitive>(string.data(), string.size());
	}

	template<bool Insensitive = false>
	constexpr string_hash hash(const std::string_view& view) noexcept
	{
		return hash<Insensitive>(view.data(), view.size());
	}


	constexpr string_hash operator"" _h(const char* str, size_t size) noexcept
	{
		//function and operator courtesy of Ershin's TDM
		return hash<false>(str, size);
	}


	constexpr string_hash operator"" _ih(const char* str, size_t size) noexcept
	{
		//function and operator courtesy of Ershin's TDM
		return hash<true>(str, size);
	}

	template<size_t N>
	struct StringLiteral {
		static constexpr size_t Size = N;
		constexpr StringLiteral(const char(&str)[N]) {
			std::copy_n(str, N, value);
		}

		constexpr operator const char* () { return &value; }


		constexpr operator std::string_view() { return std::string_view(&value, Size); }


		constexpr std::string_view view() { return std::string_view(&value, Size); }

		char value[N];
	};

	//template<const char* c_str> constexpr string_hash hash_string = hash(c_str);
	template<StringLiteral Str, bool Insensitive = true> constexpr string_hash hash_string = hash<Insensitive>(Str.value);

}