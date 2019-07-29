#pragma once
#include <type_traits>
#include <initializer_list>
#include <algorithm>

template<class Type>
class Range
{
    static_assert(std::is_arithmetic<Type>::value, "The Type should be arithmetic");
public:
    constexpr Range() = default;
    // constexpr Range(std::initializer_list<Type> list)
    // {
    //     switch(list.size())
    //     {
    //     case 0:
    //         break;
    //     case 1:
    //         _start = *list.begin();
    //         _end = _start;
    //         break;
    //     default:
    //         _start = *list.begin();
    //         _end = *(list.begin() + 1);
    //     }
    // }
    constexpr Range(Type start, Type end) noexcept
    : _start(start), _end(std::max(start, end)) {}
    ~Range() = default;
    Type start() const noexcept { return getStart(); }
    Type getStart() const noexcept { return _start; }
    Type end() const noexcept { return getEnd(); }
    Type getEnd() const noexcept { return _end; }
    std::pair<Type, Type> getPair() const noexcept { return std::make_pair<Type, Type>(_start, _end); } 
    Range(const Range<Type>& range) = default;
    Range(Range<Type>&& range) = default;
    void setStart(Type start) noexcept
    {
        _start = start;
        if (start > _end)
            _end = start;
    }

    void setEnd(Type end) noexcept
    {
        _end = end;
        if (end < _start)
            _start = end;
    }

    Type clamp(Type value) const noexcept { return std::clamp(value, _start, _end); }
    bool containsWithEnd(Type value) const noexcept { return (value >= _start && value <= _end); }
    bool contains(Type value) const noexcept { return (value >= _start && value < _end); }
private:
    Type _start { static_cast<Type>(0.0) };
    Type _end { static_cast<Type>(0.0) };
};

template<class Type>
bool operator==(const Range<Type>& lhs, const Range<Type>& rhs)
{
    return (lhs.start() == rhs.start()) && (lhs.end() == rhs.end());
}

template<class Type>
bool operator==(const Range<Type>& lhs, const std::pair<Type, Type>& rhs)
{
    return (lhs.start() == rhs.first) && (lhs.end() == rhs.second);
}

template<class Type>
bool operator==(const std::pair<Type, Type>& lhs, const Range<Type>& rhs)
{
    return rhs == lhs;
}