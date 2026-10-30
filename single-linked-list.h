
#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node()
            : value{} {
        }

        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }

        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        template <typename>
        friend class BasicIterator;

        explicit BasicIterator(Node* node) noexcept
            : node_(node) {
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept
            : node_(other.node_) {
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(
            const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(
            const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(
            const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(
            const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator old(*this);
            ++(*this);
            return old;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        try {
            Assign(values.begin(), values.end());
        } catch (...) {
            Clear();
            throw;
        }
    }

    SingleLinkedList(const SingleLinkedList& other) {
        try {
            Assign(other.cbegin(), other.cend());
        } catch (...) {
            Clear();
            throw;
        }
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList temp(rhs);
            swap(temp);
        }
        return *this;
    }

    ~SingleLinkedList() {
        Clear();
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    void PushFront(const Type& value) {
        Node* new_node = new Node(value, head_.next_node);
        head_.next_node = new_node;
        ++size_;
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);

        Node* new_node = new Node(value, pos.node_->next_node);

        pos.node_->next_node = new_node;
        ++size_;

        return Iterator{new_node};
    }

    void PopFront() noexcept {
        assert(!IsEmpty());

        Node* node_to_delete = head_.next_node;
        head_.next_node = node_to_delete->next_node;

        delete node_to_delete;
        --size_;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr);
        assert(pos.node_->next_node != nullptr);

        Node* node_to_delete = pos.node_->next_node;
        Node* next_node = node_to_delete->next_node;

        pos.node_->next_node = next_node;

        delete node_to_delete;
        --size_;

        return Iterator{next_node};
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* node_to_delete = head_.next_node;
            head_.next_node = node_to_delete->next_node;
            delete node_to_delete;
        }

        size_ = 0;
    }

    void swap(SingleLinkedList& other) noexcept {
        using std::swap;
        swap(head_.next_node, other.head_.next_node);
        swap(size_, other.size_);
    }

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{nullptr};
    }

private:
    template <typename InputIt>
    void Assign(InputIt first, InputIt last) {
        auto last_inserted = before_begin();

        for (; first != last; ++first) {
            last_inserted = InsertAfter(last_inserted, *first);
        }
    }

    Node head_;
    size_t size_ = 0;
};

// Свободная функция swap
template <typename Type>
void swap(SingleLinkedList<Type>& lhs,
          SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

// Сравнение на равенство
template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }

    return std::equal(
        lhs.begin(), lhs.end(),
        rhs.begin()
    );
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

// Лексикографическое сравнение
template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs,
               const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end()
    );
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs,
               const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
