///////////////////////////////////////////////////////////////////////////
// Copyright (c) 2013 Academy of Motion Picture Arts and Sciences 
// ("A.M.P.A.S."). Portions contributed by others as indicated.
// All rights reserved.
// 
// A worldwide, royalty-free, non-exclusive right to copy, modify, create
// derivatives, and use, in source and binary forms, is hereby granted, 
// subject to acceptance of this license. Performance of any of the 
// aforementioned acts indicates acceptance to be bound by the following 
// terms and conditions:
//
//  * Copies of source code, in whole or in part, must retain the 
//    above copyright notice, this list of conditions and the 
//    Disclaimer of Warranty.
//
//  * Use in binary form must retain the above copyright notice, 
//    this list of conditions and the Disclaimer of Warranty in the
//    documentation and/or other materials provided with the distribution.
//
//  * Nothing in this license shall be deemed to grant any rights to 
//    trademarks, copyrights, patents, trade secrets or any other 
//    intellectual property of A.M.P.A.S. or any contributors, except 
//    as expressly stated herein.
//
//  * Neither the name "A.M.P.A.S." nor the name of any other 
//    contributors to this software may be used to endorse or promote 
//    products derivative of or based on this software without express 
//    prior written permission of A.M.P.A.S. or the contributors, as 
//    appropriate.
// 
// This license shall be construed pursuant to the laws of the State of 
// California, and any disputes related thereto shall be subject to the 
// jurisdiction of the courts therein.
//
// Disclaimer of Warranty: THIS SOFTWARE IS PROVIDED BY A.M.P.A.S. AND 
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, 
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT ARE DISCLAIMED. IN NO 
// EVENT SHALL A.M.P.A.S., OR ANY CONTRIBUTORS OR DISTRIBUTORS, BE LIABLE 
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, RESITUTIONARY, 
// OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
// THE POSSIBILITY OF SUCH DAMAGE.
//
// WITHOUT LIMITING THE GENERALITY OF THE FOREGOING, THE ACADEMY 
// SPECIFICALLY DISCLAIMS ANY REPRESENTATIONS OR WARRANTIES WHATSOEVER 
// RELATED TO PATENT OR OTHER INTELLECTUAL PROPERTY RIGHTS IN THE ACADEMY 
// COLOR ENCODING SYSTEM, OR APPLICATIONS THEREOF, HELD BY PARTIES OTHER 
// THAN A.M.P.A.S., WHETHER DISCLOSED OR UNDISCLOSED.
///////////////////////////////////////////////////////////////////////////

#ifndef _MATHOPS_h__
#define _MATHOPS_h__

#include "define.h"

// Non-class functions
double invertD(double val) {
    assert (val != 0.0);
    
    return 1.0/val;
}


template<typename T>
bool isSquare(const vector< vector<T> > vm){
    FORI(vm.size()){
        if (vm[i].size() != vm.size())
            return 0;
    }
    
    return 1;
}

template <typename T>
T cross(const vector <T> &vectorA, const vector <T> &vectorB) {
    assert (vectorA.size() == 2 && vectorB.size() == 2 );
    return vectorA[0] * vectorB[1] - vectorA[1] * vectorB[0];
}

template <typename T>
const vector < vector <T> > repmat(const T* data[], int row, int col) {
    vector < vector <T> > vect(row, vector<T>(col));
    FORI(row) {
        FORJ(col) {
            vect[i][j] = data[i][j];
        }
    }
    
    const vector < vector <T> > cvect(vect);
    return cvect;
}

template <typename T>
const vector <T> repmat1d(const T data[], int row, int col) {
    vector <T> vect(row*col);
    FORI(row) {
        FORJ(col) {
            vect[i*col + j] = data[i];
        }
    }
    
    const vector < T > cvect(vect);
    return cvect;
}

template <typename T>
const vector < vector <T> > repmat2dr(const T data[], int row, int col) {
    vector < vector <T> > vect(row, vector<T>(col));
    FORI(row) {
        FORJ(col) {
            vect[i][j] = data[i];
        }
    }
    
    const vector < vector <T> > cvect(vect);
    return cvect;
}

