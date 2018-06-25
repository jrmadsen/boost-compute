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

#ifndef globals_hpp_
#define globals_hpp_

//----------------------------------------------------------------------------//

// C headers
#include <cstdlib>
#include <cstdint>

// C++ headers
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

// Boost headers
#include <boost/compute/core.hpp>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/algorithm/transform.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/functional/math.hpp>

// TiMemory headers
#include <timemory/timemory.hpp>

//============================================================================//
// typedefs

typedef boost::compute::device          device_t;
typedef boost::compute::context         context_t;
typedef boost::compute::command_queue   command_queue_t;

//============================================================================//

device_t& get_device()
{
    static device_t _instance = boost::compute::system::default_device();
    return _instance;
}

//============================================================================//

void selected_message(device_t device)
{
    // print the device's name
    std::cout << "--> selected device: \"" << device.name() << "\""
              << " (platform: " << device.platform().name() << ")"
              << std::endl;
}

//============================================================================//

device_t get_selection(int selection = -1)
{
    auto ndevices = boost::compute::system::device_count();
    auto devices = boost::compute::system::devices();

    if(selection < 0)
    {
        std::cout << "Device count: " << ndevices << std::endl;
        std::stringstream ss;
        ss << "Please select the default device:" << std::endl;
        int n = 0;
        for(auto itr : devices)
            ss << "\t[" << n++ << "] " << itr.name() << std::endl;
        std::cout << ss.str();
        std::cin >> selection;
    }

    device_t _device = devices.at(selection % devices.size());
    selected_message(_device);
    return _device;
}

//----------------------------------------------------------------------------//

#endif

