#include <complex>
#include <iostream>
#include <fstream>
#include <functional>
#include <mutex>

int attributedLine = 0;
std::mutex mut;

// calcul la suite z = z*z+c jusqu'a ce que ||z||>bound
// retourne le nombre d'iterations jusqu'a divergence
int divergence(std::complex<double> z0, std::complex<double> c, double bound, int imax){
    std::complex<double> z = z0;
    for(int i=0; i<imax; i++){
        if(norm(z) > bound) return i;
        z = z*z + c;
    }
    return imax;
}

// convertir une coordonnee du domaine discret en corrdonnee dans le domaine complexe
std::complex<double> coord2cplx(const std::complex<double>& ll, const std::complex<double>& ur, int x, int y, Array2D<int>& d){
    std::complex<double> res( ll.real() + x*(ur.real() - ll.real())/d.sizeX(), -(ll.imag() + y*(ur.imag() - ll.imag())/d.sizeY()) );
    return res;
}

// calcul l'ensemble de julia
void julia(const std::complex<double>& ll, const std::complex<double>& ur, const std::complex<double>& c, int imax, Array2D<int>& d){
    for(int y=0; y<d.sizeY(); y++){
        for(int x=0; x<d.sizeX(); x++){
            d(x, y) = divergence( coord2cplx(ll, ur, x, y, d), c, 2.0, imax );
        }
    }
}

void juliaThreadSimple(const std::complex<double>& ll, const std::complex<double>& ur, const std::complex<double>& c, int imax, Array2D<int>& d, int offset, int nbLine){
    for(int y=offset; y<offset + nbLine; y++){
        for(int x=0; x<d.sizeX(); x++){
            d(x, y) = divergence( coord2cplx(ll, ur, x, y, d), c, 2.0, imax );
        }
    }
}

void juliaThreadStatic(const std::complex<double>& ll, const std::complex<double>& ur, const std::complex<double>& c, int imax, Array2D<int>& d, std::vector<int> lines){
    for(int i=0; i < lines.size(); i++){
        for(int x=0; x<d.sizeX(); x++){
            d(x, lines[i]) = divergence( coord2cplx(ll, ur, x, lines[i], d), c, 2.0, imax );
        }
    }
}

int attribLines(int l, Array2D<int> d){
	int tmp = attributedLine;
	attributedLine += l;
	return tmp;
}

void juliaThreadDynamic(const std::complex<double>& ll, const std::complex<double>& ur, const std::complex<double>& c, int imax, Array2D<int>& d, int l){
    int myLines = 0;
    while (attributedLine < d.sizeY()) {
        mut.lock();
        if (attributedLine < d.sizeY()) {
            myLines = attribLines( l, d);
        } else{
            mut.unlock();
            break;
        }
        mut.unlock();

        for(int y = myLines; y < myLines + l && y < d.sizeY(); y++){
            for(int x=0; x<d.sizeX(); x++){
                d(x, y) = divergence( coord2cplx(ll, ur, x, y, d), c, 2.0, imax );
            }
        }
    }
}

// ecrit le domaine sous forme d'image pgm
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
