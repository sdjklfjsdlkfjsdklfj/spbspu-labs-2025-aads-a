#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_SPLIT_BUFFER_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_COMMON_SPLIT_BUFFER_HPP

#include <limits>
#include <stdexcept>
#include <utility>
#include "enable-if-input-iterator.hpp"

namespace kizhin {
  template < typename T >
  class SplitBuffer final
  {
  public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using size_type = std::size_t;

    SplitBuffer() noexcept;
    SplitBuffer(const SplitBuffer&);
    SplitBuffer(SplitBuffer&&) noexcept;
    SplitBuffer(size_type, const_reference = value_type{});
    template < typename InputIt, detail::enable_if_input_iterator< InputIt > = 0 >
    SplitBuffer(InputIt, InputIt);
    ~SplitBuffer();

    SplitBuffer& operator=(const SplitBuffer&);
    SplitBuffer& operator=(SplitBuffer&&) noexcept;

    void assign(size_type, const_reference = value_type{});
    template < typename InputIt, detail::enable_if_input_iterator< InputIt > = 0 >
    void assign(InputIt, InputIt);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    size_type size() const noexcept;
    size_type capacity() const noexcept;
    size_type backSpare() const noexcept;
    size_type frontSpare() const noexcept;
    size_type maxSize() const;
    bool empty() const noexcept;

    reference front() noexcept;
    reference back() noexcept;
    const_reference front() const noexcept;
    const_reference back() const noexcept;

    template < typename... Args >
    void emplaceFront(Args&&...);
    template < typename... Args >
    void emplaceBack(Args&&...);
    void pushBack(const_reference);
    void pushBack(value_type&&);
    void pushFront(const_reference);
    void pushFront(value_type&&);

    void popBack() noexcept;
    void popFront() noexcept;
    void clear() noexcept;
    void swap(SplitBuffer&) noexcept;

  private:
    pointer first_;
    pointer begin_;
    pointer end_;
    pointer last_;

    void allocate(size_type);
    void deallocate() noexcept;
    void reallocate(size_type);
    size_type growthCapacity(size_type) const;

    template < typename InputIt, detail::enable_if_input_iterator< InputIt > = 0 >
    void constructAtEnd(InputIt, InputIt);
    template < typename... Args >
    void constructAtEnd(size_type, Args&&...);
    void destroyAtEnd(const_pointer) noexcept;

    template < typename InputIt, detail::enable_if_input_iterator< InputIt > = 0 >
    void constructAtBegin(InputIt, InputIt);
    template < typename... Args >
    void constructAtBegin(size_type, Args&&...);
    void destroyAtBegin(const_pointer) noexcept;
  };

  template < typename T >
  SplitBuffer< T >::SplitBuffer() noexcept:
    first_(nullptr),
    begin_(nullptr),
    end_(nullptr),
    last_(nullptr)
  {}

  template < typename T >
  SplitBuffer< T >::SplitBuffer(const SplitBuffer& rhs):
    SplitBuffer()
  {
    allocate(rhs.size());
    constructAtEnd(rhs.begin(), rhs.end());
  }

  template < typename T >
  SplitBuffer< T >::SplitBuffer(SplitBuffer&& rhs) noexcept:
    first_(std::exchange(rhs.first_, nullptr)),
    begin_(std::exchange(rhs.begin_, nullptr)),
    end_(std::exchange(rhs.end_, nullptr)),
    last_(std::exchange(rhs.last_, nullptr))
  {}

  template < typename T >
  SplitBuffer< T >::SplitBuffer(const size_type size, const_reference value):
    SplitBuffer()
  {
    allocate(size);
    constructAtEnd(size, value);
  }

  template < typename T >
  template < typename InputIt, typename detail::enable_if_input_iterator< InputIt > >
  SplitBuffer< T >::SplitBuffer(InputIt first, const InputIt last):
    SplitBuffer()
  {
    for (; first != last; ++first) {
      emplaceBack(*first);
    }
  }

  template < typename T >
  SplitBuffer< T >::~SplitBuffer()
  {
    clear();
    deallocate();
  }

