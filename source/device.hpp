//  MIT License
//  
//  Copyright (c) 2018 Jonathan R. Madsen
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#ifndef device_hpp_
#define device_hpp_

#include "globals.hpp"

class device
{
public:
    // Constructor and Destructors
    device(int _device_num = -1);
    device(device_t _device);
    // Virtual destructors are required by abstract classes 
    // so add it by default, just in case
    virtual ~device();

public:
    // implicit casting
    operator device_t() { return m_device; }
    operator const device_t&() const { return m_device; }
    // explicit get
    device_t& get() { return m_device; }
    const device_t& get() const { return m_device; }

protected:
    // Protected variables
    device_t m_device;

};

//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//

#endif

