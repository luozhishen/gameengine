#ifndef __ATLAS_STL__
#define __ATLAS_STL__

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

namespace Zion
{

	void* AtlasMalloc(unsigned int size);
	void* AtlasRealloc(void* p, unsigned int size);
	void AtlasFree(void* p);

	template <class T>
	class AtlasSTLAlocator {
	public:
		typedef size_t    size_type;
		typedef std::ptrdiff_t difference_type;
		typedef T*        pointer;
		typedef const T*  const_pointer;
		typedef T&        reference;
		typedef const T&  const_reference;
		typedef T         value_type;
		template <class U> struct rebind { typedef AtlasSTLAlocator<U> other; };

		AtlasSTLAlocator() throw() {}
		AtlasSTLAlocator(const AtlasSTLAlocator&) throw() {}
		template <class U> AtlasSTLAlocator(const AtlasSTLAlocator<U>&) throw() {}

		~AtlasSTLAlocator() throw() {};

		pointer address(reference x) const
		{ return &x; }
		const_pointer address(const_reference x) const
		{ return &x; }

		pointer allocate(size_type n, typename AtlasSTLAlocator<T>::const_pointer hint = 0)
		{
			(void)(hint);
			return (pointer)AtlasMalloc((unsigned int)n*sizeof(T));
		};
		void deallocate(pointer p, size_type n)
		{
			(void)(n);
			AtlasFree(p);
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

		bool operator !=(const AtlasSTLAlocator<T>& o) const
		{
			(void)(o);
			return false;
		}

		bool operator ==(const AtlasSTLAlocator<T>& o) const
		{
			(void)(o);
			return true;
		}

		// to copy ctor or compare with private data, do this
		template<typename U>
		friend class AtlasSTLAlocator;
	};

	typedef std::basic_string<char, std::char_traits<char>, AtlasSTLAlocator<char>> String;
	typedef std::basic_stringstream<char, std::char_traits<char>, AtlasSTLAlocator<char>> StringStream;
	typedef std::basic_ostringstream<char, std::char_traits<char>, AtlasSTLAlocator<char> > OutputStringStream;

	template<class _Ty>
	class Vector : public std::vector<_Ty, AtlasSTLAlocator<_Ty>>
	{
	};
	template<class _Kty, class _Ty, class _Pr = std::less<_Kty>>
	class Map : public std::map<_Kty, _Ty, _Pr, AtlasSTLAlocator<std::pair<const _Kty, _Ty>>>
	{
	};

	template<class _Ty>
	class List : public std::list<_Ty, AtlasSTLAlocator<_Ty>>
	{
	};

	template<class _Kty, class _Pr = std::less<_Kty>>
	class Set : public std::set<_Kty, _Pr, AtlasSTLAlocator<_Kty>>
	{
	};

}

#endif // __ATLAS_STL__