  template < typename T >
  SplitBuffer< T >& SplitBuffer< T >::operator=(const SplitBuffer& rhs)
  {
    SplitBuffer copy(rhs);
    swap(copy);
    return *this;
  }

  template < typename T >
  SplitBuffer< T >& SplitBuffer< T >::operator=(SplitBuffer&& rhs) noexcept
  {
    clear();
    deallocate();
    swap(rhs);
  }

  template < typename T >
  void SplitBuffer< T >::assign(const size_type size, const_reference value)
  {
    SplitBuffer tmp(size, value);
    swap(tmp);
  }

  template < typename T >
  template < typename InputIt, detail::enable_if_input_iterator< InputIt > >
  void SplitBuffer< T >::assign(const InputIt first, const InputIt last)
  {
    SplitBuffer tmp(first, last);
    swap(tmp);
  }

  template < typename T >
  typename SplitBuffer< T >::iterator SplitBuffer< T >::begin() noexcept
  {
    return iterator(begin_);
  }

  template < typename T >
  typename SplitBuffer< T >::iterator SplitBuffer< T >::end() noexcept
  {
    return iterator(end_);
  }

  template < typename T >
  typename SplitBuffer< T >::const_iterator SplitBuffer< T >::begin() const noexcept
  {
    return const_iterator(begin_);
  }

  template < typename T >
  typename SplitBuffer< T >::const_iterator SplitBuffer< T >::end() const noexcept
  {
    return const_iterator(end_);
  }

  template < typename T >
  typename SplitBuffer< T >::size_type SplitBuffer< T >::size() const noexcept
  {
    return static_cast< size_type >(end_ - begin_);
  }

  template < typename T >
  typename SplitBuffer< T >::size_type SplitBuffer< T >::capacity() const noexcept
  {
    return static_cast< size_type >(last_ - first_);
  }

  template < typename T >
  typename SplitBuffer< T >::size_type SplitBuffer< T >::backSpare() const noexcept
  {
    return static_cast< size_type >(last_ - end_);
  }

  template < typename T >
  typename SplitBuffer< T >::size_type SplitBuffer< T >::frontSpare() const noexcept
  {
    return static_cast< size_type >(begin_ - first_);
  }

  template < typename T >
  typename SplitBuffer< T >::size_type SplitBuffer< T >::maxSize() const
  {
    return std::numeric_limits< size_type >::max() / sizeof(T);
  }

  template < typename T >
  bool SplitBuffer< T >::empty() const noexcept
  {
    return begin_ == end_;
  }

  template < typename T >
  typename SplitBuffer< T >::reference SplitBuffer< T >::front() noexcept
  {
    assert(!empty());
    return *begin_;
  }

  template < typename T >
  typename SplitBuffer< T >::reference SplitBuffer< T >::back() noexcept
  {
    assert(!empty());
    return *(end_ - 1);
  }

  template < typename T >
  typename SplitBuffer< T >::const_reference SplitBuffer< T >::front() const noexcept
  {
    assert(!empty());
    return *begin_;
  }

  template < typename T >
  typename SplitBuffer< T >::const_reference SplitBuffer< T >::back() const noexcept
  {
    assert(!empty());
    return *(end_ - 1);
  }

  template < typename T >
  template < typename... Args >
  void SplitBuffer< T >::emplaceFront(Args&&... args)
  {
    if (begin_ == first_) {
      reallocate(growthCapacity(capacity() + 1));
    }
    constructAtBegin(1, std::forward< Args >(args)...);
  }

  template < typename T >
  template < typename... Args >
  void SplitBuffer< T >::emplaceBack(Args&&... args)
  {
    if (end_ == last_) {
      reallocate(growthCapacity(capacity() + 1));
    }
    constructAtEnd(1, std::forward< Args >(args)...);
  }

  template < typename T >
  void SplitBuffer< T >::pushBack(const_reference value)
  {
    emplaceBack(value);
  }

  template < typename T >
  void SplitBuffer< T >::pushBack(value_type&& value)
  {
    emplaceBack(std::move(value));
  }

