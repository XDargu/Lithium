#ifndef _POINTER_MATH_H_
#define _POINTER_MATH_H_

namespace pointer_math
{
    inline void*
        AlignForward(void* lAddress, sUInt8 lAlignment)
    {
        // (address + (alignment - 1)) & (~(alignment - 1))
        return (void*)((reinterpret_cast<sUPtr>(lAddress) + static_cast<sUPtr>(lAlignment - 1)) & static_cast<sUPtr>(~(lAlignment - 1)));
    }

    inline sUInt8
        AlignForwardPadding(
            const void* lAddress,
            sUInt8      lAlignment)
    {
        sUInt8 lPadding = lAlignment - (reinterpret_cast<sUPtr>(lAddress) & static_cast<sUPtr>(lAlignment - 1));

        if (lPadding == lAlignment)
            return 0; //already aligned

        return lPadding;
    }

    inline sUInt8
        AlignForwardAdjustment(
            const void* lAddress,
            sUInt8      lAlignment)
    {
        sUInt8 lAdjustment = lAlignment - (reinterpret_cast<sUPtr>(lAddress) & static_cast<sUPtr>(lAlignment - 1));
        if (lAdjustment == lAlignment)
            return 0; //already aligned
        return lAdjustment;
    }

    inline sUInt8
        AlignForwardAdjustmentWithHeader(
            const void* lAddress,
            sUInt8      lAlignment,
            sUInt8      headerSize)
    {
        sUInt8 lAdjustment = AlignForwardAdjustment(lAddress, lAlignment);
        sUInt8 lNeededSpace = headerSize;
        if (lAdjustment < lNeededSpace)
        {
            lNeededSpace -= lAdjustment;
            //Increase adjustment to fit header
            lAdjustment += lAlignment * (lNeededSpace / lAlignment);
            if (lNeededSpace % lAlignment > 0)
                lAdjustment += lAlignment;
        }
        return lAdjustment;
    }

    inline void*
        Add(void* lAddress, size_t x)
    {
        return (void*)(reinterpret_cast<sUPtr>(lAddress) + x);
    }

    inline const void*
        Add(const void* lAddress, size_t x)
    {
        return (const void*)(reinterpret_cast<sUPtr>(lAddress) + x);
    }

    inline void*
        Subtract(void* lAddress, size_t x)
    {
        return (void*)(reinterpret_cast<sUPtr>(lAddress) - x);
    }

    inline const void*
        Subtract(const void* lAddress, size_t x)
    {
        return (const void*)(reinterpret_cast<sUPtr>(lAddress) - x);
    }
}

#endif