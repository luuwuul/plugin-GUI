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

#ifndef DSPFILTERS_CHEBYSHEVII_H
#define DSPFILTERS_CHEBYSHEVII_H

#include "Cascade.h"
#include "Common.h"
#include "Design.h"
#include "Filter.h"
#include "PoleFilter.h"

namespace Dsp
{

/*
 * Filters with Inverse Chebyshev response characteristics
 *
 */

namespace ChebyshevII
{

    // Half-band analog prototypes (s-plane)

    class PLUGIN_API AnalogLowPass : public LayoutBase
    {
    public:
        AnalogLowPass();

        void design (const int numPoles,
                     double stopBandDb);

    private:
        int m_numPoles;
        double m_stopBandDb;
    };

    //------------------------------------------------------------------------------

    class PLUGIN_API AnalogLowShelf : public LayoutBase
    {
    public:
        AnalogLowShelf();

        void design (int numPoles,
                     double gainDb,
                     double stopBandDb);

    private:
        int m_numPoles;
        double m_stopBandDb;
        double m_gainDb;
    };

    //------------------------------------------------------------------------------

    // Factored implementations to reduce template instantiations

    struct PLUGIN_API LowPassBase : PoleFilterBase<AnalogLowPass>
    {
        void setup (int order,
                    double sampleRate,
                    double cutoffFrequency,
                    double stopBandDb);
    };

    struct PLUGIN_API HighPassBase : PoleFilterBase<AnalogLowPass>
    {
        void setup (int order,
                    double sampleRate,
                    double cutoffFrequency,
                    double stopBandDb);
    };

    struct PLUGIN_API BandPassBase : PoleFilterBase<AnalogLowPass>
    {
        void setup (int order,
                    double sampleRate,
                    double centerFrequency,
                    double widthFrequency,
                    double stopBandDb);
    };

    struct PLUGIN_API BandStopBase : PoleFilterBase<AnalogLowPass>
    {
        void setup (int order,
                    double sampleRate,
                    double centerFrequency,
                    double widthFrequency,
                    double stopBandDb);
    };

    struct PLUGIN_API LowShelfBase : PoleFilterBase<AnalogLowShelf>
    {
        void setup (int order,
                    double sampleRate,
                    double cutoffFrequency,
                    double gainDb,
                    double stopBandDb);
    };

    struct PLUGIN_API HighShelfBase : PoleFilterBase<AnalogLowShelf>
    {
        void setup (int order,
                    double sampleRate,
                    double cutoffFrequency,
                    double gainDb,
                    double stopBandDb);
    };

    struct PLUGIN_API BandShelfBase : PoleFilterBase<AnalogLowShelf>
    {
        void setup (int order,
                    double sampleRate,
                    double centerFrequency,
                    double widthFrequency,
                    double gainDb,
                    double stopBandDb);
    };

    //------------------------------------------------------------------------------

    //
    // Raw filters
    //

    template <int MaxOrder>
    struct LowPass : PoleFilter<LowPassBase, MaxOrder>
    {
    };

    template <int MaxOrder>
    struct HighPass : PoleFilter<HighPassBase, MaxOrder>
    {
    };

    template <int MaxOrder>
    struct BandPass : PoleFilter<BandPassBase, MaxOrder, MaxOrder * 2>
    {
    };

    template <int MaxOrder>
    struct BandStop : PoleFilter<BandStopBase, MaxOrder, MaxOrder * 2>
    {
    };

    template <int MaxOrder>
    struct LowShelf : PoleFilter<LowShelfBase, MaxOrder>
    {
    };

    template <int MaxOrder>
    struct HighShelf : PoleFilter<HighShelfBase, MaxOrder>
    {
    };

    template <int MaxOrder>
    struct BandShelf : PoleFilter<BandShelfBase, MaxOrder, MaxOrder * 2>
    {
    };

    //------------------------------------------------------------------------------

    //
    // Gui-friendly Design layer
    //

    namespace Design
    {

        struct TypeIBase : DesignBase
        {
            enum
            {
                NumParams = 4
            };

            static int getNumParams()
            {
                return 4;
            }

            static const ParamInfo getParamInfo_2()
            {
                return ParamInfo::defaultCutoffFrequencyParam();
            }

            static const ParamInfo getParamInfo_3()
            {
                return ParamInfo::defaultStopDbParam();
            }
        };

        template <class FilterClass>
        struct TypeI : TypeIBase, FilterClass
        {
            void setParams (const Params& params)
            {
                FilterClass::setup (int (params[1]), params[0], params[2], params[3]);
            }
        };

        struct TypeIIBase : DesignBase
        {
            enum
            {
                NumParams = 5
            };

            static int getNumParams()
            {
                return 5;
            }

            static const ParamInfo getParamInfo_2()
            {
                return ParamInfo::defaultCenterFrequencyParam();
            }

            static const ParamInfo getParamInfo_3()
            {
                return ParamInfo::defaultBandwidthHzParam();
            }

            static const ParamInfo getParamInfo_4()
            {
                return ParamInfo::defaultStopDbParam();
            }
        };

