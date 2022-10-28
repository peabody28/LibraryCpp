#pragma once
#include <map>
#include <vector>

enum ActionType;
enum WorkmanType;

class ActionFilter
{
private:
	std::pair<ActionType, std::pair<int, WorkmanType*>>* actionFilters;
    int count;
protected:
    WorkmanType* Permissions(ActionType actionType, int *count);
public:

    ActionFilter();

	void AddPermission(WorkmanType workmanType, ActionType type);
    
    bool IsActionAvailable(WorkmanType workmanType, ActionType type);

    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<ActionType, std::pair<int, WorkmanType*>>;
        using pointer = std::pair<ActionType, std::pair<int, WorkmanType*>>*;
        using reference = std::pair<ActionType, std::pair<int, WorkmanType*>>&;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        // Prefix increment
        Iterator& operator++() { m_ptr = m_ptr++; return *this; }

        // Postfix increment
        Iterator operator++(int) { Iterator tmp = *this; m_ptr = m_ptr++; return tmp; }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer m_ptr;
    };

    Iterator begin() { return Iterator(actionFilters); }
    Iterator end() { return Iterator(actionFilters+count); }

    Iterator Find(ActionType actionType);
    void Insert(Iterator::value_type val);
};