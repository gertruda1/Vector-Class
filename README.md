# Vector-Class

Savadarbė vektoriaus klasė, panaši į std::vector.

## 5 klasės funkcijų pavyzdžiai

 * **pop_back** Funkcija, kuri sunaikina paskutinį konteineryje esantį elementą. Funkcijai nėra duodami parametrai.
 ```c++
 template<class T>
void Vector<T>::pop_back()
{
  delete (this->end() - 1);
  avail--;
}
```
* **operatror ==** Bool tipo operatorius, kuris gražina true, jei du vektoriai yra lygūs, arba false, jei vektoriuose nors vienas elementas nesutampa.
```c++
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
```
 * **shrink_to_fit** Funkcija, kuri sumažina vektoriaus capacity (rezervuotą, tačiau nepanaudojamą atmintį). Neturi parametrų.
 ```c++
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
```
 * **assign** Funkcija, kuri pakeičia jau esamus elementus kito vektoriaus elementais ir atitinkamai modifikuoja jos dydį. Nurodomas Vector tipo parametras.
 ```c++
 template <class T>
Vector<T>& Vector<T>::assign(const Vector& vector) {
  if (&vector != this) {
    uncreate();
    create(vector.begin(), vector.end());
  }
  return *this;
}
```
 * **max_size** Funkcija, kuri gražina didžiausią galimą elementų skaičių, kurį gali saugoti vektorius. Funkcijai nenurodomi jokie parametrai.
 ```c++
 size_type max_size() const { return *limit; }
 ```
 
 ## Spartos analizė (laikas matuojamas sekundėmis)
 
 Įrangos parametrai:

| CPU  | RAM | SSD |
| ----- | ------|------ |
| Intel Core i5 8th Gen 1,6 GHz | 8GB  | 256 GB |
 
Spartos analizė naudojant std::vector ir sukurtą Vector konteinerius. Lyginama, kiek laiko užtrunka std::vector tipo ir Vector tipo tuščių konteinerių užpildymas, naudojant push_back() funkciją.

 |Konteineris|10000|100000|1000000|10000000|100000000|
 |-----------|-----|------|--------|-------|---------|
 |std::vector|0|0.002174|0.052826|0.323828|2.71736|
 |Vector|0|0.004478|0.057648|0.417097|3.29725|
 
Konteinerių (Vector ir std::vector) atminties perskirstymai užpildant 100000000 elementų. Perskirstymas įvyksta tada, kai yra patenkinama sąlyga: capacity() == size().

 |Konteineris|Perskirstymai|
 |-----------|-------------|
 |std::vector|27|
 |Vector|27|
 
Spartos analizė naudojant duomenų failą su 100000 studentų įrašų, lyginant kiek laiko užtrunka programos veikimas naudojant std::vector ir Vector realizacijas.

 |Konteineris|100000|
 |-----------|------|
 |std::vector|10.8944|
 |Vector|11.1402|
 
 ## Išvados
 
 Taigi, ar verta naudoti naujai sukurtą Vector klasę vietoj std::vector?
 **Asakymas - ne**. Vector klasė nėra tokia efektyvi, kaip std::vector. std::vector yra patikrintas ir tvarkingas standartas, kurį išties yra patogu naudoti, tad geriausia tiesiog pasilikti prie jo.
 
