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
   std::vector<std::vector<KeyValuePair*>> m_map;

   /** Size of primary vector */
   unsigned int m_size;

   /** Hash function */
   unsigned int getIndex(unsigned int t_key);
};

#endif