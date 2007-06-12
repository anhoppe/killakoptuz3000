// ***************************************************************
//  CHashMap   version:  1.0   ·  date: 06/12/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>

template <class T_value>
struct KeyValuePair
{
   T_value        m_value;
   unsigned int   m_key;
};

template <class T_value>
class CHashMap
{
public:
   CHashMap();
   ~CHashMap();

   /** 
   *  retrieve element with overloaded operator []
   *  @param t_key key
   */
   T_value operator[](unsigned int t_key);

   /**
   *  add element
   *  @param t_key key
   *  @param t_value value
   */
   void add(unsigned int t_key, T_value t_value);

   /**
   *  remove element
   *  @param t_key key   
   */
   void remove(unsigned int t_key);

private:
   /** Hash array */
   std::vector<std::vector<KeyValuePair<T_value>*>> m_map;

   /** Size of primary vector */
   unsigned int m_size;

   /** Hash function */
   unsigned int getIndex(unsigned int t_key);
};

template <class T_value>
CHashMap<T_value>::CHashMap()
{
   // Size of primary vector
   // Should be a prime number
   m_size = 101;
}

template <class T_value>
CHashMap<T_value>::~CHashMap()
{

}

template <class T_value>
T_value CHashMap<T_value>::operator [](unsigned int t_key)
{
   T_value r_ret = 0;
   unsigned int a_index = getIndex(t_key);

   for (int i=0; i < m_map[a_index].size(); i++)
   {
      if (t_key == m_map[a_index][i]->m_key)
      {
         r_ret = m_map[a_index][i]->m_value;
         break;
      }
   }

   return r_ret;
}


template <class T_value>
void CHashMap<T_value>::add(unsigned int t_key, T_value t_value)
{   
   KeyValuePair<T_value>*           a_keyValue;

   a_keyValue = new KeyValuePair<T_value>;

   a_keyValue->m_key       = t_key;
   a_keyValue->m_value     = t_value;

   unsigned int a = getIndex(t_key);
   m_map[getIndex(t_key)].push_back(a_keyValue);
}

template <class T_value>
unsigned int CHashMap<T_value>::getIndex(unsigned int t_key)
{
   return t_key % m_size;
}
#endif