template <typename T>
const vector < vector <T> > repmat2dc(const T data[], int row, int col) {
    vector < vector <T> > vect(row, vector<T>(col));
    FORI(row) {
        FORJ(col) {
            vect[i][j] = data[j];
        }
    }
    
    const vector < vector <T> > cvect(vect);
    return cvect;
}

template <typename T>
vector< vector<T> > invertVM3(const vector< vector<T> > &vMtx)
{
    assert(vMtx.size() == 3 &&
           isSquare(vMtx));
    
    vector <T> mtx(9);
    FORI(3)
        FORJ(3)
            mtx[i*3+j] = vMtx[i][j];
    
    T CinvMtx[] = {
        0.0 - mtx[5] * mtx[7] + mtx[4] * mtx[8],
        0.0 + mtx[2] * mtx[7] - mtx[1] * mtx[8],
        0.0 - mtx[2] * mtx[4] + mtx[1] * mtx[5],
        0.0 + mtx[5] * mtx[6] - mtx[3] * mtx[8],
        0.0 - mtx[2] * mtx[6] + mtx[0] * mtx[8],
        0.0 + mtx[2] * mtx[3] - mtx[0] * mtx[5],
        0.0 - mtx[4] * mtx[6] + mtx[3] * mtx[7],
        0.0 + mtx[1] * mtx[6] - mtx[0] * mtx[7],
        0.0 - mtx[1] * mtx[3] + mtx[0] * mtx[4]
    };
    
    vector<T> invMtx(CinvMtx, CinvMtx+sizeof(CinvMtx) / sizeof(T));
    T det = mtx[0] * invMtx[0] + mtx[1] * invMtx[3] + mtx[2] * invMtx[6];
    
    // pay attention to this
    assert (det != 0);
    FORI(9)
        invMtx[i] /= det;
    
    vector< vector<T> > vMtxR(3, vector<T>(3));
    FORI(3)
        FORJ(3)
            vMtxR[i][j] = invMtx[3*i+j];
    
    return vMtxR;
}

template <typename T>
vector < vector<T> > diagVM(const vector<T>& vct)
{
    assert(vct.size() != 0);
    vector < vector<T> > vctdiag(vct.size(), vector<T>(vct.size(), 0.0));
    
    FORI(vct.size())
        vctdiag[i][i] = vct[i];
    
    return vctdiag;
}

template <typename T>
void transpose(T* mtx[], int row, int col)
{
    assert (row != 0 && col != 0);
    
    FORI(row) {
        FORJ(col) {
            mtx[i][j] ^= mtx[j][i];
            mtx[i][j] ^= (mtx[j][i] ^= mtx[i][j]);
        }
    }
    
    return;
}

template <typename T>
vector< vector<T> > transposeVec(const vector< vector<T> > vMtx)
{
    assert(vMtx.size() != 0
           && vMtx[0].size() != 0);
    
    int row = vMtx.size();
    int col = vMtx[0].size();
    
    assert (row != 0 && col != 0);
    vector< vector<T> > vTran(col, vector<T>(row));
    
    FORI(row) {
        FORJ(col) {
            vTran[j][i] = vMtx[i][j];
        }
    }
    
    return vTran;
}

template <typename T>
T sumVector(const vector<T>& vct)
{
    return accumulate(vct.begin(), vct.end(), static_cast<T>(0));
}

template <typename T>
vector<T> subVectors(const vector<T>& vct1, const vector<T>& vct2)
{
    assert(vct1.size() == vct2.size());

    vector<T> vct3(vct1.size(), 1.0);
    transform(vct1.begin(), vct1.end(),
              vct2.begin(), vct3.begin(),
              minus<T>());

    return vct3;
}

template <typename T>
void scaleVector(vector<T>& vct, const T scale)
{
    transform(vct.begin(),
              vct.end(),
              vct.begin(),
              bind1st(multiplies<T>(), scale));
    
    return;
}

template <typename T>
void scaleVectorD(vector<T>& vct){
    T max = *max_element(vct.begin(), vct.end());
    
    transform(vct.begin(), vct.end(), vct.begin(), invertD);
    transform(vct.begin(), vct.end(), vct.begin(),
              bind1st(multiplies<T>(), max));
    
    return;
}

