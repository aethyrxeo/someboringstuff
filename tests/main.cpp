//#define BOOST_TEST_MODULE TestClass Test
#include <boost/test/unit_test.hpp>
#include <boost/hana.hpp>
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <iostream>

namespace hana = boost::hana;
using namespace hana::literals;

struct Signal {
	template <typename Init, typename... Ts> auto event(Ts&&... args) {
		return hana::type_c<typename Init::next_state>;
	}
};

struct Input1 {};

struct Input2 {};

template <typename Init, typename... Input>
struct Transition {
	using next_state = Init;
};

struct InitialState {
};

struct OutputState {
};

template <>
struct Transition<InitialState, Input1> {
	using next_state = OutputState;
};
template <>
struct Transition<OutputState, Input2> {
	using next_state = InitialState;
};

template <typename Input>
auto print(Input&&) {
	std::cout << typeid(Input).name() << '\n';
}
template <typename Input, typename... Rest>
auto print(Input&&, Rest&&... rest) {
	std::cout << typeid(Input).name() << '\n';
	print(rest...);
}

template <typename Init, typename... Input>
auto transition() {
	return hana::type_c<
		typename Transition<
		typename std::remove_reference_t<Init>,
		typename std::remove_reference_t<Input>...>::next_state>;
};



template <class State, typename Input>
auto recurse(State&& state, Input&& i) {
	using T = typename decltype(transition<typename decltype(+state)::type, typename decltype(+i)::type>())::type;

	return hana::type_c<T>;
}


template <class State, typename Input, typename... Rest>
auto recurse(State&& state, Input&& i, Rest&&... rest) {
	using T = typename decltype(transition<typename decltype(+state)::type, typename decltype(+i)::type>())::type;

	return recurse(hana::type_c<T>, rest...);
}

auto multipleTransition = [](auto&& init) {
	return [init = init](auto&&... ts) {
		auto types = hana::make_tuple(ts...);

		return hana::unpack(types, [&](auto&&... x) {
			return recurse(init, x...);
		});
	};
};

template <class T>
using expr_type = std::remove_cv_t<std::remove_reference_t<T>>;
/*
//static_assert...
//test state transition INPUT ++ CURR_STATE -> NEXT_STATE ++ EVENT
BOOST_AUTO_TEST_SUITE(fsmTest)
	//INPUT
	BOOST_AUTO_TEST_CASE(SIMPLE_TRANSITION) {

		using out = typename decltype(transition(InitialState{})(Input1{}))::type;

		static_assert(std::is_same<expr_type<out>, expr_type<OutputState>>::value, "");
	}
	BOOST_AUTO_TEST_CASE(REVERSE_TRANSITION) {

		using out = typename decltype(transition(OutputState{})(Input2{}))::type;

		static_assert(std::is_same<expr_type<out>, expr_type<InitialState>>::value, "");
	}
	//providing hana_tuple_t<Input, Input> needs to return with the same state from the beginning
	BOOST_AUTO_TEST_CASE(STEP_VERIFICATION) {
		auto i = hana::make_tuple(hana::type_c<Input1>, hana::type_c<Input2>);
		

		//using out = typename decltype(result)::type;
		//static_assert(std::is_same<expr_type<out>, expr_type<InitialState<Input2>>>::value, "");
	}
	//call event after 
BOOST_AUTO_TEST_SUITE_END()
*/
int main() {
	auto result = multipleTransition(hana::type_c<InitialState>)(
		hana::type_c<Input1>, 
		hana::type_c<Input2>
		);
	std::cout << typeid(result).name() << '\n';
	return 0;
}