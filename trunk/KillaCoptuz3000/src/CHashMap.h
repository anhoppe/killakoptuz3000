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
#include <list>

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
   *  retrieve value with overloaded operator []
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
   std::vector<std::list<KeyValuePair<T_value>*>> m_map;

   /** Size of primary vector */
   unsigned int m_size;

   /** Hash function */
   unsigned int getIndex(unsigned int t_key);

   /** 
   *  Get KeyValuePair* 
   *  @param t_key
   */
   KeyValuePair<T_value>* getKeyValuePair(unsigned int t_key);
};

template <class T_value>
CHashMap<T_value>::CHashMap()
{
   // Size of primary vector
   // Should be a prime number
   m_size = 101;

   // Initialize vector
   m_map.resize(m_size);
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
   std::list<KeyValuePair<T_value>*>::iterator a_it;

   for (a_it = m_map[a_index].begin(); a_it != m_map[a_index].end(); a_it++)
   {
      if ((*a_it)->m_key == t_key)
      {
         r_ret = (*a_it)->m_value;
         break;
      }
   }

   return r_ret;
}


template <class T_value>
void CHashMap<T_value>::add(unsigned int t_key, T_value t_value)
{   
   KeyValuePair<T_value>* a_keyValuePair = 0;

   // Check if there is an element with that key
   if (0 != getKeyValuePair(t_key))
   {      
      // Remove it
      remove(t_key);
   }

   // Create new entry
   a_keyValuePair = new KeyValuePair<T_value>;
   a_keyValuePair->m_key       = t_key;
   a_keyValuePair->m_value     = t_value;
   
   // Insert it in list at vector position
   m_map[getIndex(t_key)].push_back(a_keyValuePair);
}

template <class T_value>
unsigned int CHashMap<T_value>::getIndex(unsigned int t_key)
{
   return t_key % m_size;
}

template <class T_value>
KeyValuePair<T_value>* CHashMap<T_value>::getKeyValuePair(unsigned int t_key)
{
   KeyValuePair<T_value>* r_ret = 0;
   unsigned int a_index = getIndex(t_key);

   std::list<KeyValuePair<T_value>*>::iterator a_it;

   for (a_it = m_map[a_index].begin(); a_it != m_map[a_index].end(); a_it++)
   {
      if ((*a_it)->m_key == t_key)
      {
         r_ret = *a_it;
         break;
      }
   }

   return r_ret;
}

template <class T_value>
void CHashMap<T_value>::remove(unsigned int t_key)
{   
   KeyValuePair<T_value>* a_keyValuePair = 0;
   unsigned int a_index = getIndex(t_key);

   a_keyValuePair = getKeyValuePair(t_key);

   if (0 != a_keyValuePair)
   {
      m_map[a_index].remove(a_keyValuePair);
      delete a_keyValuePair;
   }
}

#endif