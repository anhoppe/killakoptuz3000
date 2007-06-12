// ***************************************************************
//  CHashMap   version:  1.0   ·  date: 06/12/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CHashMap.h"
#include "Objects/CObject.h"

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
   KeyValuePair*           a_keyValue;

   a_keyValue = new KeyValuePair;

   a_keyValue->m_key       = t_key;
   a_keyValue->m_value     = t_value;

   m_map[getIndex(t_key)].push_back(a_keyValue);
}

template <class T_value>
unsigned int CHashMap<T_value>::getIndex(unsigned int t_key)
{
   return t_key % m_size;
}