#pragma once
#include <memory>
#include <algorithm>
#include <iterator>
#include <iostream>

template <class T> 
 class Vector 
  {
    public: // interfeisas

      // Member types
      typedef T* iterator;
      typedef const T* const_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef size_t size_type;
      typedef T value_type;
      typedef std::allocator<T> allocator_type;
      typedef std::ptrdiff_t difference_type;
      typedef value_type& reference;
      typedef const value_type& const_reference;
      typedef typename std::allocator_traits<std::allocator<T>>::pointer pointer;
      typedef const typename std::allocator_traits<std::allocator<T>>::pointer const_pointer;

      
      // rule of three
      Vector() { create(); }
      explicit Vector(size_type n, const T& t = T{}) { create(n, t); }
      Vector(const Vector& v) { create(v.begin(), v.end()); }
      Vector& operator=(const Vector&);
      ~Vector() { uncreate(); }
  
      T& operator[](size_type i) { return elem[i]; }
      const T& operator[](size_type i) const { return elem[i]; }

      size_type size() const { return avail - elem; }
      size_type capacity() const { return limit - elem; }
      iterator begin() { return elem; }
      const_iterator begin() const { return elem; }
      iterator end() { return avail; }
      const_iterator end() const { return avail; }
      reverse_iterator rbegin() noexcept { return reverse_iterator(avail); } 
      const_reverse_iterator rbegin() const noexcept { return reverse_iterator(avail);} 
      const_reverse_iterator crbegin() const noexcept { return reverse_iterator(avail); }
      Vector& assign(const Vector&);
      Vector<T>::allocator_type get_allocator() { return alloc; }
      reference at(size_type i)
      {
        if (i >= this->size() || i < 0)
        {
          throw std::out_of_range("ERROR");
        }
        return elem[i];
      }
      const reference at(size_type i) const
      {
        if (i >= this->size() || i < 0)
        {
          throw std::out_of_range("ERROR");
        }
        return elem[i];
      }
      reference front() { return elem; }
      const reference front() const { return elem; }

      reference back()
      {
        if (this->size() == 0)
        {
          std::cout << "ERROR! Vector is empty!\n";
        }
        return *(avail - 1);
      }
      const reference back() const
      {
        if (this->size() = 0)
        {
          std::cout << "ERROR! Vector is empty!\n";
        }
        return *(avail - 1);
      }

      void push_back(const T& t) {
        if (avail == limit)
          grow();
        unchecked_append(t);
      }

      iterator data() noexcept { return elem; }
      bool empty() const { return begin() == end();}
      size_type max_size() const { return *limit; }
      void shrink_to_fit();
      void clear() { uncreate(); }
      
      void insert(const_iterator index, const T& item) ;
      void insert(const_iterator it, int n, T value);
      void emplace(Vector<T>::const_iterator it, T args);
      void emplace_back(T arg) { push_back(arg);}
      void pop_back();
      void resize(int n, T val); 
      void resize(int n); 
      void swap(Vector<T>& v);

      // OPERATORS

      bool friend operator==(const Vector<T>& a, const Vector<T>& b)
      {
        if (a.size() != b.size())
          return false;
        else
        {
          for (size_t i = 0; i < a.size(); i++)
          if (a.elem[i] != b.elem[i])
            return false;
          return true; 
        }
      }
      bool friend operator!=(const Vector<T>& a, const Vector<T>& b)
      {
        if (a.size() != b.size())
          return true;
        else
        {
          for (size_t i = 0; i < a.size(); i++)
          if (a.elem[i] != b.elem[i])
            return true;
          return false; 
        }
      } 
      
      



    // likusi interfeiso realizacija
    private:
      iterator elem; // pakeista iš T* į iterator
      iterator avail; // pirmasis po paskutiniojo sukonstruoto Vector elementas
      iterator limit; // pakeista iš T* į iterator

      // atminties išskyrimo valdymui
      allocator_type alloc; // objektas atminties valdymui
      // išskirti atmintį (array) ir ją inicializuoti
      void create();
      void create(size_type, const T&);
      void create(const_iterator, const_iterator);
      // sunaikinti elementus array ir atlaisvinti atmintį
      void uncreate();
      // pagalbinės funkcijos push_back realizacijai
      void grow();
      void unchecked_append(const T&);

};

template <class T> 
  void Vector<T>::create() 
  {
    elem = avail = limit = nullptr;
  }
template <class T> 
  void Vector<T>::create(size_type n, const T& val) 
  {
    elem = alloc.allocate(n); // grąžina ptr į array pirmą elementą
    limit = avail = elem + n; // sustato rodykles į vietas
    std::uninitialized_fill(elem, limit, val); // inicializuoja elementus val reikšme
  }
template <class T>
  void Vector<T>::create(const_iterator i, const_iterator j) 
  {
    elem = alloc.allocate(j - i); // išskirti vietos j-i elementams
    limit = avail = std::uninitialized_copy(i, j, elem); // nukopijuoja elementus iš intervalo
  }

template <class T> 
  void Vector<T>::uncreate() 
  {
    if (elem) {
    // sunaikinti (atbuline tvarka) sukonstruotus elementus
      iterator it = avail;
      while (it != elem)
        alloc.destroy(--it);
      // atlaisvinti išskirtą atmintį. Turi būti data != nullptr
      alloc.deallocate(elem, limit - elem);
    }
    // reset'inam pointer'iuss - Vector'ius tuščias
    elem = limit = avail = nullptr;
  }

