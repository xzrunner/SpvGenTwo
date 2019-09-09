#pragma once

#include "Entry.h"

namespace spvgentwo
{
	template <class T>
	class EntryIterator
	{
	public:
		EntryIterator(Entry<T>* _pEntry = nullptr) : m_pEntry(_pEntry) {}
		EntryIterator(const EntryIterator<T>& _other) : m_pEntry(_other.m_pEntry) {}

		bool operator==(const EntryIterator<T>& _other) const;
		bool operator!=(const EntryIterator<T>& _other) const;

		// pre
		EntryIterator<T>& operator++();
		EntryIterator<T>& operator--();

		// post
		EntryIterator<T> operator++(int);
		EntryIterator<T> operator--(int);

		Entry<T>& operator*() { return *m_pEntry; }
		const Entry<T>& operator*() const { return *m_pEntry; }

		Entry<T>* operator->() { return m_pEntry; }
		const Entry<T>* operator->() const { return m_pEntry; }

	private:
		Entry<T>* m_pEntry = nullptr;
	};

	template<class T>
	inline bool EntryIterator<T>::operator==(const EntryIterator<T>& _other) const
	{
		return m_pEntry == _other.m_pEntry;
	}

	template<class T>
	inline bool EntryIterator<T>::operator!=(const EntryIterator<T>& _other) const
	{
		return m_pEntry != _other.m_pEntry;
	}

	template<class T>
	inline EntryIterator<T>& EntryIterator<T>::operator++()
	{
		m_pEntry = m_pEntry->m_pNext;
		return *this;
	}
	template<class T>
	inline EntryIterator<T>& EntryIterator<T>::operator--()
	{
		m_pEntry = m_pEntry->m_pPrev;
		return *this;
	}
	template<class T>
	inline EntryIterator<T> EntryIterator<T>::operator++(int)
	{
		EntryIterator<T> ret(m_pEntry);
		m_pEntry = m_pEntry->m_pNext;
		return ret;
	}

	template<class T>
	inline EntryIterator<T> EntryIterator<T>::operator--(int)
	{
		EntryIterator<T> ret(m_pEntry);
		m_pEntry = m_pEntry->m_pPrev;
		return ret;
	}
} // !spvgentwo