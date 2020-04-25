#pragma once

#include <string>
#include <utility>


class NotFound
{
 public:
    explicit NotFound( std::string strMessage )
        : m_strMessage( std::move( strMessage ))
    {}

    std::string what() { return m_strMessage; }

 private:
    std::string m_strMessage;
};