template <typename T>
void minusVector(vector<T>& vct, T sub)
{
    transform(vct.begin(),
              vct.end(),
              vct.begin(),
              bind1st(minus<T>(), sub));
    return;
}

template <typename T>
vector<T> mulVectorElement(const vector<T>& vct1, const vector<T>& vct2)
{
    assert(vct1.size() == vct2.size());
    
    vector<T> vct3Element(vct1.size(), 1.0);
    transform(vct1.begin(), vct1.end(),
              vct2.begin(), vct3Element.begin(),
              multiplies<T>());
    
    return vct3Element;
}


template <typename T>
vector<T> divVectorElement(const vector<T>& vct1, const vector<T>& vct2)
{
    assert(vct1.size() == vct2.size());
    
    vector<T> vct3Element(vct1.size(), 1.0);
    transform(vct1.begin(), vct1.end(),
              vct2.begin(), vct3Element.begin(),
              divides<T>());
    
    return vct3Element;
}

template <typename T>
vector < vector<T> > mulVector(const vector< vector<T> >& vct1,
                               const vector< vector<T> >& vct2)
{
    assert(vct1.size() != 0
           && vct2.size() != 0);
    
    vector< vector<T> > vct3(vct1.size(), vector<T>(vct2.size()));
    
    FORI(vct1.size()) {
        FORJ(vct2.size()) {
            vct3[i][j] = (sumVector(mulVectorElement(vct1[i], vct2[j])));
        }
    }
    
    return vct3;
}

template <typename T>
vector < T > mulVector(const vector< vector<T> >& vct1,
                       const vector<T>& vct2)
{
    assert(vct1.size() != 0 &&
           (vct1[0]).size() == vct2.size());
    
    vector< T > vct3(vct1.size(), 1.0);

    FORI(vct1.size())
        vct3[i] = (sumVector(mulVectorElement(vct1[i], vct2)));
    
    return vct3;
}

template <typename T>
vector < T > mulVector(const vector<T>& vct1,
                       const vector< vector<T> >& vct2)
{
    return mulVector(vct2, vct1);
}


float* mulVectorArray(float * data,
                      const uint32_t total,
                      const uint8_t dim,
                      const  vector< vector<double> > vct)
{
    assert(vct.size() == dim
           && isSquare(vct));
    
    if(dim == 3) {
        for(uint32_t i = 0; i < total; i+=dim ){
            data[i] = vct[0][0]*(data[i]) + vct[0][1]*(data[i+1])
                        + vct[0][2]*(data[i+2]);
            data[i+1] = vct[1][0]*(data[i]) + vct[1][1]*(data[i+1])
                        + vct[1][2]*(data[i+2]);
            data[i+2] = vct[2][0]*(data[i]) + vct[2][1]*(data[i+1])
                        + vct[2][2]*(data[i+2]);
        }
    }
    else if (dim == 4) {
        for(uint32_t i = 0; i < total; i+=4 ){
            data[i] = vct[0][0]*data[i] + vct[0][1]*data[i+1]
                        + vct[0][2]*data[i+2] + vct[0][3]*data[i+3];
            data[i+1] = vct[1][0]*data[i] + vct[1][1]*data[i+1]
                        + vct[1][2]*data[i+2] + vct[1][3]*data[i+3];
            data[i+2] = vct[2][0]*data[i] + vct[2][1]*data[i+1]
                        + vct[2][2]*data[i+2] + vct[2][3]*data[i+3];
            data[i+3] = vct[3][0]*data[i] + vct[3][1]*data[i+1]
                        + vct[3][2]*data[i+2] + vct[3][3]*data[i+3];
        }
    }

    return data;
}

template <typename T>
T calSSE(vector<T>& tcp, vector<T>& src)
{
    assert(tcp.size() == src.size());
    vector<T> tmp(src.size());
    
    FORI(tcp.size())
        tmp[i] = tcp[i]-src[i];
    
    float result = accumulate(tmp.begin(), tmp.end(), 0.0f, square<T>());
    
    return result;
}

template<typename T>
vector < vector<T> > solveVM(const vector< vector<T> >& vct1, const vector< vector<T> >& vct2)
{
    return mulVector(invertVM3(vct1), vct2);
}

#endif

