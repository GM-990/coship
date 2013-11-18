#ifndef CUTESINGLETON_H
#define CUTESINGLETON_H

template<class T>
class CSCuteSingleton
{
public:
    static T* getInstance()
    {
        if (m_instance == 0)
        {
            m_instance = new T;
        }
        return m_instance;
    }

    static T* instance()
    {
        return m_instance;
    }

    static int releaseInstance()
    {
        if (m_instance == 0)
        {
            return 0;
        }
        else
        {
            delete m_instance;
            m_instance = 0;
        }
        return 1;
    }

    static T* m_instance;
};

template<class T>
T* CSCuteSingleton<T>::m_instance = 0;
    
#endif // CUTESINGLETON_H
