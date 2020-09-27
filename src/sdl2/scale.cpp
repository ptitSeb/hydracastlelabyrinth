#include "scale.h"
#include "../xBRZ/xbrz.h"
#include <string.h>

void xbrz_scale(void* src, void* dst, int width, int height, int scale)
{
    if(scale==1) {
        memcpy(dst, src, width*height*4);
    } else if (scale>1 && scale <= 6) {
        xbrz::scale(scale, (const uint32_t*)src, (uint32_t*)dst, width, height, xbrz::ColorFormat::ARGB);
    } else {
        xbrz::nearestNeighborScale((const uint32_t*)src, width, height, (uint32_t*)dst, width*scale, height*scale);
    }
}

void dumb_scale(void* src, void* dst, int width, int height, int scale)
{
    if(scale==1) {
        memcpy(dst, src, width*height*4);
    } else {
        // manual upscaling
        int stride = width;
        int sstride = width*scale;
        uint32_t* psrc = (uint32_t*)src;
        uint32_t* pdst = (uint32_t*)dst;
        for(int y=0; y<height; ++y) {
            // do the 1st line
            for(int x=0; x<width; ++x) {
                uint32_t pixel = psrc[y*stride+x];
                // put that pixel scale horizontally
                for (int s=0; s<scale; ++s) {
                    pdst[(y*sstride+x)*scale+s] = pixel;
                }
            }
            // duplicate that line to scale vertically
            for (int s=1; s<scale; ++s) {
                memcpy(&pdst[(y*scale+s)*sstride], &pdst[y*scale*sstride], sstride*4);
            }
        }
    }
    // done...
}