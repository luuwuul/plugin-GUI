/*******************************************************************************

"A Collection of Useful C++ Classes for Digital Signal Processing"
 By Vincent Falco

Official project location:
http://code.google.com/p/dspfilterscpp/

See Documentation.cpp for contact information, notes, and bibliography.

--------------------------------------------------------------------------------

License: MIT License (http://www.opensource.org/licenses/mit-license.php)
Copyright (c) 2009 by Vincent Falco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*******************************************************************************/

#ifndef DSPFILTERS_DESIGN_H
#define DSPFILTERS_DESIGN_H

#include "Common.h"
#include "Params.h"

namespace Dsp
{

struct DesignBase
{
    // Sampling rate is the first param for every Design filter
    static const ParamInfo getParamInfo_0()
    {
        return ParamInfo::defaultSampleRateParam();
    }

    // These should never get called
    static const ParamInfo getParamInfo_1()
    {
        return ParamInfo();
    }
    static const ParamInfo getParamInfo_2()
    {
        return ParamInfo();
    }
    static const ParamInfo getParamInfo_3()
    {
        return ParamInfo();
    }
    static const ParamInfo getParamInfo_4()
    {
        return ParamInfo();
    }
    static const ParamInfo getParamInfo_5()
    {
        return ParamInfo();
    }
    static const ParamInfo getParamInfo_6()
    {
        return ParamInfo();
    }
    static const ParamInfo getParamInfo_7()
    {
        return ParamInfo();
    }
};

} // namespace Dsp

#endif
