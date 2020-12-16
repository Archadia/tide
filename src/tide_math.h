/* date = December 16th 2020 1:54 pm */

#ifndef TIDE_MATH_H
#define TIDE_MATH_H

/*
 * TODO(kroma): Think about SIMD optimisation
* TODO(kroma): MAT3F, MAT2F, & more functions
*/

namespace tide
{
    // VEC4F
    
    struct VEC4F
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };
            float D[4];
        };
    };
    
    inline VEC4F operator+(VEC4F a, VEC4F b)
    {
        VEC4F vec;
        vec.x = a.x + b.x;
        vec.y = a.y + b.y;
        vec.z = a.z + b.z;
        vec.w = a.w + b.w;
        return vec;
    }
    
    inline VEC4F operator-(VEC4F a, VEC4F b)
    {
        VEC4F vec;
        vec.x = a.x - b.x;
        vec.y = a.y - b.y;
        vec.z = a.z - b.z;
        vec.w = a.w - b.w;
        return vec;
    }
    
    inline VEC4F operator*(VEC4F a, VEC4F b)
    {
        VEC4F vec;
        vec.x = a.x * b.x;
        vec.y = a.y * b.y;
        vec.z = a.z * b.z;
        vec.w = a.w * b.w;
        return vec;
    }
    
    inline VEC4F operator/(VEC4F a, VEC4F b)
    {
        VEC4F vec;
        vec.x = a.x / b.x;
        vec.y = a.y / b.y;
        vec.z = a.z / b.z;
        vec.w = a.w / b.w;
        return vec;
    }
    
    inline void operator+=(VEC4F& a, VEC4F b)
    {
        a = a + b;
    }
    
    inline void operator-=(VEC4F& a, VEC4F b)
    {
        a = a - b;
    }
    
    inline void operator*=(VEC4F& a, VEC4F b)
    {
        a = a * b;
    }
    
    inline void operator/=(VEC4F& a, VEC4F b)
    {
        a = a / b;
    }
    
    // VEC3F
    
    struct VEC3F
    {
        union
        {
            struct
            {
                float x;
                float y;
                float z;
            };
            float D[3];
        };
    };
    
    inline VEC3F operator+(VEC3F a, VEC3F b)
    {
        VEC3F vec;
        vec.x = a.x + b.x;
        vec.y = a.y + b.y;
        vec.z = a.z + b.z;
        return vec;
    }
    
    inline VEC3F operator-(VEC3F a, VEC3F b)
    {
        VEC3F vec;
        vec.x = a.x - b.x;
        vec.y = a.y - b.y;
        vec.z = a.z - b.z;
        return vec;
    }
    
    inline VEC3F operator*(VEC3F a, VEC3F b)
    {
        VEC3F vec;
        vec.x = a.x * b.x;
        vec.y = a.y * b.y;
        vec.z = a.z * b.z;
        return vec;
    }
    
    inline VEC3F operator/(VEC3F a, VEC3F b)
    {
        VEC3F vec;
        vec.x = a.x / b.x;
        vec.y = a.y / b.y;
        vec.z = a.z / b.z;
        return vec;
    }
    
    inline void operator+=(VEC3F& a, VEC3F b)
    {
        a = a + b;
    }
    
    inline void operator-=(VEC3F& a, VEC3F b)
    {
        a = a - b;
    }
    
    inline void operator*=(VEC3F& a, VEC3F b)
    {
        a = a * b;
    }
    
    inline void operator/=(VEC3F& a, VEC3F b)
    {
        a = a / b;
    }
    
    // VEC2F
    
    struct VEC2F
    {
        union
        {
            struct
            {
                float x;
                float y;
            };
            float D[2];
        };
    };
    
    inline VEC2F operator+(VEC2F a, VEC2F b)
    {
        VEC2F vec;
        vec.x = a.x + b.x;
        vec.y = a.y + b.y;
        return vec;
    }
    
    inline VEC2F operator-(VEC2F a, VEC2F b)
    {
        VEC2F vec;
        vec.x = a.x - b.x;
        vec.y = a.y - b.y;
        return vec;
    }
    
    inline VEC2F operator*(VEC2F a, VEC2F b)
    {
        VEC2F vec;
        vec.x = a.x * b.x;
        vec.y = a.y * b.y;
        return vec;
    }
    
    inline VEC2F operator/(VEC2F a, VEC2F b)
    {
        VEC2F vec;
        vec.x = a.x / b.x;
        vec.y = a.y / b.y;
        return vec;
    }
    
    inline void operator+=(VEC2F& a, VEC2F b)
    {
        a = a + b;
    }
    
    inline void operator-=(VEC2F& a, VEC2F b)
    {
        a = a - b;
    }
    
    inline void operator*=(VEC2F& a, VEC2F b)
    {
        a = a * b;
    }
    
    inline void operator/=(VEC2F& a, VEC2F b)
    {
        a = a / b;
    }
    
    // MAT4F
    
    struct MAT4F
    {
        union
        {
            float d[16];
        };
        
        float operator[](int i)
        {
            return d[i];
        }
    };
    
    inline VEC4F operator*(MAT4F a, VEC4F b)
    {
        VEC4F vec;
        vec.x = a.d[0] * b.x + a.d[1] * b.y + a.d[2] * b.z + a.d[3] * b.w;
        vec.y = a.d[4] * b.x + a.d[5] * b.y + a.d[6] * b.z + a.d[7] * b.w;
        vec.z = a.d[8] * b.x + a.d[9] * b.y + a.d[10]* b.z + a.d[11]* b.w;
        vec.w = a.d[12]* b.x + a.d[13]* b.y + a.d[14]* b.z + a.d[15]* b.w;
        return vec;
    }
    
    inline MAT4F operator*(MAT4F a, MAT4F b)
    {
        MAT4F mat;
        mat.d[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
        mat.d[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
        mat.d[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
        mat.d[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];
        
        mat.d[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
        mat.d[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
        mat.d[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
        mat.d[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];
        
        mat.d[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
        mat.d[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
        mat.d[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
        mat.d[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];
        
        mat.d[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
        mat.d[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
        mat.d[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
        mat.d[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
        return mat;
    }
    
    inline void operator*=(MAT4F& a, MAT4F b)
    {
        a = a * b;
    }
    
}
#endif //TIDE_MATH_H
