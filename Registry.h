#pragma once

#include <map>


template<class T1, class T2>
class Registry
{
public:
    Registry() {}

    void add( const T1 t1, const T2 t2 )
    {
        m_dataT1T2.insert( { t1, t2 } );
        m_dataT2T1.insert( { t2, t1 } );
    }

    T1 get( const T2 t2 ) const
    {
        return m_dataT2T1.at( t2 );
    }

    T2 get( const T1 t1 ) const
    {
        return m_dataT1T2.at( t1 );
    }

    void clear()
    {
        m_dataT1T2.clear();
        m_dataT2T1.clear();
    }

    void remove( const T1 t1 )
    {
        auto itT1 = m_dataT1T2.find( t1 );

        if ( itT1 != m_dataT1T2.end() )
        {
            auto t2 = get( t1 );
            auto itT2 = m_dataT2T1.find( t2 );

            m_dataT1T2.erase( itT1 );
            m_dataT2T1.erase( itT2 );
        }
    }

    void remove( const T2 t2 )
    {
        auto itT2 = m_dataT2T1.find( t2 );

        if ( itT2 != m_dataT2T1.end() )
        {
            auto t1 = get( t2 );
            auto itT1 = m_dataT1T2.find( t1 );

            m_dataT2T1.erase( itT2 );
            m_dataT1T2.erase( itT1 );
        }
    }

private:
    std::map<T1, T2> m_dataT1T2;
    std::map<T2, T1> m_dataT2T1;
};
