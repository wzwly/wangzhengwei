#ifndef TYPEDEF_H
#define TYPEDEF_H

template<class T>
class POINT_T
{
public:
    POINT_T(){};
    POINT_T(T x_, T y_) {x = x_; y = y_;}
    inline POINT_T<T> operator + (POINT_T<T>  p_);
    inline POINT_T<T> operator - (POINT_T<T>  p_);

 public:
    T x;
    T y;
};


template<class T>
 POINT_T<T> POINT_T<T>::operator + (POINT_T<T>  p_)
{
     POINT_T<T> _p( x + p_.x, y + p_.y);
     return _p;
}

 template<class T>
  POINT_T<T> POINT_T<T>::operator - (POINT_T<T>  p_)
 {
         POINT_T<T> _p( x - p_.x, y - p_.y);
         return _p;
 }

#endif // TYPEDEF_H
