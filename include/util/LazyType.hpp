#ifndef SRC_LAZYTYPE_HPP
#define SRC_LAZYTYPE_HPP

#if BOOST_VERSION < 105600 && BOOST_CLANG == 1
# define BOOST_PP_VARIADICS 1
#endif

#include <memory>
#include <boost/preprocessor/cat.hpp>

#define LAZY_TYPE(Type, name) \
	using BOOST_PP_CAT(name, _type) = Type;\
	BOOST_PP_CAT(name, _type) BOOST_PP_CAT(create_, name)();\
	inline Type& name() { \
		static ::std::unique_ptr<Type> BOOST_PP_CAT(name, _value);\
		if (!BOOST_PP_CAT(name, _value)) { \
			BOOST_PP_CAT(name, _value).reset(new Type(\
					BOOST_PP_CAT(create_, name)())\
				);\
		}\
		return *BOOST_PP_CAT(name, _value);\
	}\
	inline Type BOOST_PP_CAT(create_, name)()



#endif /* SRC_LAZYTYPE_HPP */