  template < typename T >
  void SplitBuffer< T >::pushFront(const_reference value)
  {
    emplaceBack(value);
  }

  template < typename T >
  void SplitBuffer< T >::pushFront(value_type&& value)
  {
    emplaceBack(std::move(value));
  }

  template < typename T >
  void SplitBuffer< T >::popBack() noexcept
  {
    assert(!empty());
    destroyAtEnd(end_ - 1);
  }

  template < typename T >
  void SplitBuffer< T >::popFront() noexcept
  {
    assert(!empty());
    destroyAtBegin(end_ - 1);
  }

  template < typename T >
  void SplitBuffer< T >::clear() noexcept
  {
    destroyAtEnd(begin_);
  }

  template < typename T >
  void SplitBuffer< T >::swap(SplitBuffer& rhs) noexcept
  {
    using std::swap;
    swap(first_, rhs.first_);
    swap(begin_, rhs.begin_);
    swap(end_, rhs.end_);
    swap(last_, rhs.last_);
  }

  template < typename T >
  void SplitBuffer< T >::allocate(const size_type size)
  {
    assert(capacity() == 0);
    first_ = operator new(size * sizeof(T));
    begin_ = first_ + size / 2;
    end_ = begin_;
    last_ = first_ + size;
  }

  template < typename T >
  void SplitBuffer< T >::deallocate() noexcept
  {
    operator delete(first_);
    first_ = begin_ = end_ = last_ = nullptr;
  }

  template < typename T >
  void SplitBuffer< T >::reallocate(const size_type)
  {
    /* TODO: impelement SplitBuffer::reallocate */
  }

  template < typename T >
  typename SplitBuffer< T >::size_type SplitBuffer< T >::growthCapacity(
      const size_type newCapacity) const
  {
    size_type maxSz = maxSize();
    if (newCapacity > maxSz) {
      throw std::length_error("SplitBuffer");
    }
    size_type cap = capacity();
    if (cap >= maxSz / 2) {
      return maxSz;
    }
    return std::max(cap * 2, newCapacity);
  }

  template < typename T >
  template < typename InputIt, detail::enable_if_input_iterator< InputIt > >
  void SplitBuffer< T >::constructAtEnd(InputIt first, const InputIt last)
  {
    assert(end_ < last_ || first == last);
    for (; first != last; ++first, ++end_) {
      new (end_) value_type(*first);
    }
  }

  template < typename T >
  template < typename... Args >
  void SplitBuffer< T >::constructAtEnd(const size_type size, Args&&... args)
  {
    assert(size <= last_ - end_);
    /* TODO: here cannot be a loop - rvalue arguments */
    for (size_type i = 0; i != size; ++i, ++end_) {
      new (end_) value_type(std::forward< Args >(args)...);
    }
  }

  template < typename T >
  void SplitBuffer< T >::destroyAtEnd(const_pointer newEnd) noexcept
  {
    assert(newEnd >= begin_);
    assert(newEnd <= end_);
    for (; end_ != newEnd;) {
      --end_;
      end_->~value_type();
    }
  }

  template < typename T >
  template < typename InputIt, detail::enable_if_input_iterator< InputIt > >
  void SplitBuffer< T >::constructAtBegin(InputIt first, const InputIt last)
  {
    assert(first_ < begin_ || first == last);
    for (; first != last; ++first) {
      --begin_;
      new (begin_) value_type(*first);
    }
  }

  template < typename T >
  template < typename... Args >
  void SplitBuffer< T >::constructAtBegin(const size_type size, Args&&... args)
  {
    assert(first_ < begin_ || size == 0);
    /* TODO: here cannot be a loop - rvalue arguments */
    for (size_type i = 0; i != size; ++i) {
      --begin_;
      new (begin_) value_type(std::forward< Args >(args)...);
    }
  }

  template < typename T >
  void SplitBuffer< T >::destroyAtBegin(const_pointer newBegin) noexcept
  {
    assert(newBegin >= begin_);
    assert(newBegin <= end_);
    for (; begin_ != newBegin; ++begin_) {
      begin_->~value_type();
    }
  }

}

#endif
