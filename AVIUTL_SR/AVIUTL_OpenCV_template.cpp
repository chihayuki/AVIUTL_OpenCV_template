#include <lua.hpp>
#include <windows.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include <opencv2/opencv.hpp>

using namespace std;

struct Pixel_RGBA {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
};

int template_func(lua_State* L) {
    Pixel_RGBA* pixels = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));
    int w = static_cast<int>(lua_tointeger(L, 2));
    int h = static_cast<int>(lua_tointeger(L, 3));
    cv::Mat aa;
    aa = cv::Mat::zeros(1,1, CV_8U);
  
    vector<unsigned char> r,g,b,a;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int index = x + w * y;
            r.push_back(pixels[index].r);
            g.push_back(pixels[index].g);
            b.push_back(pixels[index].b);
            a.push_back(pixels[index].a);
        }
    }

    cv::Mat src, bMat(b), gMat(g), rMat(r);
    bMat = bMat.reshape(1, h);
    gMat = gMat.reshape(1, h);
    rMat = rMat.reshape(1, h);

    vector<cv::Mat> mat;
    mat.push_back(bMat);
    mat.push_back(gMat);
    mat.push_back(rMat);
    cv::merge(mat, src);

    cv::split(src, mat);

    std::vector<int> rVec, bVec, gVec;

    mat[0].reshape(0, 1).copyTo(bVec);
    mat[1].reshape(0, 1).copyTo(gVec);
    mat[2].reshape(0, 1).copyTo(rVec);
    
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int index = x + w * y;
            //char message[63];
            //sprintf_s(message, "index %d", index);
            //OutputDebugStringA(message);
            pixels[index].r = rVec[index];
            pixels[index].g = gVec[index];
            pixels[index].b = bVec[index];
            pixels[index].a;
        }
    }
    
    OutputDebugStringA("func end");
    return 0;
}

static luaL_Reg functions[] = {
    { "template_func", template_func },
    { nullptr, nullptr }
};

extern "C" {
    __declspec(dllexport) int luaopen_AVIUTL_OpenCV_template(lua_State* L) {
        luaL_register(L, "AVIUTL_OpenCV_template", functions);
        return 1;
    }
}


