#ifndef __ZION_STL__
#define __ZION_STL__

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <sstream>

namespace Zion
{
/*
	void* ZionMalloc(unsigned int size);
	void* ZionRealloc(void* p, unsigned int size);
	void ZionFree(void* p);
*/
	template <class T>
	class ZionSTLAlocator {
	public:
		typedef size_t    size_type;
		typedef std::ptrdiff_t difference_type;
		typedef T*        pointer;
		typedef const T*  const_pointer;
		typedef T&        reference;
		typedef const T&  const_reference;
		typedef T         value_type;
		template <class U> struct rebind { typedef ZionSTLAlocator<U> other; };

		ZionSTLAlocator() throw() {}
		ZionSTLAlocator(const ZionSTLAlocator&) throw() {}
		template <class U> ZionSTLAlocator(const ZionSTLAlocator<U>&) throw() {}

		~ZionSTLAlocator() throw() {};

		pointer address(reference x) const
		{ return &x; }
		const_pointer address(const_reference x) const
		{ return &x; }

		pointer allocate(size_type n, typename ZionSTLAlocator<T>::const_pointer hint = 0)
		{
			(void)(hint);
			return (pointer)ZION_ALLOC((unsigned int)n*sizeof(T));
		}
		void deallocate(pointer p, size_type n)
		{
			(void)(n);
			ZION_FREE(p);
		}

		size_type max_size() const throw()
		{
	        return static_cast<size_type>(-1) / sizeof(value_type);
		}

		void construct(pointer p, const T& val)
		{
			new(p) T(val);
		}
		void destroy(pointer p)
		{
			(void)(p);
			p->~T();
		}

		bool operator !=(const ZionSTLAlocator<T>& o) const
		{
			(void)(o);
			return false;
		}

		bool operator ==(const ZionSTLAlocator<T>& o) const
		{
			(void)(o);
			return true;
		}

		// to copy ctor or compare with private data, do this
		template<typename U>
		friend class ZionSTLAlocator;
	};

	typedef std::basic_string<char, std::char_traits<char>, ZionSTLAlocator<char> > String;
	typedef std::basic_stringstream<char, std::char_traits<char>, ZionSTLAlocator<char> > StringStream;
	typedef std::basic_ostringstream<char, std::char_traits<char>, ZionSTLAlocator<char> > OutputStringStream;

	template<class _Ty>
	class Array : public std::vector<_Ty, ZionSTLAlocator<_Ty> >
	{
	};
	template<class _Kty, class _Ty, class _Pr = std::less<_Kty> >
	class Map : public std::map<_Kty, _Ty, _Pr, ZionSTLAlocator<std::pair<const _Kty, _Ty> > >
	{
	};

	template<class _Ty>
	class List : public std::list<_Ty, ZionSTLAlocator<_Ty> >
	{
	};

	template<class _Kty, class _Pr = std::less<_Kty> >
	class Set : public std::set<_Kty, _Pr, ZionSTLAlocator<_Kty> >
	{
	};

}

#endif // __ZION_STL__
