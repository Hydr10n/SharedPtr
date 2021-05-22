/*
 * Header File: SharePtr.h
 * Last Update: 2021/05/22
 *
 * Copyright (C) Hydr10n@GitHub. All Rights Reserved.
 */

#ifndef _SHAREDPTR_H
#define _SHAREDPTR_H

#include "SharedPtrBase.h"

namespace Hydr10n {
namespace Memory {
template <class T, class Deleter = DefaultDeleter<T>> struct SharedPtr : SharedPtrBase<T, Deleter> {
    SharedPtr() : SharedPtrBase<T, Deleter>() {}

    SharedPtr(T *ptr) : SharedPtrBase<T, Deleter>(ptr) {}

    SharedPtr(const SharedPtr &src) : SharedPtrBase<T, Deleter>(src) {}

    T &operator*() const { return *this->Get(); }
};

template <class T, class Deleter> struct SharedPtr<T[], Deleter> : SharedPtrBase<T, Deleter> {
    SharedPtr() : SharedPtrBase<T, Deleter>() {}

    SharedPtr(T *ptr) : SharedPtrBase<T, Deleter>(ptr) {}

    SharedPtr(const SharedPtr &src) : SharedPtrBase<T, Deleter>(src) {}

    T &operator[](int index) const { return this->Get()[index]; }
};
} // namespace Memory
} // namespace Hydr10n

#endif