        template <class FilterClass>
        struct TypeII : TypeIIBase, FilterClass
        {
            void setParams (const Params& params)
            {
                FilterClass::setup (int (params[1]), params[0], params[2], params[3], params[4]);
            }
        };

        struct TypeIIIBase : DesignBase
        {
            enum
            {
                NumParams = 5
            };

            static int getNumParams()
            {
                return 5;
            }

            static const ParamInfo getParamInfo_2()
            {
                return ParamInfo::defaultCutoffFrequencyParam();
            }

            static const ParamInfo getParamInfo_3()
            {
                return ParamInfo::defaultGainParam();
            }

            static const ParamInfo getParamInfo_4()
            {
                return ParamInfo::defaultStopDbParam();
            }
        };

        template <class FilterClass>
        struct TypeIII : TypeIIIBase, FilterClass
        {
            void setParams (const Params& params)
            {
                FilterClass::setup (int (params[1]), params[0], params[2], params[3], params[4]);
            }
        };

        struct TypeIVBase : DesignBase
        {
            enum
            {
                NumParams = 6
            };

            static int getNumParams()
            {
                return 6;
            }

            static const ParamInfo getParamInfo_2()
            {
                return ParamInfo::defaultCenterFrequencyParam();
            }

            static const ParamInfo getParamInfo_3()
            {
                return ParamInfo::defaultBandwidthHzParam();
            }

            static const ParamInfo getParamInfo_4()
            {
                return ParamInfo::defaultGainParam();
            }

            static const ParamInfo getParamInfo_5()
            {
                return ParamInfo::defaultStopDbParam();
            }
        };

        template <class FilterClass>
        struct TypeIV : TypeIVBase, FilterClass
        {
            void setParams (const Params& params)
            {
                FilterClass::setup (int (params[1]), params[0], params[2], params[3], params[4], params[5]);
            }
        };

        // Factored kind and name

        struct LowPassDescription
        {
            static Kind getKind()
            {
                return kindLowPass;
            }
            static const char* getName()
            {
                return "Chebyshev II Low Pass";
            }
        };

        struct HighPassDescription
        {
            static Kind getKind()
            {
                return kindHighPass;
            }
            static const char* getName()
            {
                return "Chebyshev II High Pass";
            }
        };

        struct BandPassDescription
        {
            static Kind getKind()
            {
                return kindHighPass;
            }
            static const char* getName()
            {
                return "Chebyshev II Band Pass";
            }
        };

        struct BandStopDescription
        {
            static Kind getKind()
            {
                return kindHighPass;
            }
            static const char* getName()
            {
                return "Chebyshev II Band Stop";
            }
        };

        struct LowShelfDescription
        {
            static Kind getKind()
            {
                return kindLowShelf;
            }
            static const char* getName()
            {
                return "Chebyshev II Low Shelf";
            }
        };

        struct HighShelfDescription
        {
            static Kind getKind()
            {
                return kindHighShelf;
            }
            static const char* getName()
            {
                return "Chebyshev II High Shelf";
            }
        };

        struct BandShelfDescription
        {
            static Kind getKind()
            {
                return kindBandShelf;
            }
            static const char* getName()
            {
                return "Chebyshev II Band Shelf";
            }
        };

        // This glues on the Order parameter
        template <int MaxOrder,
                  template <class>
                  class TypeClass,
                  template <int>
                  class FilterClass>
        struct OrderBase : TypeClass<FilterClass<MaxOrder>>
        {
            const ParamInfo getParamInfo_1() const
            {
                return ParamInfo (idOrder, "Order", "Order", 1, MaxOrder, 2, &ParamInfo::Int_toControlValue, &ParamInfo::Int_toNativeValue, &ParamInfo::Int_toString);
            }
        };

        //------------------------------------------------------------------------------

        //
        // Design Filters
        //

        template <int MaxOrder>
        struct LowPass : OrderBase<MaxOrder, TypeI, ChebyshevII::LowPass>,
                         LowPassDescription
        {
        };

        template <int MaxOrder>
        struct HighPass : OrderBase<MaxOrder, TypeI, ChebyshevII::HighPass>,
                          HighPassDescription
        {
        };

        template <int MaxOrder>
        struct BandPass : OrderBase<MaxOrder, TypeII, ChebyshevII::BandPass>,
                          BandPassDescription
        {
        };

        template <int MaxOrder>
        struct BandStop : OrderBase<MaxOrder, TypeII, ChebyshevII::BandStop>,
                          BandStopDescription
        {
        };

        template <int MaxOrder>
        struct LowShelf : OrderBase<MaxOrder, TypeIII, ChebyshevII::LowShelf>,
                          LowShelfDescription
        {
        };

        template <int MaxOrder>
        struct HighShelf : OrderBase<MaxOrder, TypeIII, ChebyshevII::HighShelf>,
                           HighShelfDescription
        {
        };

        template <int MaxOrder>
        struct BandShelf : OrderBase<MaxOrder, TypeIV, ChebyshevII::BandShelf>,
                           BandShelfDescription
        {
        };

    } // namespace Design

} // namespace ChebyshevII

} // namespace Dsp

#endif
