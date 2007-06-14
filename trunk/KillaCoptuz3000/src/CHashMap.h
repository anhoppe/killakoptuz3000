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
   *  @param t_key key, the hash key
   *  @return T_value value, the value, 0 if not found
   */
   T_value operator[](unsigned int t_key);

   /** 
   *  Get KeyValuePair* 
   *  @param t_key, the hash key
   *  @return KeyValuePair pointer 
   */
   KeyValuePair<T_value>* getKeyValuePair(unsigned int t_key);

   /**
   *  add element
   *  @param t_key key, the hash key
   *  @param t_value value, the value to store
   */
   void add(unsigned int t_key, T_value t_value);

   /**
   *  remove element
   *  @param t_key key, the hash key
   */
   void erase(unsigned int t_key);

   /**
   *  Iterate on the hash map one step
   *  @return false if end is reached
   */
   bool iterate(bool t_reset = false);

   /** Current KeyValuePair at iterator position */
   KeyValuePair<T_value> m_current;

private:
   /** Hash array */
   std::vector<std::list<KeyValuePair<T_value>*>> m_map;

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

   // Large numbers: fast access, more memory usage
   // Small numbers: a little slower, lower memory usage
   m_size = 269;

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
      erase(t_key);
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
void CHashMap<T_value>::erase(unsigned int t_key)
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

template <class T_value>
bool CHashMap<T_value>::iterate(bool t_reset)
{
   static unsigned int a_currentIndex = 0;   
   static std::list<KeyValuePair<T_value>*>::iterator a_it;
   
   //////////////////////////////////////////////////////////////////////////
   // Restart iterator
   //////////////////////////////////////////////////////////////////////////
   if (t_reset)
   {
      a_currentIndex = 0;
      a_it = m_map[0].begin();

      // If the first entry exists we have to leave here, otherwise we would jump over
      if (0 != m_map[0].size())
      {
         m_current.m_key     = (*a_it)->m_key;
         m_current.m_value   = (*a_it)->m_value;
         return true;
      }
   }

   //////////////////////////////////////////////////////////////////////////
   // Iterate, and store current variables
   //////////////////////////////////////////////////////////////////////////
   
   // Do until an entry is found: (or the end is reached)
   // 1. Iterate over list
   // 2. If not possible, iterate over vector

   // We are at the old position (start or last one)   
   bool a_found   = false;
   bool a_end     = false;

   // Try to find the next
   while (!a_found && !a_end)
   {
      // is there a not empty list at our position?
      if (0 != m_map[a_currentIndex].size())
      {         
         if (a_it != m_map[a_currentIndex].end())
         {
            a_it++;        
            if (a_it != m_map[a_currentIndex].end())
               a_found = true;
         }
      }
      if (false == a_found)
      {
         a_currentIndex++;
         if (a_currentIndex == m_size)
         {
            a_end = true;
         }
         else if (0 != m_map[a_currentIndex].size())
         {
            a_it = m_map[a_currentIndex].begin();
            a_found = true;
         }
      }
   }

   if (a_found)
   {
      m_current.m_key   = (*a_it)->m_key;
      m_current.m_value = (*a_it)->m_value;
   }
   return a_found && (!a_end);
}

#endif