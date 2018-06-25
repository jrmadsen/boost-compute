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
//
//

#include "device.hpp"

//============================================================================//

BOOST_COMPUTE_FUNCTION(float, custom_square, (float x),
{
    return x*x;
});

//============================================================================//

typedef std::vector<std::pair<int, int>> range_t;

//============================================================================//

template <typename _Tp>
void print_vector(const std::vector<_Tp>& vec,
                  const range_t& which,
                  bool as_fixed = true,
                  const uint16_t& nbreak = 8,
                  const uint16_t& nprec = 3,
                  const uint16_t& nwidth = 8)
{
    std::stringstream ss;
    ss.precision(nprec);
    ss << ((as_fixed) ? (std::fixed) : (std::scientific));

    auto _prefix = [=] (int i)
    {
        std::stringstream _ss;
        _ss << "[" << i << "]";
        return _ss.str();
    };

    bool _broke = false;
    for(uint64_t i = 0; i < vec.size(); ++i)
    {
        bool inside = false;
        _broke = false;

        for(const auto& itr : which)
        {
            if(i >= itr.first && i < itr.second)
            {
                inside = true;
                break;
            }
        }

        if(inside)
        {
            if(i == 0 || (i)%(nbreak) == 0)
                ss << std::setw(nwidth) << _prefix(i) << " ";

            ss << std::setw(nwidth) << vec.at(i) << " ";
            if(i > 0 && (i+1)%(nbreak) == 0)
            {
                _broke = true;
                ss << std::endl;
            }
        }
        else
        {
            for(const auto& itr : which)
                if(i == itr.second)
                {
                    ss << std::setw(nwidth) << "..." << std::endl;
                    _broke = true;
                }
            continue;
        }
    }
    if(!_broke)
        ss << std::endl;
    std::cout << ss.str() << std::endl;
}

//============================================================================//

int main(int argc, char** argv)
{
    tim::manager* manager = tim::manager::instance();
    tim::format::timer::set_default_precision(6);

    int ndevice = -1;
    int nvector = 10000;
    int nprint  = 8;

    if(argc > 1)
        ndevice = atoi(argv[1]);

    if(argc > 2)
        nvector = atoi(argv[2]);

    if(argc > 3)
        nprint = atoi(argv[3]);

    std::random_device rand_device;
    // Choose a random mean between 1 and 6
    std::mt19937 engine(rand_device());
    auto random_function = [&] () { return std::generate_canonical<float, 10>(engine); };

    // get default device and setup context
    device _device(ndevice);
    context_t context(_device);
    command_queue_t queue(context, _device);

    // generate random data on the host
    std::vector<float> host_vector(nvector);
    std::generate(host_vector.begin(), host_vector.end(), random_function);

    // create a vector on the device
    boost::compute::vector<float> device_vector(host_vector.size(), context);

    // transfer data from the host to the device
    {
        TIMEMORY_AUTO_TIMER("[copy-to-device]");
        boost::compute::copy(
            host_vector.begin(), host_vector.end(), device_vector.begin(), queue);
    }


    range_t _range = { std::make_pair(0, nprint),
                       std::make_pair(nvector/2 - nprint/2, nvector/2 + nprint/2),
                       std::make_pair(nvector-nprint, nvector) };

    print_vector(host_vector, _range);

    // calculate the square-root of each element in-place
    {
        TIMEMORY_AUTO_TIMER("[transform-operation]");
        boost::compute::transform(
                    device_vector.begin(),
                    device_vector.end(),
                    device_vector.begin(),
                    //boost::compute::sqrt<float>(),
                    custom_square,
                    queue);
    }

    // copy values back to the host
    {
        TIMEMORY_AUTO_TIMER("[copy-from-device]");
        boost::compute::copy(
                    device_vector.begin(),
                    device_vector.end(),
                    host_vector.begin(),
                    queue);
    }

    print_vector(host_vector, _range);

    manager->report(std::cout, true);

    return 0;
}

//============================================================================//
