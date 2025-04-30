#pragma once

template<
    typename tkey,
    typename tvalue>
class associative_container
{

public:

    virtual ~associative_container() noexcept = default;

public:

    virtual void upsert(
        tkey const &key,
        tvalue &&value) = 0;

    virtual tvalue &obtain(
        tkey const &key) = 0;

    virtual tvalue dispose(
        tkey const &key) = 0;

};