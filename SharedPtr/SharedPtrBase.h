/*
 * Header File: SharePtrBase.h
 * Last Update: 2021/05/22
 *
 * Copyright (C) Hydr10n@GitHub. All Rights Reserved.
 */

#ifndef _SHAREDPTRBASE_H
#define _SHAREDPTRBASE_H

#include <algorithm>
#include <vector>

namespace Hydr10n {
namespace Memory {
template <class T> struct DefaultDeleter {
    void operator()(T *ptr) { delete ptr; }
};

template <class T> struct DefaultDeleter<T[]> {
    void operator()(T *ptr) { delete[] ptr; }
};

template <class T, class Deleter> class SharedPtrBase {
public:
    SharedPtrBase() { Initialize(); }

    SharedPtrBase(T *ptr) { Initialize(ptr); }

    SharedPtrBase(const SharedPtrBase &src) : m_ReferenceList(src.m_ReferenceList) {
        if (m_ReferenceList->empty())
            Initialize();
        else {
            m_ReferenceList->push_back(this);
            m_Pointer = src.m_Pointer;
            m_IsOwner = false;
        }
    }

    virtual ~SharedPtrBase() {
        Release();
        DeleteReferenceList();
    }

    SharedPtrBase &operator=(SharedPtrBase src) {
        Swap(src);
        return *this;
    }

    T *operator->() const { return Get(); }

    operator bool() const { return Get() != nullptr; }

    T *Get() const { return m_Pointer; }

    size_t GetReferenceCount() const { return m_ReferenceList->size(); }

    void Swap(SharedPtrBase &rhs) {
        std::replace(m_ReferenceList->begin(), m_ReferenceList->end(), this, &rhs);
        std::replace(rhs.m_ReferenceList->begin(), rhs.m_ReferenceList->end(), &rhs, this);
        std::swap(m_IsOwner, rhs.m_IsOwner);
        std::swap(m_Pointer, rhs.m_Pointer);
        std::swap(m_ReferenceList, rhs.m_ReferenceList);
    }

    void Reset(T *ptr = nullptr) {
        Release();
        if (!m_ReferenceList->empty()) {
            DeleteReferenceList();
            Initialize(ptr);
        } else if (ptr != nullptr) {
            m_ReferenceList->push_back(this);
            m_Pointer = ptr;
        }
    }

protected:
    using ReferenceList = std::vector<SharedPtrBase<T, Deleter> *>;

    bool m_IsOwner;
    T *m_Pointer;
    ReferenceList *m_ReferenceList;

    void Initialize(T *ptr = nullptr) {
        m_ReferenceList = new ReferenceList;
        if (ptr != nullptr)
            m_ReferenceList->push_back(this);
        m_Pointer = ptr;
        m_IsOwner = true;
    }

    void Release() {
        const auto end = m_ReferenceList->end(), pos = std::remove(m_ReferenceList->begin(), end, this);
        if (pos != end) {
            m_ReferenceList->erase(pos, end);
            if (m_ReferenceList->empty()) {
                Deleter()(m_Pointer);
                m_Pointer = nullptr;
            }
        }
    }

    void DeleteReferenceList() {
        if (m_IsOwner)
            if (m_ReferenceList->empty())
                delete m_ReferenceList;
            else
                m_ReferenceList->front()->m_IsOwner = true;
    }
};
} // namespace Memory
} // namespace Hydr10n

#endif