template <class T> 
  void Vector<T>::grow() 
  {
    // dvigubai daugiau vietos, nei buvo
    size_type new_size = std::max(2 * (limit - elem), difference_type(1));
    // išskirti naują vietą ir perkopijuoti egzistuojančius elementus
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(elem, avail, new_data);
    // atlaisvinti seną vietą
    uncreate();
    // reset'int rodykles į naujai išskirtą vietą
    elem = new_data;
    avail = new_avail;
    limit = elem + new_size;
  }
// tariame, kad `avail` point'ina į išskirtą, bet neinicializuotą vietą
template <class T> 
  void Vector<T>::unchecked_append(const T& val) 
  {
    alloc.construct(avail++, val);
  }

// priskyrimo operatoriaus realizacija
template <class T>
  Vector<T>& Vector<T>::operator=(const Vector& rhs)
 {
    // patikriname ar ne lygu
    if (&rhs != this) {
    // atlaisviname seną this objekto atmintį
      uncreate();
      // perkopijuojame elementus iš rhs į lhs (this)
      create(rhs.begin(), rhs.end());
    }
    return *this;
  }

template<class T> class allocator 
{
  public:
    T* allocate(size_t); // išskirti `raw` atmintį
    void deallocate(T*, size_t); // atlaisvinti atmintį
    void construct(T*, const T&); // sukonstruoti 1 objektą
    void destroy(T*); // sunaikinti 1 objektą
};

template <class T>
Vector<T>& Vector<T>::assign(const Vector& vector) {
  if (&vector != this) {
    uncreate();
    create(vector.begin(), vector.end());
  }
  return *this;
}

template <class T>
void Vector<T>::shrink_to_fit()
{
  iterator new_elements = alloc.allocate(this->size());
  iterator new_available = std::uninitialized_copy(elem, avail, new_elements);
  uncreate();
  elem = new_elements;
  avail = new_available;
  limit = avail;
}


template <class T>
void Vector<T>::insert(Vector<T>::const_iterator index, const T& item) 
{ 
        if ((index < elem) || (index > avail)) 
        {
            std::cout << "Throw exception" << std::endl;
        }

        int v = 0;
        for (auto k = begin(); k < index; k++) 
        {
            v++;
        }

        size_type new_size = (size() + 1);
        iterator new_data = alloc.allocate(new_size);
        iterator new_avail = std::uninitialized_copy(elem, avail + 1, new_data);

        for (int i = 0; i < v; i++) 
        {
            new_data[i] = elem[i];
        }

        new_data[v] = item;
        int z = v + 1;
        int g = size() + 1;

        for (int i = z; i < g; i++) 
        {
            new_data[i] = elem[i - 1];
        }

        uncreate();
        elem = new_data;
        avail = new_avail;
        limit = elem + new_size;

}

 template <class T>
 void Vector<T>::insert(Vector<T>::const_iterator it, int n, T value)
{
        if (it < elem || n < 1 || it > avail) 
            throw std::out_of_range{ "Vector::insert" };

        avail += n;
        int v = 0;

        for (auto k = begin(); k < it; k++) 
        {
            v++;
        }

        size_type new_size = (size() + n);
        iterator new_data = alloc.allocate(new_size);
        iterator new_avail = std::uninitialized_copy(elem, avail, new_data);

        for (int i = 0; i < v; i++) 
        {
            new_data[i] = elem[i];
        }

        for (int r = v; r <= v + n; r++)
            new_data[r] = value;

        int z = v + n;
        int g = size() + n;

        for (int i = z; i < g; i++) 
        {
            new_data[i] = elem[i - n];
        }

        uncreate();
        elem = new_data;
        avail = new_avail;
        limit = elem + new_size;
}

template<class T>
void Vector<T>::emplace(Vector<T>::const_iterator it, T args) 
{
        if ((it < elem) || (it > avail)) 
        {
          std::cout << "Throw exception" << std::endl;
        }
            int v = 0;

            for (auto a = begin(); a < it; a++) 
            {
                v++;
            }

    size_type new_size = (size() + 1);
    iterator new_data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(elem, avail+1, new_data);

    for(int i = 0; i< v; i++)
    {
        new_data[i] = elem[i];
    }

    new_data[v] = args;
    int z = v + 1;
    int g = size() + 1;

    for(int i = z; i< g; i++)
    {
        new_data[i] = elem[i-1];
    }
    
    uncreate();
    elem = new_data;
    avail = new_avail;
    limit = elem + new_size;
}

template<class T>
void Vector<T>::pop_back()
{
  delete (this->end() - 1);
  avail--;
}

template <class T>
void Vector<T>::resize(int n, T val)
{
    if (n > size()) 
    {
        int o = size();
        int z = n - size();
        avail += z;

        for (int i = o; i < n; i++) 
        {
            elem[i] = val;
        }
    }

    else resize(n);
}

template <class T>
void Vector<T>::resize(int n) 
{
    int k = 0;
    if(n < size())
    {
        for (int i = n; i < size(); i++) 
        {
            elem[i] = 0;
            k++;
        }
        avail -= k;
    }

    else 
    {
        int o = size();
        int z = n - size();
        avail += z;

        for (int i = o; i < n; i++) 
        {
            elem[i] = 0;
        }
    }
}

template<class T>
void Vector<T>::swap(Vector<T>& v) 
{
    iterator data2 = elem , avail2 = avail, limit2 = limit; 
    elem = v.elem;
    limit = v.limit;
    avail = v.avail;

    v.elem = data2;
    v.avail = avail2;
    v.limit = limit2;
}