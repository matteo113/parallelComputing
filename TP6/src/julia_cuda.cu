#include "stdio.h"
#include <iostream>
#include "Array2D.hpp"
#include <fstream>
#include <cuda_runtime.h>
#include <math.h>


// commpute the ammount of iteration before reaching divergence
__device__ int divergence(float2 z0, float2 c, double bound, int imax){
    float2 z = make_float2(z0.x, z0.y);
    for(int i=0; i<imax; i++){
        float norm = sqrt(z.x*z.x + z.y*z.y);
        if(norm > bound) return i;
        float zx = z.x, zy = z.y;
        z.x = zx*zx - zy*zy + c.x;
        z.y = 2*zx*zy + c.y;
    }
    return imax;
}

// convert discrete dcoordinate to the complex plan
__device__ float2 coord2cplx(float2 ll, float2 ur, int x, int y, int2 d){
    float2 res = make_float2( ll.x + x*(ur.x - ll.x)/d.x, -(ll.y + y*(ur.y - ll.y)/d.y ));
    return res;
}


// julia computation inspired of "Addition d'une constante sur une matrice" on 
// http://unige.ch/spc/en/info/teaching/parallelisme/programmation-sur-gpu-avec-cuda/
__global__ void julia(float2 ll, float2 ur, float2 c, int imax, int2 d, int* deviceDomain){
    int2 index = make_int2(blockDim.x * blockIdx.x + threadIdx.x, blockDim.y * blockIdx.y + threadIdx.y);
    if (index.x < d.x && index.y < d.y){
        deviceDomain[index.y*d.x+index.x] = divergence( coord2cplx(ll, ur, index.x, index.y, d), c, 2.0, imax);
    }
}

// an Array2D as a PGM image
void writePgm(Array2D<int>& d, int imax, std::string filename){
    std::ofstream file;
    file.open (filename);
    file << "P2" << std::endl;
    file << d.sizeX() << " " << d.sizeY() << std::endl;
    file << imax << std::endl;
    for(int y=0; y<d.sizeY(); y++){
        for(int x=0; x<d.sizeX(); x++){
            file << d(x, y) << " ";
        }
        file << std::endl;
    }
    file.close();
}

// Main function to be executed on host
int main(int argc, char** argv)
{
    float2 lowerLeft = make_float2(std::stof(argv[1]), std::stof(argv[2]));
    float2 upperRight = make_float2(std::stof(argv[3]), std::stof(argv[4]));
    float2 c = make_float2(std::stof(argv[5]), std::stof(argv[6]));
    int imax = std::stoi(argv[7]);
    int2 domain = make_int2(std::stoi(argv[8]), std::stoi(argv[9]));
    std::string filename(argv[10]);

    size_t domainSize = domain.x*domain.y*sizeof(int);

    // host memory allocation and setting
    Array2D<int> hostDomain(domain.x, domain.y, 0);

    // device memory allocation
    int* deviceDomain;
    cudaMalloc(&deviceDomain, domainSize);

    // device memory setting
    cudaMemset(deviceDomain, 0, domainSize);

    // exec configuration from http://unige.ch/spc/en/info/teaching/parallelisme/programmation-sur-gpu-avec-cuda/
    dim3 dimBlock(16, 16);
    dim3 dimGrid((domain.x + dimBlock.x - 1) / dimBlock.x, (domain.y + dimBlock.y - 1) / dimBlock.y);

    // setting of the time measurement 
    cudaEvent_t start, stop;
    float time;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);


    // kernel execution
    cudaEventRecord(start, 0);
    julia<<<dimGrid, dimBlock>>>(lowerLeft, upperRight, c, imax, domain, deviceDomain);
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time, start, stop);


    std::cout << "compute : " << time << std::endl;

    // reset of the time measurment
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);
    // Device to host data copy after computation
    cudaMemcpy(hostDomain.data(), deviceDomain, domainSize, cudaMemcpyDeviceToHost);

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time, start, stop);


    std::cout << "data copy: " << time << std::endl;

    writePgm(hostDomain, imax, filename);

    // device memory free
    cudaFree(deviceDomain);
}
