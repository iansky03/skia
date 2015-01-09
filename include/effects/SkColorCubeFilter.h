/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkColorCubeFilter_DEFINED
#define SkColorCubeFilter_DEFINED

#include "SkColorFilter.h"
#include "SkData.h"

class SK_API SkColorCubeFilter : public SkColorFilter {
public:
    /** cubeData must containt a 3D data in the form of cube of the size:
     *  cubeDimension * cubeDimension * cubeDimension * sizeof(SkColor)
     *  This cube contains a transform where (x,y,z) maps to the (r,g,b).
     *  The alpha components of the colors are ignored (treated as 0xFF).
     */
    static SkColorFilter* Create(SkData* cubeData, int cubeDimension);

    void filterSpan(const SkPMColor src[], int count, SkPMColor[]) const SK_OVERRIDE;
    uint32_t getFlags() const SK_OVERRIDE;

#if SK_SUPPORT_GPU
   GrFragmentProcessor* asFragmentProcessor(GrContext*) const SK_OVERRIDE;
#endif

    SK_TO_STRING_OVERRIDE()
    SK_DECLARE_PUBLIC_FLATTENABLE_DESERIALIZATION_PROCS(SkColorCubeFilter)

protected:
    SkColorCubeFilter(SkData* cubeData, int cubeDimension);
    void flatten(SkWriteBuffer&) const SK_OVERRIDE;

private:
    /** The cache is initialized on-demand when getProcessingLuts is called.
     */
    class ColorCubeProcesingCache {
    public:
        ColorCubeProcesingCache(int cubeDimension);

        void getProcessingLuts(const int* (*colorToIndex)[2],
                               const SkScalar* (*colorToFactors)[2],
                               const SkScalar** colorToScalar);

        int cubeDimension() const { return fCubeDimension; }

    private:
        // Working pointers. If any of these is NULL,
        // we need to recompute the corresponding cache values.
        int* fColorToIndex[2];
        SkScalar* fColorToFactors[2];
        SkScalar* fColorToScalar;

        SkAutoMalloc fLutStorage;

        const int fCubeDimension;

        // Make sure we only initialize the caches once.
        SkMutex fLutsMutex;
        bool fLutsInited;

        static void initProcessingLuts(ColorCubeProcesingCache* cache);
    };

    SkAutoDataUnref fCubeData;
    int32_t fUniqueID;

    mutable ColorCubeProcesingCache fCache;

    typedef SkColorFilter INHERITED;
};

#endif
